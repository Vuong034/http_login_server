INET 4021
Lab 1 read me

By Nam Vuong

Included files in lab1 folder: 
readme.txt
index.html
src/serv.c serv.o
logs/alog.txt elog.txt
conf/config.h
images/banner1.jpg 05_02.gif
cgi-bin/hello.cgi

Server Usage: 
gcc serv.c -o serv 
./serv
serv takes no arguments 

Client/browser Usage: localhost:8000

serv.c:
Server stars up, creates a socket for itself binds to any address/ local address. Port number hard wired to 8000. Starts to listen for connections and connects to client socket. Each connected client sends header to server. Server responds by sending html header and body. The requests that are received by the server for files(jpg,gif,cgi) are sent to the client. Server can handle up to 5 clients and will fork a process to handle each client that connects to the server. Once client is connected server will read request and serve files to client. Requests are checked for correct header and file extension. Image files are opened and then sent through the send function and checked for correct bytes sent. CGI get is processed by getting querystring and sending it as an environment variable to the cgi program. The cgi is then executed and processes the querystring and redirects client to another webpage with the modified input. CGI post request is receieved and sends the content length and data to the post program to process data and creates a page with the modified data. Html header is checked for 1.0 or 1.1 depending on browser compatibilty. Then writes the index.html file to the client. Access and error logs are opened for appending when server starts and logs the program actions and errors in the txt files.

error codes:
500: Server error
400: Bad request
404: produces a 404 when file is not found
200: ok request successful

Index.html:
Web source file. Includes a jpq and gif on webpage. GET login takes users first and last name using the GET Method then redirects to html welcome page with their name. Post login takes user's first and last name and processes it via the post method. Users are then sent to a another webpage with their name written on the page. 

hello.cgi
Handles both GET and post methods. Data is taken from the form in index.html and sent to the cgi program. The program is written in perl and detects which method is being used prior to printing the data on an html webpage. 

Log files
alog.txt: Access log anytime clients access files or client connects.
elog: Any errors are logged into the error log file.

config.h
defines port usage, connections, index, root, and contains simple headers for jpg, gif, html.

