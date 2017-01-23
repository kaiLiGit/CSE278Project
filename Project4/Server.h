/* 
 * File:   Server.h
 * Author: Kai Li
 *
 * 
 * Copyright (C) 2016 mygitacc50@gmail.com/
 */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>

class Server {
public:
    /**
     * The constructor to have this class operate as a server on a given port.
     * 
     * @param port The port number on which this server should listen.
     */
    explicit Server(unsigned short port = 80);
    
    /**
     * The destructor (should have empty body).
     */
    virtual ~Server();
    
    /**
     * Serves one connection from 1 client by processing HTTP request
     * (ignoring headers) and responding to the request with contents 
     * of a file (specified in the GET request).
     * 
     * @param is The input stream from where the client request is to be read.
     * @param os The output stream where the response is to be written.
     */
    virtual void serveClient(std::istream& is = std::cin, 
                             std::ostream& os = std::cout);
    /**
     * Runs the program as a server processing incoming connections/requests
     * for ever.
     * 
     * @param port
     */
    virtual void runServer();
    
protected:
    /**
     * Sends the contents of the given file to the user.
     * Note that this method assumes that the specified file is valid and
     * is readable. 
     * 
     * @param os The output stream to where the file is to be written.
     * @param path The path to the file whose contents is to be sent to 
     * the user.
     * @param size The size (in number of bytes) for the file as reported
     * by the getFileSize() method in this class.
     */
    void sendFile(std::ostream& os, const std::string& path, int size);

    /**
     * Obtain the mime type of data based on file extension.
     * 
     * @param path The path from where the file extension is to be determined.
     * 
     * @return The mime type associated with the contents of the file.
     */
    std::string getMimeType(const std::string& path);
    
    /**
     * Send a fixed 404 error message with the path to the file in the
     * response message.
     * 
     * @param os The output stream to where the message is to be written.
     * 
     * @param path The path to the file specified in the GET request.
     */
    void send404(std::ostream& os, const std::string& path);

    /**
     * Convenience method to determine file size.
     * 
     * @param path The path to the file whose file size is to be returned.
     * @return The file size. or -1 if the file does not exist.
     */
    int getFileSize(const std::string& path);

    /**
     * This method is a convenience method that extracts file path
     * from a string of the form: "GET <path> HTTP/1.1"
     * 
     * @param req The request from which the file path is to be extracted.
     * @return The path to the file requested
     */
    std::string getFilePath(const std::string& req);
    
private:
    // The default file to return for "/"
    static const std::string RootFile;
    // The port number set for this server
    unsigned short port;
};

#endif /* SERVER_H */
