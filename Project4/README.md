##Project 4 

Server Program
    is the fourth project that entailed using C++. This program is essentially ran by the server resided at the Data Center at CEC department, Miami University. In NetBeans IDE, connect the IDE with Miami University Server, and all the written C++ programs will be uploaded and synced with the correcponding student ID folder in the database that resides in the server. 

#Program Files 
    main.cpp
    Server.h
    Server.cpp
    
#Program Description: 
    This Server Program acts as a server that constantly listening requests from a client (e.g. web-browser or mobile app etc) via a given port number (normally, 80 or other). This program serves the client by sending a response, typically, contents of a file requested by the client. 
    In this case, we could use either a web-browser or a terminal with linux commands to run and test the program. 
    
#Program Implementation and Design Guidelines 

    Server class can operate as a web-server that can respond to a GET request for a file path. 
    - Get request format: 
        - GET file HTTP/1.1
    
    >Web Server Rules: 
    
1. If the file is / the server should respond with contents of given file, index.html 
    
2. If the file path is valid (that is file is readable), the server should respond with the file's contents and appropriate HTTP headers & MIME type for the file deduced from the file name extension. The extension and MIME types are determined as tabulated: 
    
        | Extension |  MIME type |
        |:--------  | :--------  |
        | .html     |  text/html |
        | .png      |  image/png |
        | .jpg      |  image/jpeg|
        | .txt      |  text/plain|
        |other cases|  text/plain|
        
        
The contents of the file can be sent to the client line-by-line. 

3. If the file is not readable the server should respond with the following HTTP 404 erro response and body. The content-length and file name would change based on the file specified in the GET request. 

   --------------------------------------------------
    HTTP/1.1 404 Not Found 
    Server: SimpleServer 
    Content-Length: 47 
    Connection: Close Content-Type: text/plain 
    
    The following file was not found: data/blah.txt

   --------------------------------------------------
    
#Functional Testing 

    main.cpp is used for testing purposes. Run the program in NetBeans first (following Miami Coding Guidelines, if no errors incur), if using web-browser to do initial testing. 
    - When using Chrome, in the search bar, type in: 
        http://ceclnx01.cec.miamioh.edu:Port/Path
        
    For correct functional testing, you must test the operation of  your web-server using the <wget> command on ceclnx01. 
    
*wget* is a simple console program that acts as web-browser to GET data from any given URL with the following option: 
    *The -S option tells wget to print headers sent by the web-server
    *The -q option tells wget to be as quiet (not print additional info) 
    * The -O option tells wget to write the data from the web-server to a given file
    
    $ wget -S -q "http://ceclnx01.cec.miamioh.edu:Port/Path" -O my_Path 2> my_Path_hdrs.txt 
    $ diff my_Path Path
    $ diff my_Path_hdrs.txt Path_hdrs.txt 
 
 Where Path is one of the following names (i.e., the word Path must be replaced with one of the following file names):
 
    * test.txt
    * cpp.png
    * benton.jpg
    * index.html
    * blah.txt (this one is only designated for testing error message/case)

    ** Note that any file names that the server (this program) has can be requested by the client. Even <Server.cpp> is able to request by the client as long as that file exists in the prgram. 
    
    

        

