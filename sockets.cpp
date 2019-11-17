#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void send();
void receive();

int main()
{
    send();
    return 0;
}

void send()
{
    char input[1024];
    int sockfd;
    struct sockaddr_in serAddr;

    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(3514);
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout<<"\nWelcome to my chat application!"<<endl;
    cout<<"To stop chatting, type 'exit' "<<endl;
    cout<<"------------------------------"<<endl;
    thread th1(receive);
    while(1)
    {
        cout<<"->";
        fgets(input, 1024, stdin);
        strtok(input, "\n");
        if(strcmp(input, "exit")==0)
        {
            break;
        }
        if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <0)
        {
            cout<<"Error creating send socket\n";
        }
        if(sockfd<0)
        {
            cout<<"Error opening socket\n";
        }
        if(sendto(sockfd, input, strlen(input), 0, (struct sockaddr*)&serAddr, sizeof(serAddr)) < 0)
        {
            cout<<"Sending error \n";
            close(sockfd);
        }
        close(sockfd);
    }
    th1.join();
}

void receive()
{
    char buff[1024] = {0};
    int sockfd, readStatus;
    struct sockaddr_in serAddr;
    socklen_t Addrlen;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(3515);
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    Addrlen = sizeof(serAddr);

    while(1)
    {
        if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            cout<<"Error creating receive socket\n";
        }
        readStatus = recvfrom(sockfd, buff, 1024, 0, (struct sockaddr*)&serAddr, &Addrlen);
        if(readStatus < 0)
        {
            cout<<"Error receiving..\n";
        }
        cout<<"->"<<buff<<endl;
    }
}