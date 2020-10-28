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

std::mutex m1;
std::string msgs[] = {"0", "1, 0.1, 0.1, 0.9, 0.9", "2, 0.3, 0.3, 0.7, 0.5"};
int connectionHandler(int socket)
{
	std::lock_guard<std::mutex> lg(m1);

	int bytes = 0;
	std::string msg = msgs[int(rand() % 3)];
	if ((bytes = send(socket, msg.c_str(), msg.length(), 0)) < 0)
	{
		std::cout << "String was not sent!" << std::endl;
        return -1;
    }
	std::cout << "String sent: " << msg << std::endl;
	if (int closeStat = close(socket) < 0) {
		return -1;
	}
	return 0;
}

void print(int i)
{
	std::cout << i << std::endl;
}
std::vector<std::thread> th_list;

int main(int argc, char** argv)
{
	// th_list.push_back(std::thread(readFrame));
	if (argc != 2) {
		std::cout << "Wrong number of input parameters" << std::endl;
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
	}
	

	for(std::thread& th : th_list)
	{
		if(th.joinable())
			th.join();
	}

	return 0;
}
