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

#if 0
extern "C" {
#include <signal.h>
}
#endif

void usage();
// void sigint_handler(int dummy);

void shell(USBtmc *tek);
void screenshot(USBtmc *tek, const char *fname);
void dl_wave(USBtmc *tek);

static volatile bool interrupted = false;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();
        return 0;
    }

    // try to load the device
    USBtmc tek(argv[1]);

    if (!tek.connect()) {
        std::cerr << "Failed to connect to " << argv[1] << std::endl;
        return -1;
    }

    if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        usage();
        return 0;
    }

    if (argc < 3) {
        tek.write("*IDN?\n");
        std::cout << "Connected to: " << tek.readln() << std::endl;
        tek.close();
        return 0;
    }


    if (std::string(argv[2]) == "shell") {
        shell(&tek);
    } else if (std::string(argv[2]) == "screenshot") {
        if (argc < 4) {
            std::cout << "Usage: tekvisa [device] screenshot [filename]" 
                      << std::endl;
            return 0;
        }

        screenshot(&tek, argv[3]);
    } else if (std::string(argv[2]) == "dl-wave") {
        dl_wave(&tek);
    } else {
        usage();
        return 0;
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

#if 0
void sigint_handler(int dummy)
{
    interrupted = true;
}
#endif

void shell(USBtmc *tek)
{
    std::istringstream *iss;
    std::ofstream savefile;
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
            if ((*iss).eof()) {
                delete iss;
                std::cout << tek->readln() << std::endl;
                continue;
            }

            *iss >> par;
            savefile.open(par);
            savefile << tek->readln() << std::endl;
            savefile.close();
            delete iss;
            continue;
        } else if (cmd == "/readbin" || cmd == "/rb") {
            if ((*iss).eof()) {
                delete iss;
                std::cerr << "Usage: /readbin [filename]" << std::endl;
                continue;
            }

            unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * 1024);

            *iss >> par;
            savefile.open(par, std::ios::out | std::ios::binary);
    
            while (tek->readbinblock(buffer, 1024)) {
                savefile.write((char *) buffer, sizeof(buffer));
            }

            savefile.close();

            free(buffer);

            delete iss;
            continue;
        } else if (cmd.at(0) == '/') {
            delete iss;
            continue;
        } 

        tek->write(text.c_str());
        delete iss;
    }
}

void screenshot(USBtmc *tek, const char *fname)
{
    std::ofstream savefile;
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * 1024);

    tek->write("HARDCOPY:PORT FILE\n");
    tek->write("HARDCOPY:filename \"TEK.PNG\"\n");
    tek->write("HARDCOPY START\n");

    savefile.open(fname, std::ios::out | std::ios::binary);

    while (tek->readbinblock(buffer, 1024)) {
        savefile.write((char *) buffer, sizeof(buffer));
    }

    savefile.close();

}

void dl_wave(USBtmc *tek)
{
    std::cerr << "Unimplemented" << std::endl;
}
