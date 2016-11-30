#ifndef __USB_DRIVER_H__
#define __USB_DRIVER_H__

#include <iostream>
#include <string>

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
}

class USBtmc {
private:
    int devfd;
    const char *dev;

    // struct pollfd

public:
    bool connected;
    size_t max_buf_bytes = 10000;

    USBtmc(const char *dev);
    ~USBtmc();

    bool connect();
    bool write(const char *txt);
    bool read(char *buf, size_t len);
    bool readbinblock(unsigned char *buf, size_t size);
    bool readbin(char *buf);
    std::string readln();
    void close();
};

#endif
