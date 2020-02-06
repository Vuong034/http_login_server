/*
INET 4021 Lab 1
wEB CGI SERVER BY Nam Vuong


EXAMPLE USAGE: 
IN COMMAND LINE: navigate to  the src/ directory containing serv.c
gcc serv.c -o serv 

ctrl+c to manually shutdown server

IN BROSWER: localhost:8000

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../conf/config.h"

int main(int argc, char *argv[]){
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;// server and client file descriptors
    char buf[MAXBUFSIZE];//request buffer
    int fdimg, fdgif;//file descriptors for getting img and gif
    int on = 1;//setsockopt
    struct stat st;
    FILE *efp,*afp;//error and access file pointers for log 	
    
    afp = fopen("../logs/alog.txt", "a");//open access log for appending
    efp = fopen("../logs/elog.txt", "a");//open error log for appending

    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_server < 0){
	fprintf(efp, "Could not create server file descriptor\n");//prints to server and prints to elog.txt
        exit(1);
    }	

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORTNO);//PORTNO in config.h

    if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
        fprintf(efp,"500 Internal Server Error: could not bind server to address\n");
        exit(1);
    }

    if(listen(fd_server, NUMCONS) == -1){
    	    fprintf(efp,"500 Internal Server Error: server unable to listen for clients\n");
    	    close(fd_server);
    	    exit(1);
    }

    fprintf(afp ,"--------Server Started-------\n Port number: %d\n Max buffer size: %d\n-----------\n",PORTNO, MAXBUFSIZE);//log 

   
   while(1){//server always running
        fprintf(afp ,"server running\n");//log 

        fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

        if(fd_client == -1){//unable to connect, but keep looking for clients
            fprintf(efp,"500 Internal Server Error: server unable connect to clients\n");
	    continue;
        }else{
	    fprintf(afp,"client connected:%d\n", fd_client);
        }

	pid_t cpid = fork();

        if(cpid == 0){//fork child

            close(fd_server);// close listening socket
            memset(buf, 0, MAXBUFSIZE);
            int ret = read(fd_client, buf, MAXBUFSIZE-1); //read request from client
	    if(ret == 0){//end of the buffer
		close(fd_client);
	    }else if(ret == -1){
		fprintf(efp, "404 file not found\n");
	    }else{
            	fprintf(afp ,"201 ok REQUEST IN BUFFER:\n%s\n", buf);//log 

	    }
	      
            if(!strncmp(buf, "GET /%22/images/banner1.jpg", 27)){///compare request and get jpg
                fprintf(afp,"-----202 GET METHOD image request received----\n");//log


		// Send header
    		write(fd_client, imageheader, sizeof(imageheader) - 1);
    		// Send image content
                fdimg = open("../images/banner1.jpg", O_RDONLY);//image file name
		if(fdimg == -1){
			fprintf(efp, "404 image file not found\n");
			close(fdimg);				
		}			       			
		int sent, jpgsize;		
		stat("../images/banner1.jpg", &st);//get file size
		jpgsize = st.st_size;		//set buffer to file size
		while(sent != jpgsize){ //byte size of file               
			char buffer[jpgsize]; 
                        int b = read(fdimg, buffer, jpgsize); //read img into buffer  		        
                        if(b == 0){//end of the buffer
			    close(fdimg);
		        }else if(b > 0){
				fprintf(afp,"202 ok img read\n");//log 

		        }else{ continue; }
			sent = send(fd_client, buffer, b, 0); // send buffer to client 
		}

		fprintf(afp,"-----200 ok img request succeeded----\n");//log
 

            }if(!strncmp(buf, "GET /%22/images/05_02.gif", 25)){///compare request and get gif
                fprintf(afp,"-----202 GET METHOD gif request recieved----\n");

		 // Send header
    		write(fd_client, gifheader, sizeof(gifheader) - 1);
    		// Send image content
                fdgif = open("../images/05_02.gif", O_RDONLY);//image file name
		if(fdgif == -1){
			fprintf(efp, "404 gif file not found\n");
			close(fdgif);				
		}	
                int sent1, gifsize;
		stat("../images/05_02.gif", &st);
		gifsize = st.st_size;
		while(sent1 != gifsize){ //byte size of file               
			char buffer[gifsize];
			int n = read(fdgif, buffer, gifsize);
			if(n == 0){//end of the buffer
			    close(fdgif);
		        }else if(n > 0){
	       	     	    fprintf(afp, "202 ok gif read\n");//log 
	            	}else{ continue;}
			sent1 = send(fd_client, buffer, n, 0);
		}

		fprintf(afp,"-----200 ok gif request succeeded----\n");//log

            }else if(!strncmp(buf, "GET /cgi-bin/hello.cgi", 22)){//send post cgi code 
                fprintf(afp,"202 get cgi request recieved\n");//log

		//look for query string in buf
   		char *token = strtok(buf, "\n");//tokenize buf 
    		char query[50];//query buf\
    	
			while (token != NULL) { 
        			if(strncmp(token,"GET /cgi-bin/hello.cgi?",23) == 0){//checks for get cgi
        	    			char end[strlen(token)];//pointer: start,middle,end
        	 
			       		char *start = token + strlen("GET /cgi-bin/hello.cgi?");//truncates path
		 		       	char *middle = end;

	 			       	strcpy(middle, start);// copy new string
    
	 			       	end[strlen(end) - 9]  = '\0';//seperate end portion
            			
       			   		strcpy(query, end);//copy to query buf
       		      
       			     		break;//break while loop
       			 	}
        			token = strtok(NULL, "\n"); 
   			} 
			
			close(1);//close STDOUT
			dup2(fd_client, 1);//copy client stdout
	
			char qstrfull[500];// completed query buff
			strcpy(qstrfull, "QUERY_STRING=");//content name
			strcat(qstrfull, query);//add query to buff
			putenv("GATEWAY_INTERFACE=CGI/1.0");
 			putenv("SCRIPT_FILENAME=cgi-bin/hello.cgi");
			putenv(qstrfull);//set query env variable
			putenv("REQUEST_METHOD=GET");
		
			system("../cgi-bin/hello.cgi");//prints html text from cgi to the browser
		

		fprintf(afp,"200 ok get cgi executed\n");//log
	   }else if(!strncmp(buf, "POST /cgi-bin/hello.cgi", 21)){//send post cgi code 
		fprintf(afp,"-----202 ok post cgi request received----\n");//log
     
		if(buf == NULL){		
			fprintf(efp,"400 bad request: unable to read request\n");
			close(fd_client);	
		}
		//parse post data 
                char *token = strtok(buf, "\n");//tokenize buf 
    		char data[50];//query buf

			while (token != NULL) { 
        			if(strncmp(token,"first_name=",11) == 0){//checks for get cgi
            				strcpy(data, token);//copy to data buf
            				break;
        			}
        			token = strtok(NULL, "\n"); 
   			}


		putenv("GATEWAY_INTERFACE=CGI/1.0");
 		putenv("SCRIPT_FILENAME=/cgi-bin/hello.cgi");

		//content length
		char datalen[100];// completed data buff
		char dlen[100];
		sprintf(dlen, "%ld", sizeof(data));
		strcpy(datalen, "CONTENT_LENGTH=");//content 
		strcat(datalen, data);//add data to buff

		putenv("REQUEST_METHOD=POST");
		
		//send to CGI	
		char datastr[100];// completed data buff
		strcpy(datastr, "DATA=");//content 
		strcat(datastr, data);//add data to buff
		putenv(datastr);

		close(1);//close STDOUT
		dup2(fd_client, STDOUT_FILENO);//copy client stdout
 		system("../cgi-bin/hello.cgi");//executes cgi 

		fprintf(afp,"200 ok post cgi executed\n");//log

            }else if((!strncmp(buf, "GET / HTTP/1.0", 14))|| (!strncmp(buf, "GET / HTTP/1.1", 14))){//depending on browser compatibility 1.1/1.0
		fprintf(afp,"-----202 html request recieved----\n");//log
		//send html header and html code
		char indexroot[50];
		indexroot[0] = '\0';//got to root
		strcat(indexroot, ROOT);			
		strcat(indexroot, INDEX); //build index.html path
		write(fd_client, webheader, sizeof(webheader) - 1);// header	
		int webfile = open(indexroot, O_RDONLY);
		if(webfile == -1){
			fprintf(efp, "404 gif file not found\n");
			close(webfile);				
		}	
		int indexsize;
		stat(indexroot, &st);//get file size of index.html
		indexsize = st.st_size;

		char webbuf[indexsize];
		int results = read(webfile, webbuf, sizeof(webbuf));
		if (results == EOF) {
			fprintf(efp,"404 file not found\n");
		}
		write(fd_client, webbuf, sizeof(webbuf));// index.html body
		close(webfile);	

		fprintf(afp,"-----200 ok html request succeeded----\n");//log
            }else{
		if(!buf){
			fprintf(efp,"400 bad request: unable to read request\n");
			close(fd_client);
		}			
		close(fd_client);		
	    }//client connection code
            
	    fprintf(afp, "client closing connection...\n");// log
	    exit(0);//
        
	}else if(cpid == -1){//error check
		close(fd_client);
        	fprintf(efp, "Fork process failed\n");// log
		exit(1);
        }else{//fork parent
        	close(fd_client);
        	fprintf(afp, "parent closing child client file...\n");// log
		waitpid(cpid,NULL,0);   	
        }//fork parent

    }//while loop    
    
    close(fd_server);// close listening socket        
    fprintf(afp, "server shutting down\n");// log	 
    fclose(afp);	
    fclose(efp);	    
    return 0;

}// main
