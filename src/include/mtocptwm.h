#include <stdint.h>

typedef struct {
    char *multicastIP;
    uint16_t multicastListenPort;

    uint16_t localListenPort;


    char name[64];

    int socket;
} netInfo_t;

extern netInfo_t netInfo;