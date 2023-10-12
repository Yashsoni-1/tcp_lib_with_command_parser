#ifndef byte_circular_buffer_hpp
#define byte_circular_buffer_hpp

#include <stdint.h>

#define BCB(bcb, n) (&bcb->buffer[n])

class byte_circular_buffer
{
public:
    unsigned char *buffer;
    uint16_t buffer_size;
    uint16_t front;
    uint16_t rear;
    uint16_t current_size;
    
    byte_circular_buffer(uint16_t size);
    ~byte_circular_buffer();
    void bcb_free();
    uint16_t bcb_write(unsigned char *data, uint16_t data_size);
    uint16_t bcb_read(unsigned char *data, uint16_t data_size, bool remove_read_bytes);
    bool bcb_is_full();
    uint16_t bcb_available_size();
    void bcb_reset();
    void bcb_print_snapshot();
    
private:
};

#endif /* byte_circular_buffer_hpp */
