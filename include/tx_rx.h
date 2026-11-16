#ifndef TX_RX_H
#define TX_RX_H

#include <stdbool.h>

struct SignalData;

int run(const char *uri, bool is_tx, SignalData &data);

#endif
