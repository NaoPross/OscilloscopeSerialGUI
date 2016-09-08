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
    
#if 0
    if (!access(this->dev, F_OK)) {
        fprintf(stderr, "Device %s does not exists\n", this->dev);
        return false;
    }
    if (!access(this->dev, R_OK)) {
        fprintf(stderr, "Cannot read device: %s\n", this->dev);
        return false;
    }
    if (!access(this->dev, W_OK)) {
        fprintf(stderr, "Cannot write on device: %s\n", this->dev);
        return false;
    }
#endif

    this->devfd = open(this->dev, O_RDWR);

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

bool USBtmc::read(const char *buf, int len)
{
    if (len != ::read(this->devfd, (void *) buf, len)) {
        // fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    return true;
}

std::string USBtmc::readln() 
{
    std::string buf;
    const char *tmpbuf = (const char *) malloc(sizeof(char));
    int bytes;

    do {
        bytes = ::read(this->devfd, (void *) tmpbuf, 1);
        if (bytes > 0) {
            buf.append(tmpbuf);
        }
    } while (tmpbuf[0] != '\n' && tmpbuf[0] != '\r');

    return buf;
}

void USBtmc::close()
{
    if (this->connected) {
        this->connected = false;
        ::close(this->devfd);
    }
}
