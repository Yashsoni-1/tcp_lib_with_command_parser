#include "byte_circular_buffer.hpp"
#include <memory.h>


byte_circular_buffer::byte_circular_buffer(uint16_t size)
 :buffer_size(size), buffer(new unsigned char [size]), current_size(0), front(0), rear(0)
    {}

byte_circular_buffer::~byte_circular_buffer() {
    delete [] buffer;
}


uint16_t byte_circular_buffer::bcb_write(unsigned char *data, uint16_t data_size)
{
    if(this->bcb_is_full()) return 0;
    if(this->bcb_available_size() < data_size) return 0;
    
    if(this->front < this->rear) {
        memcpy(BCB(this, this->front), data, data_size);
        this->front += data_size;
        if(this->front == this->buffer_size) this->front = 0;
        this->current_size += data_size;
        return data_size;
    }
    
    uint16_t leading_space = this->buffer_size - this->front;
    
    if(data_size <= leading_space)
    {
        memcpy(BCB(this, this->front), data, data_size);
        this->front += data_size;
        if(this->front == this->buffer_size) this->front = 0;
        this->current_size += data_size;
        return data_size;
    }
    
    memcpy(BCB(this, this->front), data, leading_space);
    memcpy(BCB(this, 0), data + leading_space, data_size - leading_space);
    this->front = data_size - leading_space;
    this->current_size += data_size;
    return data_size;
}

uint16_t byte_circular_buffer::bcb_read(unsigned char *data, uint16_t data_size, bool remove_read_bytes)
{
    if(this->current_size < data_size) return 0;
    
    if(this->rear < this->front) {
        memcpy(data, BCB(this, this->rear), data_size);
        if(remove_read_bytes) {
            this->rear += data_size;
            if(this->rear == this->buffer_size) this->rear = 0;
            this->current_size -= data_size;
        }
        return data_size;
    }
    
    uint16_t leading_space = this->buffer_size - this->rear;
    
    if(data_size <= leading_space) {
        memcpy(data, BCB(this, this->rear), data_size);
        if(remove_read_bytes) {
            this->rear += data_size;
            if(this->rear == this->buffer_size) this->rear = 0;
            this->current_size -= data_size;
        }
        return data_size;
    }
    
    memcpy(data, BCB(this, this->rear), leading_space);
    memcpy(data, BCB(this, 0), data_size - leading_space);
    if(remove_read_bytes) {
        this->rear = data_size - leading_space;
        this->current_size -= data_size;
    }
    
    return data_size;
}

bool byte_circular_buffer::bcb_is_full()
{
    return this->buffer_size == this->current_size;
}

uint16_t byte_circular_buffer::bcb_available_size()
{
    return this->buffer_size - this->current_size;
}

void byte_circular_buffer::bcb_reset()
{
    this->current_size = 0;
    this->front = 0;
    this->rear = 0;
}

void byte_circular_buffer::bcb_print_snapshot()
{
    
}
