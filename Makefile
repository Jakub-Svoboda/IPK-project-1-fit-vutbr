servermake: client.cpp 
	g++ -Wall -std=c++11 -o ftrest client.cpp 
	g++ -Wall -std=c++11 -o ftrestd server.cpp