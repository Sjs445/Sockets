//Shane Spangenberg
//CPSC 351
//Professor Oates

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
        fgets(input, 1024, stdin);
       input[strcspn(input, "\n")]='\0';
        if(strcmp(input, "exit")==0)
        {
            break;
        }
        if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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
    cout<<"Goodbye!";
    th1.join();
}

void receive()
{
    char buff[1024];
    int sockfd, readStatus;
    struct sockaddr_in serAddr;
    socklen_t Addrlen;
    
    while(1)
    {
        if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            cout<<"Error creating receive socket\n";
        }

        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(3515);
        serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        Addrlen = (socklen_t)sizeof(serAddr);

        if(bind(sockfd, (const struct sockaddr *)&serAddr, Addrlen) < 0)
        {
            perror("bind failed"); 
            exit(EXIT_FAILURE); 
        }

        if((readStatus=recvfrom(sockfd, buff, 1024, 0, (struct sockaddr*)&serAddr, &Addrlen)) < 0)
        {
            cout<<"Error receiving..\n";
        }
        buff[readStatus]='\0';
        cout<<buff<<endl;

        close(sockfd);
    }
}