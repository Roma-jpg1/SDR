#include "tx_rx.h"
#include "gui.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <thread>


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Использовать так: %s <rx|tx> [uri]\n", argv[0]);
    return 1;
  }
  const char *uri="usb:6.6.6";
  if (argc > 2) {uri=argv[2];}  
  if (strcmp(argv[1], "rx") != 0 && strcmp(argv[1], "tx") != 0) {
    printf("Ошибка режима работы\n");
    return 1;
  }

  SignalData data;
  std::thread sdr_thread([&] {
    run(uri, strcmp(argv[1], "tx") == 0, data);
  });
  run_gui(data);
  sdr_thread.join();
  return 0;
}
