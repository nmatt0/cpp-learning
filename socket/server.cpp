#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>

int main()
{
    int total = 0;
    int port = 10000;
    int backlog = 10;

    // socket
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        std::cout << "socket error code: " << errno << std::endl;
        std::cout << "socket error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // setsockopt SO_REUSEADDR & SO_REUSEPORT
    int options = 1;
    int status = setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options));
    if (status < 0)
    {
        std::cout << "setsockopt error code: " << errno << std::endl;
        std::cout << "setsockopt error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // bind
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    status = bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (status < 0)
    {
        std::cout << "bind error code: " << errno << std::endl;
        std::cout << "bind error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // listen
    status = listen(s, backlog);
    if (status < 0)
    {
        std::cout << "listen error code: " << errno << std::endl;
        std::cout << "listen error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // client address
    unsigned int client_addr_len;
    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr_len = sizeof(client_addr);

    while(true)
    {
        // accept
        int as = accept(s,(struct sockaddr *)&client_addr, &client_addr_len); // complete 3 way handshake
        if (as < 0)
        {
            std::cout << "accept error code: " << errno << std::endl;
            std::cout << "accept error msg: " << strerror(errno) << std::endl;
            return 1;
        }

        // recv
        char buffer[2048];
        ssize_t read = recv(as, buffer, sizeof(buffer), 0); // blocking read call
        if (read == 0)
        {
            shutdown(as, SHUT_RDWR);
            close(as);
            continue;
        }

        // parse recevied data
        buffer[read] = 0;
        std::string command(buffer);
        command.erase(command.find_last_not_of("\r\n") + 1, std::string::npos);
        if (command == "ADD")
        {
            total++;
        }
        else if (command == "SUB")
        {
            total--;
        }
        else if (command == "GET")
        {
        }
        else
        {
            std::cout << "INVALID COMMAND" << std::endl;
            shutdown(as, SHUT_RDWR);
            close(as);
            continue;
        }
        std::cout << "total: " << total << std::endl;
        
        // send
        std::string response;
        response = std::to_string(total);
        response = response + "\r\n";
        ssize_t sent = send(as, response.c_str(), response.size(), 0);

        // shutdown and close accept socket
        status = shutdown(as, SHUT_RDWR);
        if (status < 0)
        {
            std::cout << "accept socket shutdown error code: " << errno << std::endl;
            std::cout << "accept socket shutdown error msg: " << strerror(errno) << std::endl;
        }
        status = close(as);
        if (status < 0)
        {
            std::cout << "accept socket close error code: " << errno << std::endl;
            std::cout << "accept socket close error msg: " << strerror(errno) << std::endl;
        }
    }

    return 0;
}
