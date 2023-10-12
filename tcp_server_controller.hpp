#ifndef tcp_server_controller_hpp
#define tcp_server_controller_hpp

#include <iostream>

class tcp_new_connection_acceptor;
class tcp_client_db_manager;
class tcp_client_service_manager;
class tcp_client;

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
    void set_server_notif_callback(void(*client_connected)(const tcp_server_controller *, const tcp_client*),
                                   void(*client_disconnected)(const tcp_server_controller *, const tcp_client*),
                                   void(*client_msg_recvd)(const tcp_server_controller *, const tcp_client*, unsigned char *, uint16_t)
                                   );
    void display();
private:
    tcp_new_connection_acceptor *tcp_new_con_acc;
    tcp_client_db_manager *tcp_cl_db_mgr;
    tcp_client_service_manager *tcp_cl_svc_mgr;
};






#endif
