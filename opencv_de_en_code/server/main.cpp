#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

#include "webcam.hpp"

// Server side
int main()
{
    // for the server, we only need to specify a port number

    // grab the port number
    int port = atoi("8080");
    // buffer to send and receive messages with
    char msg[1500];

    // setup a socket and connection tools
    sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // open stream oriented socket with internet address
    // also keep track of the socket descriptor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Error establishing the server socket" << std::endl;
        exit(0);
    }
    // bind the socket to its local address
    int bind_status = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (bind_status < 0)
    {
        std::cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    std::cout << "Waiting for a client to connect..." << std::endl;
    // listen for up to 5 requests at a time
    listen(server_socket, 5);
    // receive a request from client using accept
    // we need a new address to connect with the client
    sockaddr_in new_socket_addr;
    socklen_t new_socket_addr_size = sizeof(new_socket_addr);
    // accept, create a new socket descriptor to
    // handle the new connection with client
    int new_socket = accept(server_socket, (sockaddr *)&new_socket_addr, &new_socket_addr_size);
    if (new_socket < 0)
    {
        std::cerr << "Error accepting request from client!" << std::endl;
        exit(1);
    }
    std::cout << "Connected with client!" << std::endl;

    // also keep track of the amount of data sent as well
    int bytes_read, bytes_written = 0;

    // WORK FROM HERE ON RECEIVING FEED
    Webcam webcam;
    webcam.create_window();
    
    while (true)
    {
        int len = 0;
        std::vector<char> buffer;
        size_t buffer_size = 0;

        while (buffer_size == 0)
        {
            recv(new_socket, (char *)(&buffer_size), sizeof(size_t), 0);
        }
        
        size_t remaining = buffer_size;
        unsigned char data[buffer_size];

        len = recv(new_socket, data, remaining, MSG_WAITALL);
        if (len <= 0)
        {
            continue;
        }
        
        buffer.assign(data, data + remaining);

        webcam.m_frames = cv::imdecode(buffer, cv::IMREAD_COLOR);

        webcam.show(webcam.m_frames);

        if(cv::waitKey(25) == 'q') {
            break;
        }
    }

    close(new_socket);
    close(server_socket);
    std::cout << "********Session********" << std::endl;
    std::cout << "Bytes written: " << bytes_written << " Bytes read: " << bytes_read << std::endl;
    std::cout << "Connection closed..." << std::endl;
    return 0;
}

// receive a message from the client (listen)
//     std::cout << "Awaiting client response..." << std::endl;
//     memset(&msg, 0, sizeof(msg));//clear the buffer
//     bytes_read += recv(new_socket, (char*)&msg, sizeof(msg), 0);
//     if(!strcmp(msg, "exit"))
//     {
//         std::cout << "Client has quit the session" << std::endl;
//         break;
//     }
//     std::cout << "Client: " << msg << std::endl;
//     std::cout << ">";
//     std::string data;
//     getline(std::cin, data);
//     memset(&msg, 0, sizeof(msg)); //clear the buffer
//     strcpy(msg, data.c_str());
//     if(data == "exit")
//     {
//         //send to the client that server has closed the connection
//         send(new_socket, (char*)&msg, strlen(msg), 0);
//         break;
//     }
//     //send the message to client
//     bytes_written += send(new_socket, (char*)&msg, strlen(msg), 0);
// }