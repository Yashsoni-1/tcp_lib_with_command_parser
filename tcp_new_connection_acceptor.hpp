#ifndef tcp_new_connection_acceptor_hpp
#define tcp_new_connection_acceptor_hpp

#include <pthread.h>

class tcp_server_controller;

class tcp_new_connection_acceptor
{
public:
    tcp_server_controller *tcp_svr_ctrlr;
    
    tcp_new_connection_acceptor(tcp_server_controller *);
    ~tcp_new_connection_acceptor();
    
    void start_tcp_new_connection_acceptor_thread();
    void start_tcp_new_connection_acceptor_thread_internal();
    
private:
    int accept_fd;
    pthread_t *accept_new_conn_thread;
    
};

#endif /* tcp_new_connection_acceptor_hpp */
