#ifndef MODULATE_H
#define MODULATE_H

#include <stdint.h>
#include <stddef.h>

void gen_bits(int *bits, size_t n);
void bpsk(const int *bits, int16_t *buff, size_t n);
void qpsk(const int *bits, int16_t *buff, size_t n);
void upsample(const int16_t *in, size_t n, int16_t *out_buff, size_t sps);
void convolve(const int16_t *in_upsmpl, int16_t *out, size_t num);

#endif