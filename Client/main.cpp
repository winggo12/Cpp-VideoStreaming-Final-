#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>

std::mutex m1;
std::string msgs[] = {"0", "1, 0.1, 0.1, 0.9, 0.9", "2, 0.3, 0.3, 0.7, 0.5"};
std::vector<std::thread> th_list;

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

using namespace cv;

int main(int argc, char** argv)
{

    int         sokt;
    char*       serverIP;
    int         serverPort;

    
    if (argc < 3) {
           std::cerr << "Usage: client <serverIP> <serverPort> " << std::endl;
    }

    serverIP   = argv[1];
    serverPort = atoi(argv[2]);
   
    struct  sockaddr_in serverAddr;
    socklen_t           addrLen = sizeof(struct sockaddr_in);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);


    int key;
    while(key != 'q'){
    
        if ((sokt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "socket() failed" << std::endl;
        }

        std::cout << "socket: " << sokt << std::endl;

        if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
            std::cerr << "connect() failed!" << std::endl;
        }

        int bytes = 0;
        std::string msg = msgs[int(rand() % 3)];
        if ((bytes = send(sokt, msg.c_str(), msg.length(), 0)) < 0)
        {
            std::cout << "String was not sent!" << std::endl;
            return -1;
        }
        std::cout << "String sent: " << msg << std::endl;

        close(sokt);
    }

    for(std::thread& th : th_list)
	{
		if(th.joinable())
			th.join();
	}

    

    return 0;
}
