/**
*IPK: project 1
*Author: Jakub Svoboda
*Email: xsvobo0z@stud.fit.vutbr.cz
*Date: 8. March 2017
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
#include <sstream>


#define BUFFER_SIZE 1024
using namespace std;

//checks if file exists
bool fileExists(string &str){
	ifstream file(str.c_str());
    return file.good();
}

//parses the remote path from 
string getUrl(string message){
	
	string url;
	std::istringstream f(message.c_str());
	std::string line;   
	getline(f, line);
	
	size_t pos = message.find_first_of("\n");
	url = message.substr(0,pos+1);						//remove all but first line
	url = url.substr(4,url.length());					//remove command
	url = url.substr(0,url.length()-10);				//remove HTTP/1.1
	return url;
}

//reads the command
void getRequest(int sockfd2){
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
    int n = read(sockfd2,buffer,BUFFER_SIZE-1);
    if (n < 0){
		fprintf(stderr,"Error: reading from socket\n");
    }
	string message(buffer);
	string url=getUrl(message);
	if(message[0] == 'D'){
		if(fileExists(url)){
			fprintf(stderr,"got delete and the file exists\n");
		}else{
			fprintf(stderr,"got delete but the file does not exist %s\n",url.c_str());
		}
				
	}else if (message[0] == 'G'){
		int n = write(sockfd2,"200",4);
		if (n < 0) {
			fprintf(stderr,"Error: Could not write to socket.\n");
		}
	}else if (message[0] == 'P'){
		int n = write(sockfd2,"200",4);
		if (n < 0) {
			fprintf(stderr,"Error: Could not write to socket.\n");
		}
	}else if (message[0] == 'L'){
		int n = write(sockfd2,"200",4);
		if (n < 0) {
			fprintf(stderr,"Error: Could not write to socket.\n");
		}
	}else if (message[0] == 'M'){
		int n = write(sockfd2,"200",4);
		if (n < 0) {
			fprintf(stderr,"Error: Could not write to socket.\n");
		}
	}else if (message[0] == 'R'){
		int n = write(sockfd2,"200",4);
		if (n < 0) {
			fprintf(stderr,"Error: Could not write to socket.\n");
		}
	}else{
		exit(1);	//Should not happen, commands are checked before on clients side.
	}
	
	fprintf(stderr,"%s",message.c_str());
	n = write(sockfd2,"200",4);
	if (n < 0) {
		fprintf(stderr,"Error: Could not write to socket.\n");
	}
}

//returns the port specified in a parameter
int getPort(int argc, char* argv[]){
	if(argc==3){
		return atoi(argv[2]);
	}else if (argc ==5){
		if(strcmp(argv[1],"-p")){
			return atoi(argv[2]);
		}else{
			return atoi(argv[4]);
		}
	}else{
		return 6677;					//port should be 6677 if no other value has been specified
	}	
}

//returns the root argument
char* getRoot(int argc, char* argv[]){
	if(argc == 1){
		return NULL;
	}else if(argc == 3 or argc == 5){
		if(!strcmp(argv[1],"-r")){
			return argv[2];
		}else{
			return NULL;
		}		
	}else{
		return NULL;
	}
} 

void validateArgs(int argc, char* argv[]){
	if(argc ==4 or argc ==2){
		fprintf(stderr,"Error: Wrong number of arguments %d \n",argc);
		exit(400);		//TODO
	}
	for(int i = 1; i<argc; i++){
		if(i==1){							//validate COMMAND
			if(!strcmp("-r",argv[i])){
				i++;
				continue;
			}else{
				if(!strcmp("-p",argv[i])){
					i++;
					continue;
				}else{
					fprintf(stderr,"Error: 3rd argument not -p, but: %s \n",argv[i]);
					exit(400);		//TODO
				}
			}
		}
		if(i==3){
			if(!strcmp("-p",argv[i])){
				i++;
				continue;
			}else{
				fprintf(stderr,"Error: 3rd argument not -p, but: %s \n",argv[i]);
				exit(400);		//TODO
			}
		}		
	}
}

int main(int argc, char* argv[]) {
	struct sockaddr_in serverAddress, clientAddress;
	if(argc>5){
		fprintf(stderr,"Error: Wrong number of arguments %d \n",argc);
		exit(400);		//TODO
	}
	validateArgs(argc,argv);
	char* root = getRoot(argc,argv);
	int port = getPort(argc,argv);	
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);						//create a socket
	if(sockfd <=0){
		fprintf(stderr,"Error: Server could not create a socket.\n");
		exit(-1);						//TODO
	}
	bzero((char *) &serverAddress, sizeof(serverAddress));				//null the adress before use
	serverAddress.sin_addr.s_addr = INADDR_ANY;	
	serverAddress.sin_family = AF_INET;  
	serverAddress.sin_port = htons(port);									//assign the port 
	bind(sockfd, (struct sockaddr *) &serverAddress,sizeof(serverAddress));
    if(sizeof(serverAddress) <0){
		fprintf(stderr, "Error: bind socket.\n");
		exit(-1);			//TODO
	}   
	listen(sockfd,10);
	socklen_t clilen = sizeof(clientAddress);
	int sockfd2 = accept(sockfd, (struct sockaddr *) &clientAddress, &clilen);
	if(sockfd2 < 0){
		fprintf(stderr, "Error: Socket error,\n");
	} 
	
	//connection has been established
	getRequest(sockfd2);
	
	
    	
	
	
	
	
    close(sockfd2);
    close(sockfd);
    return 200;		//TODO 
}