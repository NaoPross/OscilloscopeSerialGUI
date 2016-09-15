/* 
 * Nao Pross 2016
 * tekvisa - Tool to comunicate with TekVisa compatible devices
 *
 * Tested to work with 
 *      - Tektronix TDS 2024C
 */

#include "usb_driver.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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
    std::istringstream *iss;
    std::string text;
    std::string cmd;
    std::string par;

    while (tek->connected) {
        std::cout << "interactive> ";

        // catch ctrl+D / EOF
        if (!std::getline(std::cin, text)) {
            break;
        } else if (text.empty()) {
            continue;
        }

        iss = new std::istringstream(text);
        *iss >> cmd;
        
        if (cmd == "/quit" || cmd == "/q") {
            break;
        } else if (cmd == "/readln" || cmd == "/r") {
            if (*iss) {
                *iss >> par;
                std::ofstream savefile;
                savefile.open(par);
                savefile << tek->readln() << std::endl;
                savefile.close();
            } else {
                std::cout << tek->readln() << std::endl;
            }
            continue;
        } else if (cmd.at(0) == '/') {
            continue;
        } 

        tek->write(text.c_str());

        delete iss;
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
