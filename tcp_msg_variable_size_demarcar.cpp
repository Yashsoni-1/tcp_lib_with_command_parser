#include "tcp_msg_variable_size_demarcar.hpp"
#include "tcp_client.hpp"
#include "byte_circular_buffer.hpp"
#include "tcp_msg_demarcar.hpp"
#include "tcp_server_controller.hpp"


tcp_msg_variable_size_demarcar::tcp_msg_variable_size_demarcar()
:  tcp_msg_demarcar{DEFAULT_CBC_SIZE} {}

tcp_msg_variable_size_demarcar::~tcp_msg_variable_size_demarcar() {}

bool tcp_msg_variable_size_demarcar::is_buffer_ready_to_flush()
{
    return true;
}

void tcp_msg_variable_size_demarcar::process_client_msg(tcp_client *tcp_clnt)
{
    uint16_t bytes_read;
    
    if(!this->is_buffer_ready_to_flush()) return;
    unsigned short bytes_to_read = 0;
    
    while ((bytes_read =
            this->tcp_msg_demarcar::bcb->bcb_read((unsigned char *)(&bytes_to_read),
                                                  2, true))) {
        tcp_clnt->tcp_svr_crtrlr->client_msg_recvd(tcp_clnt->tcp_svr_crtrlr,
                                                   tcp_clnt,
                                                   this->tcp_msg_demarcar::buffer,
                                                   bytes_to_read-2);
    }
}


void tcp_msg_variable_size_demarcar::destroy()
{
    this->tcp_msg_demarcar::destroy();
}
