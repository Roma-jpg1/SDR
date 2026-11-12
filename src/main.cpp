#include "tx_rx.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Использовать так: %s <rx|tx> [uri]\n", argv[0]);
    return 1;
  }
  const char *uri="usb:6.6.6";
  if (argc > 2) {uri=argv[2];}  
  if (strcmp(argv[1], "rx") == 0){return run(uri, false);}
  else if (strcmp(argv[1], "tx") == 0) {return run(uri, true);} 
  else {
    printf("Ошибка режима работы\n");
    return 1;
  }
}
