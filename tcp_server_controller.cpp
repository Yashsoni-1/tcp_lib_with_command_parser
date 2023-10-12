#include "tcp_server_controller.hpp"
#include "tcp_new_connection_acceptor.hpp"
#include "tcp_client_service_manager.hpp"
#include "tcp_client_db_manager.hpp"
#include "network_utils.hpp"


tcp_server_controller::tcp_server_controller(std::string ip_addr, uint16_t port, std::string name)
:   ip_addr(network_convert_ip_p_to_n(ip_addr.data())),
    port_no(port),
    name(name),
    tcp_new_con_acc(new tcp_new_connection_acceptor(this)),
    tcp_cl_db_mgr(new tcp_client_db_manager(this)),
    tcp_cl_svc_mgr(new tcp_client_service_manager(this))    {}


tcp_server_controller::~tcp_server_controller()
{}


void tcp_server_controller::start()
{
    tcp_new_con_acc->start_tcp_new_connection_acceptor_thread();
    tcp_cl_db_mgr->start_tcp_client_db_mgr_init();
    tcp_cl_svc_mgr->start_tcp_client_service_manager_thread();
    
    std::cout << "TCP Server is Up and Running [" << network_convert_ip_n_to_p(ip_addr, NULL) << ", "
    << port_no << "]\nOk\n";
}

void tcp_server_controller::stop()
{
    
}

void tcp_server_controller::process_new_client(tcp_client *tcp_clnt)
{
    this->tcp_cl_db_mgr->add_client_to_db(tcp_clnt);
    this->tcp_cl_svc_mgr->client_fd_start_listen(tcp_clnt);
}

void tcp_server_controller::set_server_notif_callback(void(*client_connected)(const tcp_server_controller *, const tcp_client*),
                               void(*client_disconnected)(const tcp_server_controller *, const tcp_client*),
                               void(*client_msg_recvd)(const tcp_server_controller *, const tcp_client*, unsigned char *, uint16_t))
{
    this->client_connected = client_connected;
    this->client_disconnected = client_disconnected;
    this->client_msg_recvd = client_msg_recvd;
}


void tcp_server_controller::display()
{
    tcp_client *tcp_clnt;
    std::list<tcp_client *>::iterator it;
    
    std::cout << "Server Name: " << this->name << '\n';
    std::cout << "Listening on : [" << network_convert_ip_n_to_p(this->ip_addr, NULL) << ", "
    << this->port_no << "]\n";
    
    this->tcp_cl_db_mgr->display_client_db();
}

