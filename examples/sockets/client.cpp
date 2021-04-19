static void main(int argc, char ** argv)
{
    int s = socket(AF_INET, SOCK_STREAM, SOCK_CLOEXEC);

    if (s < 0)
    {
        LOG_ERROR_ERRNO("failed to open socket", errno);
        return 1;
    }

    name_resolver addr;

    int status = name_resolver::resolve("www.cnn.com", "80", true, false, addr);
    if (status)
    {
        LOG_ERROR_ERRNO(...);
        close(s);
        return 1;
    }

    int cs = connect(s, (struct sockeraddr *)addr.addr, addr.addr_len);
    if (cs < 0)
    {
        LOG_ERROR_ERRNO(...);
        return 1;
    }

    std::string cmd;

    unsigned int to_send = htons(cmd.size());
    
    ssize_t sent = send(cs, &to_send, sizeof(to_send), 0);
    if (sent < to_send)
    {
        LOG_WARN(...);
        shutdown(cs);
        close(cs);
        return 1;
    }
    
    sent = send(cs, &cmd.c_str(), cmd.size(), 0);
    if (sent < response.size())
    {
        LOG_WARN(...);
        shutdown(cs);
        close(cs);
        return 1;
    }            
    
    unit32_t hdr;
    
    ssize_t read = recv(cs, &hdr, sizeof(hdr), 0);
    
    if (read < sizeof(hdr))
    {
        LOG_ERROR_ERRNO(...);
        shutdown(cs);
        close(cs);
        return 1;
    }
    
    unsigned int length = ntohs(hdr);
    
    char buf[2048];
    
    if (length > sizeof(buf))
    {
        LOG_ERROR_ERRNO(...);
        shutdown(cs);
        close(cs);
        return 1;
    }

    read = recv(cs, buf, length);
    if (read < length)
    {
        LOG_ERROR_ERRNO(...);
        shtudown(cs);
        close(cs);
        return 1;
    }
    buf[read] = 0;
    
    std::string val(buf);

    std::cout << buf << std::endl;
    std::cout.flush();

    shutdown(cs, SHUT_RDWR);
    close(cs);

    return 0;
}
