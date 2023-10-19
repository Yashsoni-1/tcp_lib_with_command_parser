#ifndef tcp_server_controller_hpp
#define tcp_server_controller_hpp

#include <iostream>
#include <list>

class tcp_new_connection_acceptor;
class tcp_client_db_manager;
class tcp_client_service_manager;
class tcp_client;

constexpr int TCP_SERVER_INITIALIZED = 1;
constexpr int TCP_SERVER_RUNNING = 2;
constexpr int TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS = 4;
constexpr int TCP_SERVER_NOT_LISTENING_CLIENTS = 8;
constexpr int TCP_SERVER_CREATE_MULTITHREADED_CLIENT = 16;

class tcp_server_controller
{
public:
    uint32_t ip_addr;
    uint16_t port_no;
    std::string name;
    
    void(*client_connected)(const tcp_server_controller *, const tcp_client*);
    void(*client_disconnected)(const tcp_server_controller *, const tcp_client*);
    void(*client_msg_recvd)(const tcp_server_controller *, const tcp_client*, unsigned char *, uint16_t);
    
    
    tcp_server_controller(std::string ip_addr, uint16_t port, std::string name);
    ~tcp_server_controller();
    
    void start();
    void stop();
    void process_new_client(tcp_client *tcp_client);
    void set_server_notif_callbacks(void(*client_connected)(const tcp_server_controller *, const tcp_client*),
                                   void(*client_disconnected)(const tcp_server_controller *, const tcp_client*),
                                   void(*client_msg_recvd)(const tcp_server_controller *, const tcp_client*, unsigned char *, uint16_t)
                                   );
    void display();
    void set_bit(uint32_t bit_value);
    void unset_bit(uint32_t bit_value);
    bool is_bit_set(uint32_t bit_value);
    void start_connection_accepting_svc();
    void stop_connection_accepting_svc();
    void stop_tcp_client_svc_mgr();
    void start_tcp_client_svc_mgr();
    void copy_all_clients_to_list(std::list<tcp_client *> *ls);
    void create_active_client(uint32_t server_ip_addr, uint16_t server_port_no);
private:
    tcp_new_connection_acceptor *tcp_new_con_acc;
    tcp_client_db_manager *tcp_cl_db_mgr;
    tcp_client_service_manager *tcp_cl_svc_mgr;
    uint32_t state_flags;
};






#endif
