/*
Server settings
and html headers

*/
#define PORTNO 8000
#define MAXBUFSIZE 2048//client fd buf
#define NUMCONS 5//number of connections
#define ROOT "../"
#define INDEX "index.html" 



char webheader[] = 
"HTTP/1.0 200 Ok\r\n"
"Content-Type: text/html\r\n\r\n";

char imageheader[] = 
"HTTP/1.0 200 Ok\r\n"
"Content-Type: image/jpeg\r\n\r\n";

char gifheader[] = 
"HTTP/1.0 200 Ok\r\n"
"Content-Type: image/gif\r\n\r\n";


