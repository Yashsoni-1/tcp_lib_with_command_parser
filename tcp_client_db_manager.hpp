#ifndef tcp_client_db_manager_hpp
#define tcp_client_db_manager_hpp

#include <stdio.h>
#include <list>



class tcp_server_controller;
class tcp_client;

class tcp_client_db_manager
{
public:
    tcp_server_controller *tcp_svr_ctrlr;
    
    tcp_client_db_manager(tcp_server_controller *);
    ~tcp_client_db_manager();
    
    void start_tcp_client_db_mgr_init();
    void add_client_to_db(tcp_client *tcp_clnt);
    void display_client_db();
private:
    std::list<tcp_client *> tcp_client_db;
    
};

#endif
