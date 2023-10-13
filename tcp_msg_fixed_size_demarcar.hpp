#ifndef tcp_msg_fixed_size_demarcar_hpp
#define tcp_msg_fixed_size_demarcar_hpp

#include <stdint.h>
#include "tcp_msg_demarcar.hpp"

class tcp_client;


class tcp_msg_fixed_size_demarcar: public tcp_msg_demarcar
{
public:
    bool is_buffer_ready_to_flush();
    void process_client_msg(tcp_client *tcp_clnt);
    
    tcp_msg_fixed_size_demarcar(uint16_t fixed_size);
    ~tcp_msg_fixed_size_demarcar();
    void destroy();
private:
    uint16_t msg_fixed_size;
};

#endif /* tcp_msg_fixed_size_demarcar_hpp */
