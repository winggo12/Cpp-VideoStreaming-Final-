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

    //----------------------------------------------------------
    //OpenCV Code
    //----------------------------------------------------------
    Mat img = Mat::zeros(480 , 640, CV_8UC3); ;
    // Remember that the order of w , h in here is inverted -->Server (w,h), Client (h,w)

    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    uchar sockData[imgSize];
    int bytes = 0;
    

        for (int i = 0; i < imgSize; i += bytes) {
            if ((bytes = recv(sokt, sockData +i, imgSize-i, 0)) == -1)  {
            std::cout << "recv failed" << std::endl ;
            exit(1);
            }
        }


	Mat img2(Size(640, 480), CV_8UC3, sockData);


    cv::imshow("CV Video Client", img2); 
    key = cv::waitKey(500);

	
       
    close(sokt);

    }

    

    return 0;
}
