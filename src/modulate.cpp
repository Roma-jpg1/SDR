#include "modulate.h"
#include "config.h"
#include <stdlib.h>

void gen_bits(int *bits, size_t n) {
    for (size_t i = 0; i < n; i++) bits[i] = rand() & 1;}

void bpsk(const int *bits, int16_t *buff, size_t n) {
    for (size_t sym = 0; sym < n; sym++) {
        int bit = bits[sym];

        if (bit) {
            buff[2 * sym] = AMP;}
        else {
            buff[2 * sym] = -AMP;}
        buff[2 * sym + 1] = 0;}}

void qpsk(const int *bits, int16_t *buff, size_t n) {
    for (size_t sym = 0; sym < n; sym++) {
        int bit_i = bits[2 * sym];
        int bit_q = bits[2 * sym + 1];

        if (bit_i) {
            buff[2 * sym] = AMP;}
        else {
            buff[2 * sym] = -AMP;}

        if (bit_q) {
            buff[2 * sym + 1] = AMP;}
        else {
            buff[2 * sym + 1] = -AMP;}}}

void upsample(const int16_t *in, size_t n, int16_t *out_buff, size_t sps) {
    for (size_t sym = 0; sym < n; sym++)
    {
        size_t idx = sym * sps * 2;

        out_buff[idx] = in[2 * sym];
        out_buff[idx + 1] = in[2 * sym + 1];

        for (size_t k = 1; k < sps; k++){
            out_buff[idx + 2 * k] = 0;
            out_buff[idx + 2 * k + 1] = 0;}}}

void convolve(const int16_t *in_upsmpl, int16_t *out, size_t num) {
    for (size_t n = 0; n < num; n++){
        int32_t i = 0;
        int32_t q = 0;

        for (size_t k = 0; k < SPS; k++){
            if (n >= k){
                size_t idx = n - k;
                i += in_upsmpl[2 * idx] * h[k];
                q += in_upsmpl[2 * idx + 1] * h[k];}}

        out[2 * n] = (int16_t)i;
        out[2 * n + 1] = (int16_t)q;}}