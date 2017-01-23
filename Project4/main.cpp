/**
 * A main file 
 * 
 * Copyright (C) 2016 mygitacc50@gmail.com/
 */

#include <string>
#include "Server.h"

int main(int argc, char *argv[]) {
    const int port = (argc == 2) ? std::stoi(argv[1]) : 0;
    Server httpd(port);
    
     if (argc == 2) {
        httpd.runServer();
    } else {
        // Process 1 request from cin/cout for functional testing
        httpd.serveClient(std::cin, std::cout);
    }
    return 0;
}
