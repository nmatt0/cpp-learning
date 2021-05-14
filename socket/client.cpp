#include <iostream>
#include <string>
#include <algorithm> // std::transform
#include <cstring> // strerror
#include <arpa/inet.h> // sockaddr_in socket AF_INET SOCK_STREAM htons inet_pton connect send recv shutdown SHUT_RDWR
#include <unistd.h> // close

int main(int argc, char * argv[])
{
    std::string command;
    if (argc < 2)
    {
        command = "GET\r\n";
    }
    else
    {
        command = argv[1];
        std::transform(command.begin(), command.end(), command.begin(), std::ptr_fun<int, int>(std::toupper));
        command = command + "\r\n";
    }
    std::cout << command << std::endl;

    std::string server_host = "127.0.0.1";
    int server_port = 10000;
    char buffer[2048];

    sockaddr_in server_addr;
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        std::cout << "socket error code: " << errno << std::endl;
        std::cout << "socket error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
       
    int status = inet_pton(AF_INET, server_host.c_str(), &server_addr.sin_addr);
    if (status < 0)
    {
        std::cout << "inet_pton error code: " << errno << std::endl;
        std::cout << "inet_pton error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // connect
    status = connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (status < 0)
    {
        std::cout << "connect error code: " << errno << std::endl;
        std::cout << "connect error msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // send
    send(s, command.c_str(), command.size(), 0);

    // recv
    int read = recv(s, buffer, sizeof(buffer), 0);
    if (read == 0)
    {
        shutdown(s, SHUT_RDWR);
        close(s);
        return 1;
    }

    // print response from server
    buffer[read] = 0; //null terminate string
    std::string response(buffer);
    response.erase(response.find_last_not_of("\r") + 1, std::string::npos);
    response.erase(response.find_last_not_of("\n") + 1, std::string::npos);
    int resp;
    try
    {
        resp = std::stoi(response);
    }
    catch (...)
    {
        shutdown(s, SHUT_RDWR);
        close(s);
        return 1;
    }
    std::cout << "total: " << resp << std::endl;

    shutdown(s, SHUT_RDWR);
    close(s);
    return 0;
}
