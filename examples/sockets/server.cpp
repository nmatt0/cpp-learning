
static int val = 0;

static void main(int argc, char ** argv)
{
    int s = socket(AF_INET, SOCK_STREAM, SOCK_CLOEXEC);

    if (s < 0)
    {
        LOG_ERROR_ERRNO("failed to open socket", errno);
        return 1;
    }

    int enable = 1;

    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)))
    {
        LOG_ERROR_ERRNO(...);
        return 1;
    }

    int port = 10200;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    int status = bind(s, (struct sockaddr *)&addr, size);
    if (status < 0)
    {
        LOG_ERROR_ERRNO(...);
        return 1;
    }

    status = listen(s, 10);
    if (status < 0)
    {
        LOG_ERROR_ERRNO(...);
        return 1;
    }

    while (true)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));

        int as = accept(s, (struct sockaddr *)addr, sizeof(addr));
        if (as < 0)
        {
            LOG_ERROR_ERRNO(...);
            continue;
        }

        unit32_t hdr;

        ssize_t read = recv(as, &hdr, sizeof(hdr), 0);

        if (read < sizeof(hdr))
        {
            LOG_ERROR_ERRNO(...);
            shutdown(as);
            close(as);
            continue;
        }

        unsigned int length = ntohs(hdr);
        
        char buf[2048];

        if (length > sizeof(buf))
        {
            LOG_ERROR_ERRNO(...);
            shutdown(as);
            close(as);
            continue;
        }

        read = recv(as, buf, length);
        buf[read] = 0;

        std::string cmd(buf);

        std::string response;

        if (cmd == "INCR")
        {
            val++;
        }
        else if (cmd == "DECR")
        {
            val--;
        }
        else
        {
            LOG_WARN("invalid command");
            shutdown(as);
            close(as);
            continue;
        }

        response = std::to_string(val);

        unsigned int to_send = htons(response.size());

        ssize_t sent = send(as, &to_send, sizeof(to_send), 0);
        if (sent < to_send)
        {
            LOG_WARN(...);
            shutdown(as);
            close(as);
            continue;
        }

        sent = send(as, &response.c_str(), response.size(), 0);
        if (sent < response.size())
        {
            LOG_WARN(...);
            shutdown(as);
            close(as);
            continue;
        }            

        status = shutdown(as, SHUT_WR);
        if (status < 0)
        {
            LOG_ERROR_ERRNO(...);
        }

        status = close(as);
        if (status < 0)
        {
            LOG_ERROR_ERRNO(...);
        }
    }
}
