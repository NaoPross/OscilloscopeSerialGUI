#include "usb_driver.h"

USBtmc::USBtmc(const char *dev)
{
    this->dev = dev;
}

USBtmc::~USBtmc()
{
    this->close();
}

bool USBtmc::connect()
{
    struct termios term;
    
    if (access(this->dev, F_OK) != 0) {
        fprintf(stderr, "Device %s does not exists\n", this->dev);
        // fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    if (access(this->dev, R_OK) != 0) {
        fprintf(stderr, "Cannot read device: %s\n", this->dev);
        // fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    if (access(this->dev, W_OK) != 0) {
        fprintf(stderr, "Cannot write on device: %s\n", this->dev);
        // fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    this->devfd = open(this->dev, O_RDWR | O_NOCTTY | O_NDELAY);

    tcgetattr(this->devfd, &term);
    term.c_lflag &= ~ICANON;
    term.c_cc[VTIME] = 10; // 1 second
    tcsetattr(this->devfd, TCSANOW, &term);
    this->connected = true; 

    return true;
}

bool USBtmc::write(const char *txt)
{
    int buflen = strlen(txt);
    if (buflen != ::write(this->devfd, txt, buflen)) {
        // fprintf(stderr, "%s", strerror(errno));
        return false;
    }

    return true;
}

bool USBtmc::read(char *buf, size_t len)
{
    int rv;
    if ((rv = ::read(this->devfd, buf, len)) < 0) {
        // fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    return true;
}

bool USBtmc::readbinblock(unsigned char *buf, size_t size)
{
    int rv;
    bool nodata = true;

    rv = ::read(this->devfd, buf, size);

    if (rv > 0) {
        // fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    for (size_t i = 0; i < size; i++) {
        if (buf[i] != 0) {
            nodata = false;
            break;
        }
    }
    
    return nodata;
}

bool USBtmc::readbin(char *buf)
{
    std::cerr << "Unimplemented" << std::endl;
    return false;
}

std::string USBtmc::readln() 
{
    std::string buf;
    char tmpbuf;
    int bytes;

    do {
        bytes = ::read(this->devfd, &tmpbuf, 1);
        if (bytes > 0) {
            buf.append(&tmpbuf);
        }
    } while (tmpbuf != '\n' && tmpbuf != '\r');

    return buf;
}

void USBtmc::close()
{
    if (this->connected) {
        this->connected = false;
        ::close(this->devfd);
    }
}
