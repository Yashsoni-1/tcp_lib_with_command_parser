
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>


char msg[] = "Namaste, from the client :)";

struct test_struct {
    int a, b;
};


void set_up_tcp_client(const char * server_ip, uint16_t server_port)
{
    struct sockaddr_in server_det;
    socklen_t addr_len = sizeof(sockaddr);
    int sock_fd{}, ret{};

    
    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sock_fd == -1)
    {
        std::cerr << "Socket creation failed\n";
        exit(1);
    }
    
    server_det.sin_family = AF_INET;
    server_det.sin_port = server_port;
    
    hostent *dest = (hostent *)gethostbyname(server_ip);
    server_det.sin_addr = *((in_addr *)dest->h_addr);
    
    ret = connect(sock_fd, (struct sockaddr *)&server_det, addr_len);
    
    if(ret == -1)
    {
        std::cerr << "Couldn't connect to server errno" << errno << "\n";
        exit(1);
    } else {
        std::cout << "Connected\n";
    }

    int a, b, c;
    test_struct t_s;

    while(true) {

        ret = sendto(sock_fd, (char *)&msg, strlen(msg), 0, (struct sockaddr *)&server_det, addr_len);
        if(ret == -1)
        {
                std::cerr << "Send fail  errno" << errno << "\n";
                exit(1);
        }
    }


    return;
}
int main(int argc, const char * argv[]) {

    if(argc < 3) {std::cerr << "Please enter ip and port\n";
                        exit(1);}
    set_up_tcp_client(argv[1], atoi(argv[2]));


    return 0;
}


