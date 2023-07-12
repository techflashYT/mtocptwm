#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char *multicastIP;
    uint16_t multicastListenPort;

    uint16_t localListenPort;


    char name[64];

    int socket;
} netInfo_t;

extern netInfo_t netInfo;
extern void __attribute__ ((noreturn)) PLAT_Exit(bool err);
extern void PLAT_RxLoop(const uint_fast8_t delaySec);
