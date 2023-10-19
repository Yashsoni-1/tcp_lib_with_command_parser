#include "tcp_client.hpp"
#include "network_utils.hpp"
#include <unistd.h>
#include "tcp_msg_demarcar.hpp"

tcp_client::tcp_client(uint32_t ip_addr, uint16_t port_no)
    : ip_addr(ip_addr), port_no(port_no), server_ip_addr(0), server_port_no(0), msgd(nullptr){}

void tcp_client::display()
{
    std::cout << "Tcp client: [" << network_convert_ip_n_to_p((this->ip_addr), NULL)
    << ", " << (this->port_no) << "] connected to [" << network_convert_ip_n_to_p((this->server_ip_addr), NULL)
    << ", " << (this->server_port_no) << "]\n";
}

void tcp_client::abort()
{
    if(this->comm_fd) {
        close(this->comm_fd);
        this->comm_fd = 0;
    }
    
    this->tcp_svr_crtrlr = nullptr;
    
    if(this->msgd) {
        this->msgd->destroy();
        delete this->msgd;
        this->msgd = NULL;
    }
    
    delete this;
}
