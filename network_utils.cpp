#include "network_utils.hpp"

#include <netinet/in.h>
#include <arpa/inet.h>

char *
network_convert_ip_n_to_p(uint32_t ip_addr, char *output_buffer)
{
    static char res_buff[16];
    char *out = NULL;
    
    out = !output_buffer ? res_buff : output_buffer;
    memset(out, 0, 16);
    ip_addr = htonl(ip_addr);
    inet_ntop(AF_INET, &ip_addr, out, 16);
    out[15] = '\0';
    
    return out;
}

uint32_t
network_convert_ip_p_to_n(const char *ip_addr)
{
    uint32_t res{};
    inet_pton(AF_INET, ip_addr, &res);
    res = htonl(res);
    
    return res;
}
