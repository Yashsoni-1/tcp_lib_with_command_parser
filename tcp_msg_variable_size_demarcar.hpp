#ifndef tcp_msg_variable_size_demarcar_hpp
#define tcp_msg_variable_size_demarcar_hpp

#include <stdint.h>
#include "tcp_msg_demarcar.hpp"

class tcp_client;


class tcp_msg_variable_size_demarcar: public tcp_msg_demarcar
{
public:
    bool is_buffer_ready_to_flush();
    void process_client_msg(tcp_client *tcp_clnt);
    
    tcp_msg_variable_size_demarcar();
    ~tcp_msg_variable_size_demarcar();
    void destroy();
private:
    
};


#endif /* tcp_msg_variable_size_demarcar_hpp */
