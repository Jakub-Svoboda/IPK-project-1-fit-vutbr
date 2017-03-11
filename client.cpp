/**
*IPK: project 1
*Author: Jakub Svoboda
*Email: xsvobo0z@stud.fit.vutbr.cz
*Date: 8.March 2017
*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <regex>
#include <unistd.h>


#define BUFFER_SIZE 1024
using namespace std;
int readSocket(int clientSocket){
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	int n = read(clientSocket,buffer,BUFFER_SIZE-1);
    if (n < 0){ 
		fprintf(stderr,"Error: reading from socket");
	}
	if(atoi(buffer)!=200){
		fprintf(stderr,"ERROR: received wrong code: %s %d.\n",buffer,atoi(buffer));
		exit(1);		//TODO  Validate.
	}	
	return 0;
}

//Sends a message and checks if writing to socket was ok.
void send(string message,int clientSocket){
    int n = write(clientSocket,message.c_str(),strlen(message.c_str()));
    if (n < 0){ 
		fprintf(stderr,"Error: Could not write to socket.");
	}	
}

//Returns port form argument as an integer. This could have been done with regex, but there is a g++ 4.8.5 on the testing machine. 
//The thing is that in versin 4.8.5 regex is not fully supported and because we were given this testing enviroment as is, this function
//must be used instead of a single line of regular exrepression.
int getPort(string remotePath){
	string port="";
	for(unsigned i = 0; i< remotePath.length(); i++){
		if(remotePath[i] == ':'){
			if(isdigit(remotePath[i+1])){
				port+=remotePath[i+1];
			}
			if(isdigit(remotePath[i+2])){
				port+=remotePath[i+2];
			}
			if(isdigit(remotePath[i+3])){
				port+=remotePath[i+3];
			}
			if(isdigit(remotePath[i+4])){
				port+=remotePath[i+4];
			}
			if(isdigit(remotePath[i+5])){
				port+=remotePath[i+5];
			}		
		}
	}
	return atoi(port.c_str());
}

//creates a socket and returns it
int getSocket(){  
	int clientSocket;
	//From https://wis.fit.vutbr.cz/FIT/st/course-files-st.php?file=%2Fcourse%2FIPK-IT%2Flectures%2Fipk2015-programovani-bsd_schranky.pdf&cid=11465
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0){
		perror("ERROR: Could not create socket.");
		exit(EXIT_FAILURE);
	}
	return clientSocket;
}

//returns the localPath as string type, empty if none
string getLocal(int argc, char* argv[]){
	if(argc == 3){
		const char *s = "";
		return s;		
	}else{
		return argv[3];
	}
}

//validates the first command, which must match one of the know commands
void validateArgs(int argc, char* argv[]){
	for(int i = 1; i<argc; i++){
		if(i==1){							//validate COMMAND
			if(!strcmp("del",argv[i])){
				continue;
			}
			else if(!strcmp("get",argv[i])){
				continue;
			}
			else if(!strcmp("put",argv[i])){
				continue;
			}
			else if(!strcmp("lst",argv[i])){
				continue;
			}
			else if(!strcmp("mkd",argv[i])){
				continue;
			}
			else if(!strcmp("rmd",argv[i])){
				continue;
			}else{
				fprintf(stderr,"Error: Wrong COMMAND argument %s \n",argv[i]);
				exit(400);		//TODO
			}
		}					
	}
}

int main(int argc, char* argv[]) {
	if(argc!=3 and argc!=4){
		fprintf(stderr,"Error: Wrong number of arguments %d \n",argc);
		exit(400);		//TODO
	}
	validateArgs(argc,argv);
	string remotePath(argv[2]);	
	string localPath(getLocal(argc,argv));
	int pathLen=strlen(remotePath.c_str());
	int port = getPort(remotePath);
	

	int clientSocket = getSocket();								//creates a socket
	struct hostent *server;										//describes host
	server = gethostbyname("localhost");						//get the IP
    if (server == NULL) {
        fprintf(stderr,"Error: no such host\n");
        exit(0);
    }
	
	struct sockaddr_in serverAddress;	
	bzero((char *) &serverAddress, sizeof(serverAddress));		//null the server adress
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_port=htons(port);
	bcopy((char *) server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
	//Connect
	if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) != 0) {
		perror("ERROR: Connection has failed");
		exit(EXIT_FAILURE);
	}	
	
	//Connected, read the message
	
	
	string message = argv[1];	
	std::transform(message.begin(), message.end(), message.begin(), std::ptr_fun<int, int>(std::toupper));		//uppercase the command
	size_t pos = remotePath.find_last_of(to_string(port));														//cut the argument behind the port number
	char buf[1000];
	time_t t = time(0);
	struct tm time = *gmtime(&t);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &time);		//format for Date
	
	string justPath = remotePath.substr(pos+1);
	message+= " ";
	message+=justPath;	
	message+=" HTTP/1.1";
	message+="\n";
	message+="Date: ";
	message+=buf;
	message+="\n";
	message+="Accept: text/plain\n";
	message+="Accept-Encoding: gzip, deflate\n";
	message+="Content-Type: text/plain\n";
	message+="Content-Length: ";
	message+=to_string(pathLen);
	message+="\n";
	message+="\r\n";
	
	send(message,clientSocket);
	readSocket(clientSocket);
	
    
	
	
	
	close(clientSocket);

}













