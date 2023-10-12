#include <iostream>
#include "tcp_server_controller.hpp"
#include "tcp_client.hpp"
#include "network_utils.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

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

int main(int argc, const char * argv[]) {
    
    std::unique_ptr<tcp_server_controller> server1 =
        std::make_unique<tcp_server_controller>("127.0.0.1", 40000, "Default TCP Server");

    server1->set_server_notif_callback(appln_client_connected, appln_client_disconnected, appln_client_msg_recvd);
    server1->start();

    scanf("\n");
    
    server1->display();
    
    
    
    
    return 0;
}
