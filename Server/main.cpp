#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <condition_variable>
#include <arpa/inet.h>
#include <atomic>

std::vector<std::thread> th_list;

int connectionHandler(int socket)
{
    const unsigned int MAX_BUF_LENGTH = 4096;
    std::vector<char> buffer(MAX_BUF_LENGTH);
    std::string rcv;   
    int bytesReceived = 0;

    do {
        bytesReceived = recv(socket, &buffer[0], buffer.size(), 0);
        // append string from buffer.
        if ( bytesReceived == -1 ) { 
        // error 
        } else {
        rcv.append( buffer.cbegin(), buffer.cend() );
        }
    } while ( bytesReceived == MAX_BUF_LENGTH );
        
    std::cout << rcv << std::endl;
	return 0;
}

void print(int i)
{
	std::cout << i << std::endl;
}

int main(int argc, char** argv)
{
	// th_list.push_back(std::thread(readFrame));
	if (argc != 2) {
		std::cout << "Wrong number of input parameters" << std::endl;
		std::cerr << "Usage: server <serverPort> " << std::endl;
		return 0;
	}
	// std::cout << argc << std::endl;
	int socket_id, new_socket, c;
	struct sockaddr_in server, client;
	char *message;

	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id == -1)
	{
		std::cout << "Failed to create socket!" << std::endl;
		return -1;
	}
	std::cout << argv[0] << std::endl;
	std::cout << argv[1] << std::endl;
	std::cout << "Created socket!" << std::endl;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	if (bind(socket_id , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		std::cout << "Bind error" << std::endl;
		return 1;
	}
	std::cout << "Binded" << std::endl;

	listen(socket_id , 3);
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(socket_id, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		//Reply to the client
		th_list.push_back(std::thread(connectionHandler, new_socket));
		for(std::thread& th : th_list)
		{
			if(th.joinable())
				th.join();
		}
	}

	return 0;
}
