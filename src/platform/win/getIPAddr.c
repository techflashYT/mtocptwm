#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iptypes.h>

void PLAT_GetIPv4(char *ifname, char *addr) {
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG bufferSize = 0;
    DWORD result = GetAdaptersAddresses(AF_INET, 0, NULL, NULL, &bufferSize);

    pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(bufferSize);

    result = GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &bufferSize);

    PIP_ADAPTER_ADDRESSES pAdapter = pAddresses;
    while (pAdapter) {
        if (strcmp(pAdapter->AdapterName, ifname) == 0) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAdapter->FirstUnicastAddress;
            while (pUnicast) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    struct sockaddr_in *sa = (struct sockaddr_in *)pUnicast->Address.lpSockaddr;
                    inet_ntop(AF_INET, &(sa->sin_addr), addr, INET_ADDRSTRLEN);
                    free(pAddresses);
                    return;
                }
                pUnicast = pUnicast->Next;
            }
        }
        pAdapter = pAdapter->Next;
    }

    free(pAddresses);
}