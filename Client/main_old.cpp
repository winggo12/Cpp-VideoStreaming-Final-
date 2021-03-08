#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>

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
    

    if ((sokt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);


    int key;
    while(key != 'q'){
    
    if ((sokt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }

    std::cout << sokt << std::endl;

    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {

        std::cerr << "connect() failed!" << std::endl;

    }

    const unsigned int MAX_BUF_LENGTH = 4096;
    std::vector<char> buffer(MAX_BUF_LENGTH);
    std::string rcv;   
    int bytesReceived = 0;

	do {
	    bytesReceived = recv(sokt, &buffer[0], buffer.size(), 0);
	    // append string from buffer.
	    if ( bytesReceived == -1 ) { 
		// error 
	    } else {
		rcv.append( buffer.cbegin(), buffer.cend() );
	    }
	} while ( bytesReceived == MAX_BUF_LENGTH );
    
    std::cout << rcv << std::endl;
    close(sokt);

    }

    

    return 0;
}
