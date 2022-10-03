#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

void GetMessages(char* name);

void GetTramName(size_t size, char *name);

string ReadContents(char *name, size_t startIndex);

void TestPrint(string s)
{
    printf("%s", s.c_str());
    printf("\n\n");
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

    // Commented to use hard coding for the port #
    // I will explain the reasoning in interview
    // if(argc < 2){
    //     fprintf(stderr,"No port provided\n");
    //     exit(1);
    // }
    int sockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = 8081; // atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Socket failed \n");
    }

    server = gethostbyname("127.0.0.1");
    if (server == NULL)
    {
        error("No such host\n");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed\n");

    while (1)
    {
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("Error reading from Server");
        GetMessages(buffer);
    }

    return 0;
}

void GetMessages(char *name)
{
    vector<string> strings;
    size_t len = strlen(name);
    for (size_t i = 0; i < len; i++)
    {
        char c = name[i];
        if (!isalpha(name[i]) && !isdigit(name[i]) && (c != '_') && (c != ' '))
        {
            strings.push_back(ReadContents(name, i));
        } 
    }

    cout << strings.at(3)<<": "<<endl;
    cout << strings.at(1)<<": "<< strings.at(5)<<endl;
}

string ReadContents(char *name, size_t startIndex)
{
    size_t len = strlen(name);
    string s = string();
    for (size_t i = ++startIndex; i < len; i++)
    {
        char c = name[i];
        if (!isalpha(name[i]) && !isdigit(name[i]) && (c != '_') && (c != ' '))
             break;
        s += c;
    }
    //TestPrint(s);
    return s;
}
