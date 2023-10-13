#include <algorithm>
#include <sys/select.h>
#include "tcp_client_service_manager.hpp"
#include "tcp_client.hpp"
#include "tcp_server_controller.hpp"
#include "tcp_msg_demarcar.hpp"


unsigned char client_recv_buffer[MAX_CLIENT_BUFFER_SIZE];

tcp_client_service_manager::tcp_client_service_manager(tcp_server_controller *tcp_svr_cntrlr)
: tcp_svr_ctrlr(tcp_svr_cntrlr), max_fd(0) {
    
    FD_ZERO(&this->active_fd_set);
    FD_ZERO(&this->backup_fd_set);
    
    client_svc_mgr_thread = new pthread_t();
}


tcp_client_service_manager::~tcp_client_service_manager() {}

void tcp_client_service_manager::tcp_start_svc_manager_thread_internal()
{
    size_t rcv_bytes;
    tcp_client *tcp_clnt, *next_tcp_clnt;
    sockaddr_in client_addr;
    std::list<tcp_client *>::iterator it;
    
    socklen_t addr_len = sizeof(client_addr);
    FD_ZERO(&this->backup_fd_set);
    this->copy_client_fd_to_fd_set(&this->backup_fd_set);
    this->max_fd = get_max_fd();
    
    while (true) {
        memcpy(&this->active_fd_set, &this->backup_fd_set, sizeof(fd_set));
        select(max_fd + 1, &this->active_fd_set, 0, 0, 0);
        
        for(it = this->tcp_client_db.begin(), tcp_clnt = *it;
            it != this->tcp_client_db.end();
            tcp_clnt = next_tcp_clnt)
        {
            next_tcp_clnt = *(++it);
            if(FD_ISSET(tcp_clnt->comm_fd, &this->active_fd_set))
            {
                rcv_bytes = recvfrom(tcp_clnt->comm_fd,
                                     client_recv_buffer,
                                     MAX_CLIENT_BUFFER_SIZE,
                                     0,
                                     (sockaddr *)&client_addr, &addr_len);
                
                if(rcv_bytes == 0) {
                    std::cout << "Errno no = " << errno << '\n';
                }
                
                if(tcp_clnt->msgd) {
                    tcp_clnt->msgd->process_msg(tcp_clnt, client_recv_buffer, rcv_bytes);
                } else if(this->tcp_svr_ctrlr->client_msg_recvd)
                {
                    this->tcp_svr_ctrlr->client_msg_recvd(this->tcp_svr_ctrlr,
                                                          tcp_clnt,
                                                          client_recv_buffer,
                                                          rcv_bytes);
                }
            }
            
            memset(client_recv_buffer, 0, MAX_CLIENT_BUFFER_SIZE);
            
        }
    }
    
}

void *tcp_client_svc_manager_thread_fn(void *arg)
{
    tcp_client_service_manager *svc_mgr = (tcp_client_service_manager *)arg;
    
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    svc_mgr->tcp_start_svc_manager_thread_internal();
    
    return NULL;
}


void tcp_client_service_manager::start_tcp_client_service_manager_thread()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    pthread_create(this->client_svc_mgr_thread, &attr, tcp_client_svc_manager_thread_fn, (void *)this);
    std::cout << "Service started: " << __FUNCTION__ << '\n';
}

void tcp_client_service_manager::stop_tcp_client_service_manager_thread()
{
    pthread_cancel(*this->client_svc_mgr_thread);
    pthread_join(*this->client_svc_mgr_thread, NULL);
    
    delete this->client_svc_mgr_thread;
    this->client_svc_mgr_thread = nullptr;
}

void tcp_client_service_manager::client_fd_start_listen(tcp_client *tcp_clnt)
{
    this->stop_tcp_client_service_manager_thread();
    std::cout << "Client service manager thread cancelled\n";
    
    this->add_client_to_db(tcp_clnt);
    
    this->client_svc_mgr_thread = new pthread_t;
    
    this->start_tcp_client_service_manager_thread();
}

int tcp_client_service_manager::get_max_fd()
{
    std::list<tcp_client *>::iterator it;
    tcp_client *tcp_clnt;
    
    int max_result  = -1;
    
    for(it = this->tcp_client_db.begin(); it != this->tcp_client_db.end(); ++it)
    {
        tcp_clnt = *it;
        
        if(tcp_clnt->comm_fd > max_result)
            max_result = tcp_clnt->comm_fd;
    }
    
    return max_result;
}

void tcp_client_service_manager::copy_client_fd_to_fd_set(fd_set *fd_st)
{
    std::list<tcp_client *>::iterator it;
    tcp_client *tcp_clnt;
    
    for(it = this->tcp_client_db.begin(); it != tcp_client_db.end(); ++it)
    {
        tcp_clnt = *it;
        FD_SET(tcp_clnt->comm_fd, fd_st);
    }
}

tcp_client* tcp_client_service_manager::look_up_client_db(uint32_t ip_addr, uint16_t port_no)
{
    std::list<tcp_client *>::iterator it;
    tcp_client *tcp_clnt = NULL;
    
    for(it = this->tcp_client_db.begin(); it != tcp_client_db.end(); ++it)
    {
        tcp_clnt = *it;
        if(tcp_clnt->ip_addr == ip_addr && tcp_clnt->port_no == port_no)
            return tcp_clnt;
    }
    
    return NULL;
}

void tcp_client_service_manager::add_client_to_db(tcp_client *tcp_clnt)
{
    if(tcp_clnt)
    {
        this->tcp_client_db.push_back(tcp_clnt);
    }
}
