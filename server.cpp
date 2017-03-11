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
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <regex>
#include <unistd.h>
#include <sstream>
#include <fstream>


#define BUFFER_SIZE 1024
using namespace std;

//returns true if path leads to a file
bool isItFile(string path){
	struct stat buf;
    stat(path.c_str(), &buf);
    return S_ISREG(buf.st_mode);
}

//creates a response message and sends it back to client
void respond(string response, string content, int code, int sockfd2){
	char buf[1000];
	time_t t = time(0);
	struct tm time = *gmtime(&t);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &time);		//format for Date
	
	string combined ="HTTP/1.1 ";
	combined+=to_string(code);
	combined += " ";
	combined += response;
	combined += "\n";
	combined +="Date: ";
	combined +=buf;
	combined += "\n";
	combined +="Content-Type: text/plain\n";
	combined +="Content-Length: ";
	combined +=to_string(strlen(content.c_str())-1);
	combined += "\n";
	combined += content;
	combined +="\r\n";

	int n = write(sockfd2, combined.c_str()  ,combined.length());
	if (n < 0) {
		fprintf(stderr,"Error: Could not write to socket.\n");
	}
}

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

//Function for the DEL operation
void del(string message, string url, int code, string root,string response, int sockfd2){
	string content="";
	if(url[0]=='/'){		//remove the /, we are deling with a file
		url=url.substr(1,url.length());
	}
	if(root.length() > 0 and root[root.length()]!='/'){	//append / to the end of url
		root+="/";
	}
	url=root+url;					//append root to the url, if not specified, "" is appended
	if(fileExists(url)){
		if(isItFile(url)){	
			if( remove(url.c_str()) != 0 ){
				response="Unknown error.";
				code=400;	//todo? neslo remove
				content=response;
			}else{	//removed just fine
				code=200;
				response="OK";
				content=response;
			}
		}else{		//not a file, its a folder
			code=400;
			response="Not a file.";
			content=response;
		}
			
	}else{
		response="File not found.";
		code = 404;		//todo
		content=response;
	}
	respond(response, content, code, sockfd2);	
}

//Function for the GET operation
void get(string message, string url, int code, string root,string response, int sockfd2){
	string content = "";
	if(url[0]=='/'){		//remove the /, we are deling with a file
		url=url.substr(1,url.length());
	}
	if(root.length() > 0 and root[root.length()]!='/'){	//append / to the end of url
		root+="/";
	}
	url=root+url;					//append root to the url, if not specified, "" is appended
	if(fileExists(url)){
		if(isItFile(url)){	
			std::ifstream ifs(url);
			string content2((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
			response="OK";
			code = 200;
			content=content2;
		}else{		//not a file, its a folder
			code=400;
			response="Not a file.";
			content=response;
		}			
	}else{
		response="File not found.";
		code = 404;		//todo
		content=response;
	}
	respond(response, content, code, sockfd2);	
}

//reads the command
void getRequest(int sockfd2, string root){
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
    int n = read(sockfd2,buffer,BUFFER_SIZE-1);
    if (n < 0){
		fprintf(stderr,"Error: reading from socket\n");
    }
	string message(buffer);
	string url=getUrl(message);
	int code=200;											//response code will be filled here
	string response;
	if(message[0] == 'D'){	//DEL operation
		del(message,url,code,root, response,sockfd2);				
	}else if (message[0] == 'G'){//GET operation	
		get(message,url,code,root, response,sockfd2);	
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
	
	//fprintf(stderr,"%s",message.c_str());
	
	
}

//returns the port specified in a parameter
int getPort(int argc, char* argv[]){
	if(argc==3){
		if(!strcmp(argv[1],"-p")){
			return atoi(argv[2]);
		}else{
			return 6677;
		}
	}else if (argc ==5){
		if(!strcmp(argv[1],"-p")){	
			return atoi(argv[2]);
		}else{	
			return atoi(argv[4]);			
		}
	}else{
		return 6677;					//port should be 6677 if no other value has been specified
	}	
}

//returns the root argument //TODO folder does not exist?
char* getRoot(int argc, char* argv[]){
	if(argc == 1){
		return NULL;	
	}else if(argc == 3){
		if(!strcmp(argv[1],"-r")){
			return argv[2];
		}else{
			return NULL;
		}
	}else if(argc ==5 ){
		if(!strcmp(argv[3],"-r")){
			return argv[4];
		}else{		
			if(!strcmp(argv[1],"-r")){
				return argv[2];
			}else{
				return NULL;
			}
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
					fprintf(stderr,"Error: 1st argument not -p or -r, but: %s \n",argv[i]);
					exit(400);		//TODO
				}
			}
		}
		if(i==3){
			if(!strcmp("-p",argv[i])){
				i++;
				continue;
			}else{
				if(!strcmp("-r",argv[i])){
					i++;
					continue;
				}else{
					fprintf(stderr,"Error: 3rd argument not -p or -r, but: %s \n",argv[i]);
					exit(400);		//TODO
				}
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
	string rootStr;
	if(root==NULL){
		rootStr="";
	}else{
		rootStr = root;
	}
	
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
	
	while(1){
		listen(sockfd,10);
		socklen_t clilen = sizeof(clientAddress);
		int sockfd2 = accept(sockfd, (struct sockaddr *) &clientAddress, &clilen);
		if(sockfd2 < 0){
			fprintf(stderr, "Error: Socket error,\n");
		} 
		
		
		//connection has been established
		getRequest(sockfd2,rootStr);
		close(sockfd2);
		
	}
	close(sockfd);
  
    return 200;		//TODO 
}