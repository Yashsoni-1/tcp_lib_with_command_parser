#include <iostream>
#include "tcp_server_controller.hpp"
#include "tcp_client.hpp"
#include "network_utils.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "CommandParser/libcli.h"
#include "CommandParser/cmdtlv.h"
#include <list>

#define TCP_SERVER_CREATE 1
#define TCP_SERVER_START 2
#define TCP_SERVER_SHOW_TCP_SERVER 3
#define TCP_SERVER_STOP_CONN_ACCEPT 4
#define TCP_SERVER_STOP_CLIENT_LISTEN 5
#define TCP_SERVER_STOP 6
#define TCP_SERVER_CONNECT_REMOTE 7

std::list<tcp_server_controller *> tcp_server_list;
uint16_t default_port_no = 40000;
const char *default_ip_addr = "127.0.0.1";

static tcp_server_controller*
tcp_server_lookup(std::string server_name) {
    
    tcp_server_controller *ctrlr;
    std::list<tcp_server_controller *>::iterator it;

    for(it = tcp_server_list.begin(); it != tcp_server_list.end(); ++it) {
        ctrlr = *it;
        if(ctrlr->name == server_name) {
            return ctrlr;
        }
    }

    return NULL;
}

static void print_client(const tcp_client *tcp_clnt)
{
    
    std::cout << "[" << network_convert_ip_n_to_p(ntohl(tcp_clnt->ip_addr), NULL) << ", " << (ntohs(tcp_clnt->port_no)) << "]\n";
}

static void print_server(const tcp_server_controller *tcp_svr)
{
    
    std::cout << "[" << network_convert_ip_n_to_p(tcp_svr->ip_addr, NULL) << ", " << (tcp_svr->port_no) << "]\n";
}

static void appln_client_connected(const tcp_server_controller *tcp_svr, const tcp_client *tcp_clnt)
{
    print_server(tcp_svr);
    std::cout << "Appln : Client Connected: ";
    print_client(tcp_clnt);
}
static void appln_client_disconnected(const tcp_server_controller *tcp_svr, const tcp_client *tcp_clnt)
{
    
}

static void appln_client_msg_recvd(const tcp_server_controller *tcp_svr, const tcp_client *tcp_clnt, unsigned char *msg, uint16_t msg_size)
{
    std::cout << __FUNCTION__ << "() Bytes recvd: " << msg_size << " msg: " << msg << '\n';
}

int config_tcp_server_handler(param_t *param, ser_buff_t *ser_buff, op_mode enable_or_disable)
{
    int cmd_code;
    const char *server_name = NULL;
    tlv_struct_t *tlv = NULL;
    tcp_server_controller *tcp_server = NULL;
    char *ip_addr = (char *)default_ip_addr;
    uint16_t port_no = default_port_no;
    const char *remote_ip_addr = NULL;
    uint16_t remote_port = 0;
    
    cmd_code = EXTRACT_CMD_CODE(ser_buff);

    TLV_LOOP_BEGIN(ser_buff, tlv) {
        if(strncmp(tlv->leaf_id, "tcp-server-name", strlen("tcp-server-name")) == 0) {
            server_name = tlv->value;
        } else if(strncmp(tlv->leaf_id, "tcp-server-addr", strlen("tcp-server-addr")) == 0) {
            ip_addr = tlv->value;
        } else if(strncmp(tlv->leaf_id, "tcp-server-port", strlen("tcp-server-port")) == 0) {
            port_no = atoi(tlv->value);
        } else if(strncmp(tlv->leaf_id, "remote-addr", strlen("remote-addr")) == 0) {
            remote_ip_addr = (tlv->value);
        } else if(strncmp(tlv->leaf_id, "remote-port", strlen("remote-port")) == 0) {
            remote_port = atoi(tlv->value);
        }  else {
            assert(0);
        }
    } TLV_LOOP_END;

    switch(cmd_code) {
        case TCP_SERVER_CREATE:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(tcp_server) {
                std::cout << "Error: Tcp server already exists\n";
                return -1;
            }
            tcp_server = new tcp_server_controller(std::string(ip_addr), port_no, std::string(server_name));
            tcp_server_list.push_back(tcp_server);
            tcp_server->set_server_notif_callbacks(appln_client_connected, appln_client_disconnected, appln_client_msg_recvd);
            break;
        
        case TCP_SERVER_START:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(!tcp_server) {
                std::cout << "Error: Tcp server do not exists\n";
                return -1;
            }
            tcp_server->start();
            break;
        
        case TCP_SERVER_STOP_CONN_ACCEPT:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(!tcp_server) {
                std::cout << "Error: Tcp server do not exists\n";
                return -1;
            }
            switch(enable_or_disable) {
                case CONFIG_ENABLE:
                    tcp_server->stop_connection_accepting_svc();
                    break;
                case CONFIG_DISABLE:
                    tcp_server->start_connection_accepting_svc();
                    break;
                default:
                    break;
            }
            break;
        
        case TCP_SERVER_STOP_CLIENT_LISTEN:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(!tcp_server) {
                std::cout << "Error: Tcp server do not exists\n";
                return -1;
            }
            switch(enable_or_disable) {
                case CONFIG_ENABLE:
                    tcp_server->stop_tcp_client_svc_mgr();
                    break;
                case CONFIG_DISABLE:
                    tcp_server->start_tcp_client_svc_mgr();
                    break;
            }
            break;
        
        case TCP_SERVER_STOP:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(!tcp_server) {
                std::cout << "Error: Tcp server do not exists\n";
                return -1;
            }
            switch(enable_or_disable) {
                case CONFIG_ENABLE:
                    tcp_server->stop();
                    break;
                case CONFIG_DISABLE:
                    std::cout << "Command negation is not available for this CLI\n";
                    return -1;
            }
            break;
        
        case  TCP_SERVER_CONNECT_REMOTE:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(!tcp_server) {
                std::cout << "Error: Tcp server do not exists\n";
                return -1;
            }
            switch(enable_or_disable) {
                case CONFIG_ENABLE:
                    tcp_server->create_active_client(network_convert_ip_p_to_n(remote_ip_addr), remote_port);
                    break;
                case CONFIG_DISABLE:
                    break;
            }
            break;
        default:
            break;
    }
    return 0;
}

static int 
show_tcp_server_handler(param_t *param, ser_buff_t *ser_buff, op_mode enable_or_disable) 
{
    int cmd_code;
    const char *server_name = nullptr;
    tlv_struct_t *tlv = nullptr;
    tcp_server_controller *tcp_server = nullptr;
   
    cmd_code = EXTRACT_CMD_CODE(ser_buff);

    TLV_LOOP_BEGIN(ser_buff, tlv) {
        if(strncmp(tlv->leaf_id, "tcp-server-name", strlen("tcp-server-name")) == 0) {
            server_name = tlv->value;
        } 
    } TLV_LOOP_END;

    switch(cmd_code) {
        case TCP_SERVER_SHOW_TCP_SERVER:
            tcp_server = tcp_server_lookup(std::string(server_name));
            if(tcp_server) {
                std::cout << "Error: Tcp server already exists\n";
                return -1;
            }
            tcp_server->display();
            break;
        default:
            break;
    }

    return 0;
}



static void tcp_build_config_cli_tree()
{
    param_t *config_hook = libcli_get_config_hook();

    {
        static param_t tcp_server;
        init_param(&tcp_server, CMD, "tcp-server", NULL, NULL, INVALID, NULL, "config tcp-server");
        libcli_register_param(config_hook, &tcp_server); 
        {
            static param_t server_name;
            init_param(&server_name, LEAF, NULL, config_tcp_server_handler, NULL, STRING, "tcp-server-name", "tcp server name");
            libcli_register_param(&tcp_server, &server_name); 
            set_param_cmd_code(&server_name, TCP_SERVER_CREATE);
            {
                static param_t connect;
                init_param(&connect, CMD, "connect", NULL, NULL, INVALID, NULL, "connect tcp-server");
                libcli_register_param(&server_name, &connect); 
                set_param_cmd_code(&server_name, TCP_SERVER_CREATE);
                {
                    static param_t remote_machine_addr;
                    init_param(&remote_machine_addr, LEAF, NULL, NULL, NULL, IPV4, "remote-addr", "Remote IPV4 address");
                    libcli_register_param(&connect, &remote_machine_addr); 
                   
                    {
                        static param_t remote_machine_port;
                        init_param(&remote_machine_port, LEAF, NULL, config_tcp_server_handler, NULL, INT, "remote-port", "Remote port");
                        libcli_register_param(&remote_machine_addr, &remote_machine_port); 
                        set_param_cmd_code(&remote_machine_port, TCP_SERVER_CONNECT_REMOTE);
                    }
                
                }
            }
            
            {
                static param_t dis_conn_accept;
                init_param(&dis_conn_accept, CMD, "disable-conn-accept", config_tcp_server_handler, NULL, INVALID, NULL, "Connection Accept Settings");
                libcli_register_param(&server_name, &dis_conn_accept);
                set_param_cmd_code(&dis_conn_accept, TCP_SERVER_STOP_CONN_ACCEPT);
            }

            {
                static param_t disable_client_listen;
                init_param(&disable_client_listen, CMD, "disable-client-listen", config_tcp_server_handler, NULL, INVALID, NULL, "Stop Listening");
                libcli_register_param(&server_name, &disable_client_listen);
                set_param_cmd_code(&disable_client_listen, TCP_SERVER_STOP_CLIENT_LISTEN);
            }

            {
                static param_t tcp_server_addr;
                init_param(&tcp_server_addr, LEAF, NULL, NULL, NULL, IPV4, "tcp-server-addr", "tcp server address");
                libcli_register_param(&server_name, &tcp_server_addr); 

                {
                    static param_t tcp_server_port;
                    init_param(&tcp_server_port, LEAF, NULL, config_tcp_server_handler, NULL, INT, "tcp-server-port", "tcp server port no");
                    libcli_register_param(&tcp_server_addr, &tcp_server_port); 
                    set_param_cmd_code(&tcp_server_port, TCP_SERVER_CREATE);
                }
            }

            {
                static param_t start;
                init_param(&start, CMD, "start", config_tcp_server_handler, NULL, INVALID, NULL, "start tcp server");
                libcli_register_param(&server_name, &start); 
                set_param_cmd_code(&start, TCP_SERVER_START);
            }

            support_cmd_negation(&server_name);
        }
    } 

    support_cmd_negation(config_hook);
}

static void tcp_build_show_cli_tree()
{
    param_t *show_hook = libcli_get_show_hook();

    {
        static param_t tcp_server;
        init_param(&tcp_server, CMD, "tcp-server", NULL, NULL, INVALID, NULL, "show tcp-server");
        libcli_register_param(show_hook, &tcp_server); 
        {
            static param_t server_name;
            init_param(&server_name, LEAF, NULL, show_tcp_server_handler, NULL, STRING, "tcp-server-name", "tcp server name");
            libcli_register_param(&tcp_server, &server_name); 
            set_param_cmd_code(&server_name, TCP_SERVER_SHOW_TCP_SERVER);
        }
    }
}

static void
tcp_build_cli()
{
    tcp_build_config_cli_tree();
    tcp_build_show_cli_tree();
}

int main(int argc, const char * argv[]) {

#if 0
    
    std::unique_ptr<tcp_server_controller> server1 =
        std::make_unique<tcp_server_controller>("127.0.0.1", 40000, "Default TCP Server");

    server1->set_server_notif_callback(appln_client_connected, appln_client_disconnected, appln_client_msg_recvd);
    server1->start();

    scanf("\n");
    
    server1->display();
    
#endif

    init_libcli();
    tcp_build_cli();
    start_shell();
    
    
    
    return 0;
}
