#include <unistd.h>
#include <ifaddrs.h>
#include <string.h>
#include <netinet/in.h>
void PLAT_GetIPv4(char *ifname, char *addr) {
	struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in *sa;

    getifaddrs(&ifaddr);

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        if (strcmp(ifa->ifa_name, ifname) == 0) {
            sa = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), addr, INET_ADDRSTRLEN);
            freeifaddrs(ifaddr);
            return 0;
        }
    }

    freeifaddrs(ifaddr);
    return -1;
}