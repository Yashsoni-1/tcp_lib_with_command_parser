#include "tcp_msg_demarcar.hpp"
#include "byte_circular_buffer.hpp"
#include "tcp_client.hpp"
#include <assert.h>

tcp_msg_demarcar::tcp_msg_demarcar(uint16_t circular_buffer_len)
{
    this->bcb = new byte_circular_buffer(circular_buffer_len);
    this->buffer = new unsigned char[MAX_CLIENT_BUFFER_SIZE];
}

tcp_msg_demarcar::tcp_msg_demarcar()
{
    this->bcb = new byte_circular_buffer(DEFAULT_CBC_SIZE);
    this->buffer = new unsigned char[MAX_CLIENT_BUFFER_SIZE];
}


tcp_msg_demarcar::~tcp_msg_demarcar()
{
    assert(this->bcb);
    assert(this->buffer);
}

void tcp_msg_demarcar::destroy()
{
    if(this->bcb) {
        delete bcb;
        this->bcb = nullptr;
    }
    
    if(this->buffer) {
        delete [] buffer;
        this->buffer = nullptr;
    }
}

void tcp_msg_demarcar::process_msg(tcp_client *tcp_clnt, unsigned char *msg_recvd, uint16_t msg_size)
{
    assert(bcb->bcb_write(msg_recvd, msg_size));
    
    if(!this->is_buffer_ready_to_flush()) return;
    
    this->process_client_msg(tcp_clnt);
}
