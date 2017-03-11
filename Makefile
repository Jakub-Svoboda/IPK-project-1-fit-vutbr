servermake: client.cpp 
	g++ -Wall -std=c++1y -o ftrest client.cpp 
	g++ -Wall -std=c++1y -o ftrestd server.cpp