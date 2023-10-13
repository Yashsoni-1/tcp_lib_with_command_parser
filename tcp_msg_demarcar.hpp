#ifndef tcp_msg_demarcar_hpp
#define tcp_msg_demarcar_hpp

#include <stdint.h>

constexpr int DEFAULT_CBC_SIZE = 10240;

enum tcp_msg_demarcar_type {
    TCP_DEMARCAR_NONE,
    TCP_DEMARCAR_FIXED_SIZE,
    TCP_DEMARCAR_VARIABLE_SIZE,
    TCP_DEMARCAR_PATTERN
};

class byte_circular_buffer;
class tcp_client;

class tcp_msg_demarcar {
    
public:
    virtual bool is_buffer_ready_to_flush() = 0;
    virtual void process_client_msg(tcp_client *) = 0;
    
    tcp_msg_demarcar(uint16_t circular_buffer_len);
    tcp_msg_demarcar();
    ~tcp_msg_demarcar();
    
    void destroy();
    void process_msg(tcp_client *, unsigned char *msg_recvd, uint16_t msg_size);
protected:
    byte_circular_buffer *bcb;
    unsigned char *buffer;
private:
    
};



#endif /* tcp_msg_demarcar_hpp */
