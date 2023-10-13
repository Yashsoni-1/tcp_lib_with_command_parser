#include "tcp_msg_fixed_size_demarcar.hpp"
#include "tcp_client.hpp"
#include "byte_circular_buffer.hpp"
#include "tcp_msg_demarcar.hpp"
#include "tcp_server_controller.hpp"


tcp_msg_fixed_size_demarcar::tcp_msg_fixed_size_demarcar(uint16_t fixed_size)
: msg_fixed_size(fixed_size), tcp_msg_demarcar{DEFAULT_CBC_SIZE} {}

tcp_msg_fixed_size_demarcar::~tcp_msg_fixed_size_demarcar() {}

bool tcp_msg_fixed_size_demarcar::is_buffer_ready_to_flush()
{
    if((this->tcp_msg_demarcar::bcb->current_size / this->msg_fixed_size) > 0)
        return true;
    else
        return false;
}

void tcp_msg_fixed_size_demarcar::process_client_msg(tcp_client *tcp_clnt)
{
    uint16_t bytes_read;
    
    if(!this->is_buffer_ready_to_flush()) return;
    
    while ((bytes_read =
            this->tcp_msg_demarcar::bcb->bcb_read(this->tcp_msg_demarcar::buffer,
                                                  this->msg_fixed_size, true))) {
        tcp_clnt->tcp_svr_crtrlr->client_msg_recvd(tcp_clnt->tcp_svr_crtrlr,
                                                   tcp_clnt,
                                                   this->tcp_msg_demarcar::buffer,
                                                   bytes_read);
    }
}


void tcp_msg_fixed_size_demarcar::destroy()
{
    this->tcp_msg_demarcar::destroy();
}
