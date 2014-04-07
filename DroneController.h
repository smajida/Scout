#ifndef DRONE_CONTROLLER_H
#define DRONE_CONTROLLER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.1"
#define SERVER_PORT 5556
#define BUFLEN 255

class Drone_Msg;

class Msg_Sender{
private:
	int port;	
    char ip[BUFLEN];
    int sockfd, n;
    struct sockaddr_in serv_addr;
	int slen;	
	char buf[BUFLEN];

public:
	//initialize socket stuff
	void initialize(char * _ip, int _port);
	
	void send_msg(Drone_Msg*);

};

class Drone_Msg{
protected: 
	char message[BUFLEN];
public:
	char* get_msg(){return message;}
};

class Drone_Takeoff_Msg:public Drone_Msg{
public:
	Drone_Takeoff_Msg( int on);
	void set_msg(int on);
};

class Drone_Hold_Msg : public Drone_Msg
{
public:
    Drone_Hold_Msg();
    void set_msg();
};

class Drone_Move_Msg:public Drone_Msg{
public:
	Drone_Move_Msg(float lr,float fb, float alt, float rot);
	void set_msg( float lr,float fb, float alt, float rot);
};

#endif
