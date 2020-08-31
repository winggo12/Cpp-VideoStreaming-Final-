#include <iostream>
#include <unistd.h>
#include<string.h>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <condition_variable>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <atomic>

//std::atomic<bool> ready{};
//std::condition_variable var;
cv::Mat img;// = cv::Mat::zeros(640, 480, CV_8UC3);
std::mutex m1;
int connectionHandler(int socket)
{
	//std::unique_lock<std::mutex> lg(m1);
	//var.wait(lg, [=]{return ready.load();});
	std::lock_guard<std::mutex> lg(m1);
	//cv::Mat frame = cv::Mat::zeros(640, 480, CV_8UC3);
	//cv::resize(img,img,cv::Size(640,480));
	//u_char *data = img.data;
	img = (img.reshape(0,1));
	int imgSize = img.total() * img.elemSize();
	int bytes = 0;
	if ((bytes = send(socket, img.clone().data, imgSize, 0)) < 0)
	{
        return -1;
    }
	//var.notify_all();
	return 0;
}
void readFrame()
{
	cv::VideoCapture vc("../videos/48.mp4");
	while(vc.isOpened())
	{
		//std::unique_lock<std::mutex> ul(m1);
		{
			std::lock_guard<std::mutex> lg(m1);
			vc >> img;
			cv::resize(img,img,cv::Size(640,480));
			cv::imshow("window", img);
		}
		cv::waitKey(500);
		//ready.store(false);
		//var.wait(ul, [=]{return ready.load();});
	}
}

void print(int i)
{
	std::cout << i << std::endl;
}
std::vector<std::thread> th_list;

int main()
{
	th_list.push_back(std::thread(readFrame));

	int socket_id, new_socket, c;
	struct sockaddr_in server, client;
	char *message;

	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id == -1)
	{
		std::cout << "Failed to create socket!" << std::endl;
		return -1;
	}
	std::cout << "Created socket!" << std::endl;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
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
