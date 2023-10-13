#include "tcp_client.hpp"
#include "network_utils.hpp"

tcp_client::tcp_client(uint32_t ip_addr, uint16_t port_no)
    : ip_addr(ip_addr), port_no(port_no), msgd(nullptr){}

void tcp_client::display()
{
    std::cout << "Tcp client: [" << network_convert_ip_n_to_p(htonl(this->ip_addr), NULL)
    << ", " << htons(this->port_no) << "]\n";
}
