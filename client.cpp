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
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <regex>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <vector>


#define BUFFER_SIZE 1024
using namespace std;

string getType(int argc, char *argv[]){
	string response;
	string message = argv[1];
	if(message[0] == 'd'){	
		return response="?type=file";			
	}else if (message[0] == 'g'){	
		return response="?type=file";
	}else if (message[0] == 'p'){
		return response="?type=file";
	}else if (message[0] == 'l'){	
		return response="?type=folder";
	}else if (message[0] == 'm'){
		return response="?type=folder";
	}else if (message[0] == 'r'){
		return response="?type=folder";
	}else{	
		exit(1);	//Should not happen, commands are checked before on clients side.
	}	
	return response;
}

//parses the content length from the HTTP header
int parseLength(char buf[BUFFER_SIZE]){
	string message = buf;
	for (int i =0; i< 3; i++){
		message.erase(0, message.find("\n") + 1);
	}
	size_t pos = message.find_first_of(":");
	size_t pos2= message.find_first_of('\n');
	message = message.substr(pos+2,pos2);
	return atoi(message.c_str());
}

//calculates the http header length
int parseHeaderLength(char buf[BUFFER_SIZE]){
	string message = buf;
	int length=0;
	for (int i =0; i< 5; i++){
		length+=message.find("\n")+1;
		message.erase(0, message.find("\n") + 1);
	}
	return length;
}

//checks if file exists
bool fileExists(string &str){
	ifstream file(str.c_str());
    return file.good();
}

//returns true if path leads to a directory
bool isItDir(string path){
	struct stat s;
	if( stat(path.c_str(),&s) == 0 ){
		if( s.st_mode & S_IFDIR ){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

//returns true if path leads to a file
bool isItFile(string path){
	struct stat buf;
    stat(path.c_str(), &buf);
    return S_ISREG(buf.st_mode);
}

//reads from file to string
string getContent(string localPath){
	if(localPath[localPath.length()-1 ]== '/'){
		localPath=localPath.substr(0, localPath.length()-1);
	}
	if(localPath.length() > 0 and isItFile(localPath)){	
		std::ifstream ifs(localPath);
		string content( (std::istreambuf_iterator<char>(ifs) ),(std::istreambuf_iterator<char>()    ) );
		return content;
	}else{
		return "";
	}
}

//prints an error to stderr when a non 200 code is received
void printPotentialStderr(string outStr,int clientSocket){
	size_t pos = outStr.find_first_of("\n");
	if(outStr[9] !='2' or outStr[10] !='0' or outStr[11] !='0' ){			
		outStr=outStr.substr(0,pos);
		outStr=outStr.substr(13,outStr.length());
		fprintf(stderr,"%s\n",outStr.c_str());
		close(clientSocket);
//cerr<<"foo this is client, closed now from printPotentialStderr"<<endl<<endl;	
		exit(1);	//todo, if not exited here, file will be created regardless if get was the command
	}
}

//reads from socket and formats the answer message
int readSocket(int clientSocket,char* argv[], int argc, string remotePath, string localPath){
	std::vector<char> outStr2;
	char buf[BUFFER_SIZE];

	int numread;
	int length=0;
	int headerLength=1;
	int numReadTotal=0;
	numread=0;
	//cicles while reading from socket		
	while(numReadTotal<(length+headerLength)){		
		if ((numread= read(clientSocket, buf, sizeof(buf)-1)) == -1){
			fprintf(stderr,"Error: reading from socket");		//TODO exit
			exit(1);
		}	
		if(numReadTotal == 0){			
			length = parseLength(buf);							//get the content lenght from the header	
			headerLength=parseHeaderLength(buf);
		}		
		numReadTotal+=numread;
		for(int i = 0; i< numread; i++){
			outStr2.push_back(buf[i]);
		}
	}
	string outStr(outStr2.begin(), outStr2.end());	
	if(!strcmp("get",argv[1])){		//we got back a file, create a new file on clients side a write to it
		string fileName= argv[2];
		size_t pos = fileName.find_last_of("/");	
		fileName = fileName.substr(pos+1, fileName.length());		
		if(localPath.length()==0 ){
			localPath+=fileName;	
		}else{
			if(localPath[localPath.length()-1]=='/'){		//remove the /, we are deling with a file
				localPath=localPath.substr(0,localPath.length()-1);
			}
		}
		for(int i = 0; i<5; i++){		//delete first 5 lines = header
			if(i==0){
				printPotentialStderr(outStr,clientSocket);
			}
			outStr.erase(0, outStr.find("\n") + 1);
		}				
		std::ofstream outfile (localPath);
		outfile << outStr ;
		outfile.close();	
	}else if (!strcmp("put",argv[1])){
		printPotentialStderr(outStr,clientSocket);
	}else if (!strcmp("del",argv[1])){
		printPotentialStderr(outStr,clientSocket);	
	}else if (!strcmp("lst",argv[1])){
		printPotentialStderr(outStr,clientSocket);
		for(int i = 0; i < 5; i++){
			outStr.erase(0, outStr.find("\n") + 1);
		}
		fprintf(stdout,"%s",outStr.c_str());			//lst output print
	}else if (!strcmp("mkd",argv[1])){
		printPotentialStderr(outStr,clientSocket);
	}else if (!strcmp("rmd",argv[1])){
		printPotentialStderr(outStr,clientSocket);
	}	
			
	return 0;		//TODO?
}

//Sends a message and checks if writing to socket was ok.
void send(string message,int clientSocket){
//cerr<<message;//TODO deleteme	
	int n = write(clientSocket,message.c_str(),message.length());
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
	string local="";
	if(argc == 3){
		return local;	
	}else{
		local+=argv[3];
		if(local[local.length()-1] != '/'){		
			local+="/";
		}
		if(!strcmp(argv[1],"put")){
			string a = argv[3];
			if(isItFile(a)){
				return local;
			}else{
				fprintf(stderr,"Error: local file not found. %s\n",local.c_str());
				exit(1);
			}
		}
		return local;
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
				if(argc != 4){
					fprintf(stderr,"Unknown error.\n");
					exit(1); 
				}
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
				exit(1);		//todo?
			}
		}					
	}
}

int main(int argc, char* argv[]) {
	
	if(argc!=3 and argc!=4){
		fprintf(stderr,"Error: Wrong number of arguments %d \n",argc);
		exit(1);		
	}
	validateArgs(argc,argv);
	string remotePath(argv[2]);	
	string localPath(getLocal(argc,argv));
	int port = getPort(remotePath);
	
	int clientSocket = getSocket();								//creates a socket
	struct hostent *server;										//describes host
	server = gethostbyname("localhost");						//get the IP
    if (server == NULL) {
        fprintf(stderr,"Error: no such host\n");
        exit(1);
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
	size_t pos = remotePath.find_first_of(to_string(port));														//cut the argument behind the port number
	char buf[1000];
	time_t t = time(0);
	struct tm time = *gmtime(&t);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &time);		//format for Date
	int length=0;
	while ( port /= 10 ){	//decadic length of port integer
		length++;
	}
	string content="";
	if(!strcmp(argv[1],"put")){
		if(localPath[localPath.length()-1 ]== '/'){
			localPath=localPath.substr(0, localPath.length()-1);
			if(!isItFile(localPath)){
				cerr<<"Error: local file not found."<<endl;
				exit(1);
			}
		}
		if(fileExists(localPath)){
			content=getContent(localPath);		
		}else{
			content="";			
		}	
	}	
	string justPath = remotePath.substr(pos+1+length,remotePath.length());	
	string type=getType(argc,argv);
	
	message+= " ";
	message+=justPath;	
	message+=type;
	message+=" HTTP/1.1";
	message+="\n";
	message+="Date: ";
	message+=buf;
	message+="\n";
	message+="Accept: text/plain\n";
	message+="Accept-Encoding: gzip, deflate\n";
	message+="Content-Type: text/plain\n";
	message+="Content-Length: ";
	message+=to_string(content.length());
	message+="\n";
	message+="\r\n";
	message+=content;	

	send(message,clientSocket);	
	readSocket(clientSocket,argv, argc, remotePath, localPath);
	close(clientSocket);
//cerr<<"foo this is client, closed now"<<endl<<endl;
}













