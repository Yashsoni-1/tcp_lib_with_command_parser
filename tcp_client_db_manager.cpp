#include "tcp_client_db_manager.hpp"
#include "tcp_client.hpp"


tcp_client_db_manager::tcp_client_db_manager(tcp_server_controller *tcp_svr_cntrlr)
: tcp_svr_ctrlr(tcp_svr_cntrlr) {}


tcp_client_db_manager::~tcp_client_db_manager() {}

void tcp_client_db_manager::start_tcp_client_db_mgr_init()
{
    
}

void tcp_client_db_manager::add_client_to_db(tcp_client *tcp_clnt)
{
    this->tcp_client_db.push_back(tcp_clnt);
}


void tcp_client_db_manager::display_client_db()
{
    tcp_client *tcp_clnt;
    std::list<tcp_client *>::iterator it;
    
    for(it = this->tcp_client_db.begin(); it != tcp_client_db.end(); ++it)
    {
        tcp_clnt = *it;
        tcp_clnt->display();
    }
}
