#ifndef tcp_client_hpp
#define tcp_client_hpp

#include <stdio.h>
#include <stdint.h>

class tcp_server_controller;

class tcp_client
{
public:
    uint32_t ip_addr;
    uint16_t port_no;
    int comm_fd;
    tcp_server_controller *tcp_svr_crtrlr;

    tcp_client(uint32_t ip_addr, uint16_t port_no);
    void display();
private:
};

#endif /* tcp_client_hpp */
