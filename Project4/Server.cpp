/* 
 * File:   Server.cpp
 * Author: Kai Li
 * 
 * Copyright (C) 2016 mygitacc50@gmail.com/
 */

#include "Server.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <fstream>

// The default file to return for "/"
const std::string Server::RootFile = "index.html";
    
Server::Server(unsigned short port) {
    this->port = port;
}

Server::~Server() {
    // Nothing to be done in the destructor.
}

// This method is a convenience method that extracts file path
// from a string of the form: "GET <path> HTTP/1.1"
std::string 
Server::getFilePath(const std::string& req) {
    std::cout << "Request: " << req << std::endl;
    if (req.size() < 8) {
        return "";  // Invalid request. 
    }
    size_t spc1 = req.find(' '), spc2 = req.rfind(' ');
    std::string path = req.substr(spc1 + 2, spc2 - spc1 - 2);
    if (path == "") {
        return RootFile;  // default root file
    }
    return path;
}

// Convenience method to determine file size.
int 
Server::getFileSize(const std::string& path) {
    std::ifstream file(path);
    if (file.good()) {
        file.seekg(0, std::ios_base::end);
        return file.tellg();
    }
    return -1;
}

void 
Server::send404(std::ostream& os, const std::string& path) {
    const std::string msg = "The following file was not found: " + path;
    // Send a fixed message back to the client.
    os << "HTTP/1.1 404 Not Found\r\n"
       << "Server: SimpleServer\r\n"
       << "Content-Length: " << msg.size() << "\r\n"
       << "Connection: Close\r\n"
       << "Content-Type: text/plain\r\n\r\n"
       << msg;
}

// Obtain the mime type of data based on file extension.
std::string 
Server::getMimeType(const std::string& path) {
    const size_t dotPos = path.rfind('.');
    if (dotPos != std::string::npos) {
        const std::string ext = path.substr(dotPos + 1);
        if (ext == "html") {
            return "text/html";
        } else if (ext == "png") {
            return "image/png";
        } else if (ext == "jpg") {
            return "image/jpeg";
        }
    }
    // In all cases return default mime type.
    return "text/plain";
}

// Send contents of a given file back to the user by copying contents
// to the specified output stream.
void 
Server::sendFile(std::ostream& os, const std::string& path, int size) {
    // Get the mime type from the file extension.
    std::cout << "Sending file: " << path << std::endl;
    const std::string mimeType = getMimeType(path);
    // First write HTTP header.
    os << "HTTP/1.1 200 OK\r\n"
       << "Server: SimpleServer\r\n"
       << "Content-Length: " << size << "\r\n"
       << "Connection: Close\r\n"
       << "Content-Type: " << mimeType << "\r\n\r\n";
    // Copy contents of the file to the output stream.
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        os << line << std::endl;
    }
}

// Process HTTP request (from first line & headers) and
// provide suitable HTTP response back to the client.
void 
Server::serveClient(std::istream& is, std::ostream& os) {
    // Read headers from client and print them. This server
    // does not really process client headers
    std::string line;
    // Read the GET request line.
    std::getline(is, line);
    const std::string path = getFilePath(line);
    if (path.empty()) {
        send404(os, path);  // bad request. Can't do much
        return;
    }
    // Skip/ignore all the HTTP request & headers for now.
    while (std::getline(is, line), line != "\r") {}
    // Get the file size (if path exists)
    int fileSize = getFileSize(path);
    if (fileSize == -1) {
        // File not found. Return 404 error message.
        send404(os, path);
    } else {
        // Send contents of the file to the client.
        sendFile(os, path, fileSize);
    }
}

// Runs the program as a server that listens to incoming connections.
void 
Server::runServer() {
    // Setup a server socket to accept connections on the socket
    using namespace boost::asio;
    using namespace boost::asio::ip;
    io_service service;
    // Create end point
    tcp::endpoint myEndpoint(tcp::v4(), port);
    // Create a socket that accepts connections
    tcp::acceptor server(service, myEndpoint);
    std::cout << "Server is listening on " << port 
              << " & ready to process clients...\n";
    // Process client connections one-by-one...forever
    while (true) {
        tcp::iostream client;
        // Wait for a client to connect
        server.accept(*client.rdbuf());
        // Process information from client.
        serveClient(client, client);
    }
}
