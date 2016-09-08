/* 
 * Nao Pross 2016
 * tekvisa - Tool to comunicate with TekVisa compatible devices
 *
 * Tested to work with 
 *      - Tektronix TDS 2024C
 */

#include "usb_driver.h"

#include <iostream>
#include <cctype>

extern "C" {
#include <signal.h>
}


void usage();
void sigint_handler(int dummy);

void shell(USBtmc *tek);
void screenshot(USBtmc *tek);
void dl_wave(USBtmc *tek);

static volatile bool interrupted = false;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();
        return -1;
    }

    // try to load the device
    USBtmc tek(argv[1]);

    if (!tek.connect()) {
        std::cerr << "Failed to connect to " << argv[1] << std::endl;
        return -1;
    }

    if (argc < 3) {
        tek.write("*IDN?\n");
        std::cout << "Connected to: " << tek.readln() << std::endl;
        tek.close();
        return 0;
    }

    if (!std::string(argv[2]).compare("shell")) {
        shell(&tek);
    } else if (!std::string(argv[2]).compare("screenshot")) {
        screenshot(&tek);
    } else if (!std::string(argv[2]).compare("dl-wave")) {
        dl_wave(&tek);
    }

    if (tek.connected) {
        tek.close();
    }

    return 0;
}

void usage()
{
    std::cout 

    << "Usage: tekvisa [device] [command]\n"
    << "Tool to communicate TekVISA compatible devices, this software\n"
    << "was originally developed to work with Tektronix TDS2024C.\n\n" 

    << "Available commands:\n"
    << "    shell           - open an interactive shell\n"
    << "    screenshot      - take a screenshot and dowload the image\n"
    << "    dl-wave         - dowload data points in dat or csv format\n"
    ;
}

void sigint_handler(int dummy)
{
    interrupted = true;
}

void shell(USBtmc *tek)
{
    std::string command;

    while (tek->connected) {
        std::cout << "interactive> ";
        std::cin >> command;
        
        if (!command.compare("/quit") || !command.compare("/q")) {
            break;
        } else if (!command.compare("/readln") || !command.compare("/r")) {
            std::cout << tek->readln() << std::endl;
            continue;
        } else if (command[0] == '/') {
            continue;
        } 

        for (auto it = command.begin(); it != command.end(); it++) {

        }

        tek->write(command.c_str());
    }
}

void screenshot(USBtmc *tek)
{
    std::cerr << "Unimplemented" << std::endl;
}

void dl_wave(USBtmc *tek)
{
    std::cerr << "Unimplemented" << std::endl;
}
