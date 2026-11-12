#ifndef SDR_H
#define SDR_H

#include <SoapySDR/Device.h>
#include <stdbool.h>

SoapySDRDevice *make_device(const char *uri);

#endif