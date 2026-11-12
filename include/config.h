#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define SPS 10
#define AMP 1500

static int freq1 = 800e6;
static int freq2 = 850e6;
static int mode  = 0;   // 0-bpsk, 1-qpsk

static const int16_t h[SPS] = {1,1,1,1,1,1,1,1,1,1};

#endif