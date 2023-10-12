#ifndef tcp_client_service_manager_hpp
#define tcp_client_service_manager_hpp

#include <stdio.h>
#include <list>
#include <netinet/in.h>

class tcp_server_controller;
class tcp_client;

class tcp_client_service_manager
{
public:
    tcp_server_controller *tcp_svr_ctrlr;
    
    tcp_client_service_manager(tcp_server_controller *);
    ~tcp_client_service_manager();
    
    void start_tcp_client_service_manager_thread();
    void stop_tcp_client_service_manager_thread();
    void client_fd_start_listen(tcp_client *tcp_clnt);
    void tcp_start_svc_manager_thread_internal();
    void add_client_to_db(tcp_client *tcp_clnt);
    tcp_client *look_up_client_db(uint32_t ip_addr, uint16_t port_no);
private:
    int max_fd;
    fd_set active_fd_set;
    fd_set backup_fd_set;
    pthread_t *client_svc_mgr_thread;
    std::list<tcp_client *> tcp_client_db;
    
    void copy_client_fd_to_fd_set(fd_set *fd_st);
    int  get_max_fd();
};

#endif /* tcp_client_service_manager_hpp */
