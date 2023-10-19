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
    tcp_cl_svc_mgr(new tcp_client_service_manager(this)),
    state_flags(0)
{
    this->set_bit(TCP_SERVER_INITIALIZED);
}

tcp_server_controller::~tcp_server_controller() {}

void tcp_server_controller::start()
{
    
    tcp_new_con_acc->start_tcp_new_connection_acceptor_thread();
    tcp_cl_db_mgr->start_tcp_client_db_mgr_init();
    tcp_cl_svc_mgr->start_tcp_client_service_manager_thread();
    
    std::cout << "TCP Server is Up and Running [" << network_convert_ip_n_to_p(ip_addr, NULL) << ", "
    << port_no << "]\nOk\n";
    
    this->set_bit(TCP_SERVER_RUNNING);
}

void tcp_server_controller::stop()
{
    if(this->tcp_new_con_acc) {
        this->stop_connection_accepting_svc();
        this->set_bit(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS);
    }
    
    if(this->tcp_cl_svc_mgr) {
        this->stop_tcp_client_svc_mgr();
        this->set_bit(TCP_SERVER_NOT_LISTENING_CLIENTS);
    }
    
    this->tcp_cl_db_mgr->purge();
    delete this->tcp_cl_db_mgr;
    
    this->tcp_new_con_acc = nullptr;
    this->tcp_cl_svc_mgr = nullptr;
    this->tcp_cl_db_mgr = nullptr;
    
    this->unset_bit(TCP_SERVER_RUNNING);
    delete this;
}

void tcp_server_controller::process_new_client(tcp_client *tcp_clnt)
{
    this->tcp_cl_db_mgr->add_client_to_db(tcp_clnt);
    this->tcp_cl_svc_mgr->client_fd_start_listen(tcp_clnt);
}

void tcp_server_controller::set_server_notif_callbacks(void(*client_connected)(const tcp_server_controller *, const tcp_client*),
                               void(*client_disconnected)(const tcp_server_controller *, const tcp_client*),
                               void(*client_msg_recvd)(const tcp_server_controller *, const tcp_client*, unsigned char *, uint16_t))
{
    this->client_connected = client_connected;
    this->client_disconnected = client_disconnected;
    this->client_msg_recvd = client_msg_recvd;
}

void tcp_server_controller::display()
{
    std::cout << "Server Name : " << this->name << '\n';
    
    if(this->is_bit_set(TCP_SERVER_RUNNING)) {
        std::cout << "Tcp server is not running\n";
        return;
    }
    
    std::cout << "Listening on : [" << network_convert_ip_n_to_p(this->ip_addr, NULL) << ", "
    << this->port_no << "]\n";
    std::cout << "Flags : " << ((this->is_bit_set(TCP_SERVER_INITIALIZED)) ? "I" : "Un-I")
                            << ((this->is_bit_set(TCP_SERVER_RUNNING)) ? " R" : " Not-R")
                            << ((this->is_bit_set(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS)) ? " Acc" : " Not-Acc")
                            << ((this->is_bit_set(TCP_SERVER_NOT_LISTENING_CLIENTS)) ? " L" : " Not-L")
                            << ((this->is_bit_set(TCP_SERVER_CREATE_MULTITHREADED_CLIENT)) ? " M" : " Not-M");
    
    std::cout << '\n';
    
    this->tcp_cl_db_mgr->display_client_db();
}

void tcp_server_controller::stop_connection_accepting_svc()
{
    if(this->is_bit_set(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS)) return;
    this->tcp_new_con_acc->stop();
    this->tcp_new_con_acc = nullptr;
    this->set_bit(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS);
}

void tcp_server_controller::start_connection_accepting_svc()
{
    if(!this->is_bit_set(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS)) return;
    this->tcp_new_con_acc = new tcp_new_connection_acceptor(this);
    this->tcp_new_con_acc->start_tcp_new_connection_acceptor_thread();
    this->unset_bit(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS);
}

void tcp_server_controller::stop_tcp_client_svc_mgr()
{
    if(this->is_bit_set(TCP_SERVER_NOT_LISTENING_CLIENTS)) return;
    this->tcp_cl_svc_mgr->stop();
    this->tcp_cl_svc_mgr = nullptr;
    this->set_bit(TCP_SERVER_NOT_LISTENING_CLIENTS);
}

void tcp_server_controller::start_tcp_client_svc_mgr()
{
    if(!this->is_bit_set(TCP_SERVER_NOT_LISTENING_CLIENTS)) return;
    this->tcp_cl_svc_mgr = new tcp_client_service_manager(this);
    this->tcp_cl_svc_mgr->start_tcp_client_service_manager_thread();
    this->unset_bit(TCP_SERVER_NOT_LISTENING_CLIENTS);
}

void tcp_server_controller::copy_all_clients_to_list(std::list<tcp_client *> *ls)
{
    this->tcp_cl_db_mgr->copy_all_clients_to_list(ls);
}

void tcp_server_controller::set_bit(uint32_t bit_value)
{
    this->state_flags |= bit_value;
}

void tcp_server_controller::unset_bit(uint32_t bit_value)
{
    this->state_flags &= ~bit_value;
}

bool tcp_server_controller::is_bit_set(uint32_t bit_value)
{
    return (this->state_flags &= bit_value);
}


void tcp_server_controller::create_active_client(uint32_t server_ip_addr, uint16_t server_port_no)
{
    
}
