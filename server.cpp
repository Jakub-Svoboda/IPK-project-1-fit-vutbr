/**
*IPK: project 1
*Author: Jakub Svoboda
*Email: xsvobo0z@stud.fit.vutbr.cz
*Date: 8. March 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		return 0;					//TODO?
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
		fprintf(stderr,"Wrong number of arguments %d \n",argc);
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
					fprintf(stderr,"3rd argument not -p, but: %s \n",argv[i]);
					exit(400);		//TODO
				}
			}
		}
		if(i==3){
			if(!strcmp("-p",argv[i])){
				i++;
				continue;
			}else{
				fprintf(stderr,"3rd argument not -p, but: %s \n",argv[i]);
				exit(400);		//TODO
			}
		}		
	}
}

int main(int argc, char* argv[]) {
	if(argc>5){
		fprintf(stderr,"Wrong number of arguments %d \n",argc);
		exit(400);		//TODO
	}
	validateArgs(argc,argv);
	char* root = getRoot(argc,argv);
	int port = getPort(argc,argv);
	printf("root is %s, port is %d\n",root,port);
}