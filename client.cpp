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


using namespace std;

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
				fprintf(stderr,"Wrong COMMAND argument %s \n",argv[i]);
				exit(400);		//TODO
			}
		}					
	}
}

int main(int argc, char* argv[]) {
	if(argc!=3 and argc!=4){
		fprintf(stderr,"Wrong number of arguments %d \n",argc);
		exit(400);		//TODO
	}
	validateArgs(argc,argv);
	string remotePath(argv[2]);	
	string localPath(getLocal(argc,argv));
	int clientSocket = getSocket();		//creates a socket
	struct sockaddr_in serverAddress;
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_port=htons(80);
	serverAddress.sin_addr;
	if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) != 0) {
		perror("ERROR: Connection has failed");
		exit(EXIT_FAILURE);
	}	
}