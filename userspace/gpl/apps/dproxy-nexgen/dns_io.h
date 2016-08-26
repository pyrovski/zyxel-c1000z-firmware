#include "dproxy.h"

int dns_read_packet(int sock, dns_request_t *m);
int dns_write_packet(int sock, struct in_addr in, int port, dns_request_t *m);
#ifdef DMP_X_404A03_IPV6_1
int dns_write_packet_ipv6(int sock, struct in6_addr in, int port, dns_request_t *m);
#endif