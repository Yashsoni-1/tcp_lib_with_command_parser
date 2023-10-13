#include "tcp_new_connection_acceptor.hpp"
#include "tcp_server_controller.hpp"
#include "tcp_client.hpp"
#include "network_utils.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "tcp_msg_fixed_size_demarcar.hpp"
#include "tcp_msg_variable_size_demarcar.hpp"

tcp_new_connection_acceptor::
tcp_new_connection_acceptor(tcp_server_controller *tcp_svr_cntrlr)
                                : tcp_svr_ctrlr(tcp_svr_cntrlr)
{
    accept_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(accept_fd == -1)
    {
        std::cerr << "Error: Could not create Accept FD\n";
        exit(1);
    }
    
    accept_new_conn_thread = (pthread_t *)calloc(1, sizeof(pthread_t));
}

tcp_new_connection_acceptor::~tcp_new_connection_acceptor() {}

void tcp_new_connection_acceptor::start_tcp_new_connection_acceptor_thread_internal()
{
    int opt = 1, comm_sock_fd{};
    socklen_t addr_len = sizeof(sockaddr_in);
    struct sockaddr_in server_addr{},
                       client_addr{};
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = tcp_svr_ctrlr->port_no;
    server_addr.sin_addr.s_addr = htonl(tcp_svr_ctrlr->ip_addr);
    
    if(setsockopt(this->accept_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsocketopt() failed\n";
        exit(1);
    }
    
    if(setsockopt(this->accept_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsocketopt() failed\n";
        exit(1);
    }
    
    if(bind(this->accept_fd, (struct sockaddr *)&server_addr, addr_len) == -1)
    {
        std::cerr << "Error : bind() failed [" << network_convert_ip_n_to_p(tcp_svr_ctrlr->ip_addr, NULL)
                  << ' ' << tcp_svr_ctrlr->port_no << "] Error = " << errno << '\n';
        exit(1);
    }
    
    if(listen(this->accept_fd, 5) < 0)
    {
        std::cerr << "listen() failed\n";
        exit(1);
    }
    
    while (true) {
        comm_sock_fd = accept(this->accept_fd, (struct sockaddr *)&client_addr, &addr_len);
        
        if(comm_sock_fd < 0) {
            std::cerr << "Error : accept() failed errno = " << errno << '\n';
            continue;
        }
        
        tcp_client *tcp_clnt = new tcp_client(client_addr.sin_addr.s_addr, client_addr.sin_port);
        
        tcp_clnt->tcp_svr_crtrlr = this->tcp_svr_ctrlr;
        tcp_clnt->comm_fd = comm_sock_fd;
        
        if(this->tcp_svr_ctrlr->client_connected) {
            this->tcp_svr_ctrlr->client_connected(this->tcp_svr_ctrlr, tcp_clnt);
        }
        
        tcp_clnt->msgd = new tcp_msg_fixed_size_demarcar(25);
        
        this->tcp_svr_ctrlr->process_new_client(tcp_clnt);
        
        std::cout << "Connection Accepted from client ["
        << network_convert_ip_n_to_p(htonl(client_addr.sin_addr.s_addr), NULL)
        << ", " << (htons(client_addr.sin_port)) << "]\n";
    }
}

static void * tcp_listen_for_new_connections(void *arg)
{
    tcp_new_connection_acceptor *tcp_new_conn_acc =
    (tcp_new_connection_acceptor *) arg;
    
    tcp_new_conn_acc->start_tcp_new_connection_acceptor_thread_internal();
    return NULL;
}



void tcp_new_connection_acceptor::start_tcp_new_connection_acceptor_thread()
{
    if(pthread_create(this->accept_new_conn_thread, NULL, tcp_listen_for_new_connections, (void*)this))
    {
        std::cerr << __FUNCTION__ << "() Thread Creation failed, error = " << errno << '\n';
        exit(1);
    }
    
    std::cout << "Service Started : " << __FUNCTION__ << '\n';
}

