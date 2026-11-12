#include "tx_rx.h"
#include "modulate.h"
#include "sdr.h"
#include "config.h"

#include <SoapySDR/Formats.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run(const char *uri, bool is_tx) {
    SoapySDRDevice *sdr = make_device(uri);

    int sample_rate = 1e6;
    int rx_freq = freq1;
    int tx_freq = freq2;
    const char *filename = "rxdata.pcm";

    if (is_tx) {
        rx_freq = freq2;
        tx_freq = freq1;
        filename = "txdata.pcm";
    }

    SoapySDRDevice_setSampleRate(sdr, SOAPY_SDR_RX, 0, sample_rate);
    SoapySDRDevice_setSampleRate(sdr, SOAPY_SDR_TX, 0, sample_rate);
    SoapySDRDevice_setFrequency(sdr, SOAPY_SDR_RX, 0, rx_freq, NULL);
    SoapySDRDevice_setFrequency(sdr, SOAPY_SDR_TX, 0, tx_freq, NULL);

    size_t channels[] = {0};
    SoapySDRDevice_setGain(sdr, SOAPY_SDR_RX, *channels, 45.0);
    SoapySDRDevice_setGain(sdr, SOAPY_SDR_TX, *channels, 45.0);
    size_t channel_count = sizeof(channels) / sizeof(channels[0]);

    SoapySDRStream *rxStream = SoapySDRDevice_setupStream(sdr, SOAPY_SDR_RX, SOAPY_SDR_CS16, channels, channel_count, NULL);
    SoapySDRStream *txStream = SoapySDRDevice_setupStream(sdr, SOAPY_SDR_TX, SOAPY_SDR_CS16, channels, channel_count, NULL);

    SoapySDRDevice_activateStream(sdr, rxStream, 0, 0, 0);
    SoapySDRDevice_activateStream(sdr, txStream, 0, 0, 0);

    size_t rx_mtu = SoapySDRDevice_getStreamMTU(sdr, rxStream);
    size_t tx_mtu = SoapySDRDevice_getStreamMTU(sdr, txStream);

    int16_t *rx_buffer = (int16_t*)malloc(2 * rx_mtu * sizeof(int16_t));
    int16_t *tx_buff = (int16_t*)calloc(2 * tx_mtu, sizeof(int16_t));

    size_t total_samples = tx_mtu;
    size_t num_symbols = total_samples / SPS;
    size_t num_bits = (mode ? num_symbols * 2 : num_symbols);

    int *tx_bits = (int*)malloc(num_bits * sizeof(int));
    int16_t *mod_sym = (int16_t*)malloc(2 * num_symbols * sizeof(int16_t));
    int16_t *upsampled = (int16_t*)malloc(2 * total_samples * sizeof(int16_t));
    int16_t *final = (int16_t*)malloc(2 * total_samples * sizeof(int16_t));

    const long timeoutUs = 400000;
    long long last_time = 0;
    FILE *file = fopen(filename, "w");
    size_t buffers_read = 0;

    if (!is_tx) {
        for (size_t i = 2; i < 2 * tx_mtu; i += 2) {
            tx_buff[i] = 1500 << 4;
            tx_buff[i + 1] = 1500 << 4;}}

    void *rx_buffs[] = {rx_buffer};
    while (1) {
        int flags;
        long long timeNs = last_time;
        int sr = SoapySDRDevice_readStream(sdr, rxStream, rx_buffs, rx_mtu, &flags, &timeNs, timeoutUs);

        printf("Buffer: %lu - Samples: %i, Flags: %i, Time: %lli, TimeDiff: %lli\n", buffers_read, sr, flags, timeNs, timeNs - last_time);
        last_time = timeNs;

        long long tx_time = last_time + (4 * 1000 * 1000);

        if (is_tx) {
            gen_bits(tx_bits, num_bits);
            if (mode) qpsk(tx_bits, mod_sym, num_symbols);
            else bpsk(tx_bits, mod_sym, num_symbols);

            upsample(mod_sym, num_symbols, upsampled, SPS);
            convolve(upsampled, final, total_samples);

            memset(tx_buff, 0, 2 * tx_mtu * sizeof(int16_t));

            for (size_t i = 0; i < total_samples; i++) {
                tx_buff[10 + 2*i] = (int16_t)(final[2*i] << 4);
                tx_buff[10 + 2*i + 1] = (int16_t)(final[2*i + 1] << 4);}

            for (size_t i = 0; i < 2; i++) {
                tx_buff[0 + i] = 0xffff;
                tx_buff[10 + i] = 0xffff;}

            for (size_t i = 0; i < 8; i++) {
                int tx_time_byte = (tx_time >> (i * 8)) & 0xff;
                tx_buff[2 + i] = tx_time_byte << 4;}

            void *tx_buffs[] = {tx_buff};
            if( (buffers_read % 2 == 0) ){
                printf("buffers_read: %d\n", buffers_read);
                flags = SOAPY_SDR_HAS_TIME;
                int st = SoapySDRDevice_writeStream(sdr, txStream,(const void * const*)tx_buffs,total_samples, &flags,tx_time, timeoutUs);
                if ((size_t)st != total_samples){
                    printf("TX Failed: %i\n", st);}}
        }

        fwrite(rx_buffer, 2 * (size_t)sr * sizeof(int16_t), 1, file);
        buffers_read++;
    }

    fclose(file);
    free(rx_buffer);
    free(tx_buff);
    free(tx_bits);
    free(mod_sym);
    free(upsampled);
    free(final);
    SoapySDRDevice_deactivateStream(sdr, rxStream, 0, 0);
    SoapySDRDevice_deactivateStream(sdr, txStream, 0, 0);
    SoapySDRDevice_closeStream(sdr, rxStream);
    SoapySDRDevice_closeStream(sdr, txStream);
    SoapySDRDevice_unmake(sdr);
}