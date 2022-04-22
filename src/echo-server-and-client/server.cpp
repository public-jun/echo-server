#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>

// using namespace std;

bool is_int(std::string s)
{
    size_t i = 0;
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
        ++i;
    }
    return true;
}

int main(int ac, char** av)
{
    std::cout << av[1] << std::endl;
    if (ac != 2 || !is_int(static_cast<std::string>(av[1])))
    {
        std::cerr << "[ERROR] Port is not provided via command line parameters!"
                  << std::endl;
        return -1;
    }

    // ソケットを作る && fd を取得
    int sock_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listener < 0)
    {
        std::cerr << "[ERROR] Socket cannot be created!" << std::endl;
        return -2;
    }

    std::cout << "[INFO] Socket has benn created." << std::endl;

    // Address info to bind socket
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(atoi(av[1]));
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    // OR
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

    char buf[INET_ADDRSTRLEN];

    // Bind socket
    if (bind(sock_listener, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "[ERROR] Created socket cannot be binded to"
                  << inet_ntop(AF_INET, &server_addr.sin_addr, buf,
                               INET_ADDRSTRLEN)
                  << ":" << ntohs(server_addr.sin_port) << ")" << std::endl;
        return -3;
    }

    std::cout << "[INFO] Sock is binded to ("
              << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
              << ":" << ntohs(server_addr.sin_port) << ")" << std::endl;

    // Start listening
    if (listen(sock_listener, SOMAXCONN) < 0)
    {
        std::cerr << "[ERROR] Socket cannot be swithed to listen mode!"
                  << std::endl;
        return -4;
    }

    std::cout << "[INFO] Socket is listening now." << std::endl;

    // Accept a call
    sockaddr_in client_addr;
    socklen_t   client_addr_size = sizeof(client_addr);
    int         sock_client;
    if ((sock_client = accept(sock_listener, (sockaddr*)&client_addr,
                              &client_addr_size)) < 0)
    {
        std::cerr << "[ERROR] Connections cannot be accepted for a reason."
                  << std::endl;
        return -5;
    }

    std::cout << "[INFO] A connection is accepted now." << std::endl;

    // Close main lister socket
    close(sock_listener);
    std::cout << "[INFO] Main listener socket is closed." << std::endl;

    // Get name info
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    if (getnameinfo((sockaddr*)&client_addr, client_addr_size, host, NI_MAXHOST,
                    svc, NI_MAXSERV, 0) != 0)
    {
        std::cout << "[INFO] Client: ("
                  << inet_ntop(AF_INET, &client_addr.sin_addr, buf,
                               INET_ADDRSTRLEN)
                  << ":" << ntohs(client_addr.sin_port) << ")" << std::endl;
    }
    else
    {
        std::cout << "[INFO] Client: (host: " << host << ", service: " << svc
                  << ")" << std::endl;
    }

    char msg_buf[4096];
    int bytes;
    // while receving -display & echo msg;
    while (true)
    {
        bytes = recv(sock_client, &msg_buf, 4096, 0);
        // check how many bytes received
        // If there is no data, it means server is disonnected
        if (bytes == 0)
        {
            std::cout << "[INFO] Client is disconnected." << std::endl;
            break;
        }
        // If something gone wrong
        else if (bytes < 0)
        {
            std::cerr << "[ERROR] Something weng wrong while reveiving data!." << std::endl;
            break;
        }
        // If there is some bytes
        else
        {
            // print msg
            sleep(2);
            std::cout << "client> " << std::string(msg_buf, 0, bytes) << std::endl;
            // Resend the same msg
            if (send(sock_client, &msg_buf, bytes, 0) < 0)
            {
                std::cerr << "[ERROR] Message cannot be send, exiting..." << std::endl;
                break;
            }
        }
    }

    // Close client socket
    close(sock_client);
    std::cout << "[INFO] Client socket is closed." << std::endl;

    return 0;
}
