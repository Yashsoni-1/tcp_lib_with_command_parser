#include "tcp_client_db_manager.hpp"
#include "tcp_client.hpp"


tcp_client_db_manager::tcp_client_db_manager(tcp_server_controller *tcp_svr_cntrlr)
: tcp_svr_ctrlr(tcp_svr_cntrlr)
{
    pthread_rwlock_init(&this->rwlock, nullptr);
}


tcp_client_db_manager::~tcp_client_db_manager()
{
    pthread_rwlock_destroy(&this->rwlock);
}

void tcp_client_db_manager::start_tcp_client_db_mgr_init()
{
    
}

void tcp_client_db_manager::add_client_to_db(tcp_client *tcp_clnt)
{
    pthread_rwlock_wrlock(&this->rwlock);
    this->tcp_client_db.push_back(tcp_clnt);
    pthread_rwlock_unlock(&this->rwlock);
}

void tcp_client_db_manager::display_client_db()
{
    tcp_client *tcp_clnt;
    std::list<tcp_client *>::iterator it;
    
    pthread_rwlock_rdlock(&this->rwlock);
    for(it = this->tcp_client_db.begin(); it != tcp_client_db.end(); ++it)
    {
        tcp_clnt = *it;
        tcp_clnt->display();
    }
    pthread_rwlock_unlock(&this->rwlock);
}

void tcp_client_db_manager::copy_all_clients_to_list(std::list<tcp_client *> *ls)
{
    tcp_client *tcp_clnt;
    std::list<tcp_client *>::iterator it;
    
    pthread_rwlock_rdlock(&this->rwlock);
    for(it = this->tcp_client_db.begin(); it != tcp_client_db.end(); ++it)
    {
        tcp_clnt = *it;
        ls->push_back(tcp_clnt);
    }
    pthread_rwlock_unlock(&this->rwlock);
}

void tcp_client_db_manager::purge()
{
    tcp_client *tcp_clnt, *next_tcp_clnt;
    std::list<tcp_client *>::iterator it;
    
    pthread_rwlock_rdlock(&this->rwlock);
    for(it = this->tcp_client_db.begin(),tcp_clnt = *it;
        it != this->tcp_client_db.end();
        tcp_clnt = next_tcp_clnt)
    {
        next_tcp_clnt = *(++it);
        this->tcp_client_db.remove(tcp_clnt);
        tcp_clnt->abort();
    }
    
    pthread_rwlock_unlock(&this->rwlock);
}
