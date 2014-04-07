#include "DroneController.h"

void diep(char *s){
    perror(s);
    exit(1);
}

int seq=1;

int get_seq(){
	return seq++;
}

int FloatToInt(float f){
	float *p = &f;
	int* a = (int*)p;
	return *a;
}


void Drone_Takeoff_Msg::set_msg(int on){
	memset(message,0,BUFLEN);
	sprintf(message,"AT*REF=%d,%d\r",get_seq(),((1<<18)+(1<<20)+(1<<22)+(1<<24)+(1<<28)+(1<<9)*on));
}

void Drone_Move_Msg::set_msg(float lr,float fb, float alt, float rot){
	memset(message,0,BUFLEN);
	sprintf(message,"AT*PCMD=%d,%d,%d,%d,%d,%d\r",get_seq(),1, FloatToInt(-lr), FloatToInt(-fb), FloatToInt(alt), FloatToInt(-rot));
}

void Drone_Hold_Msg::set_msg(){
	memset(message,0,BUFLEN);
	//sprintf(message,"AT*PCMD=%d,%d,%d,%d,%d,%d\r",get_seq(),1, FloatToInt(-lr), FloatToInt(-fb), FloatToInt(alt), FloatToInt(-rot));
    sprintf(message,"AT*PCMD=%d,%d,%d,%d,%d,%d\r",get_seq(),0, FloatToInt(0), FloatToInt(0), FloatToInt(0), FloatToInt(0));
}

Drone_Takeoff_Msg::Drone_Takeoff_Msg(int on){
	set_msg(on);
}

Drone_Move_Msg::Drone_Move_Msg(float lr,float fb, float alt, float rot){
	set_msg(lr, fb,  alt, rot);
}

Drone_Hold_Msg::Drone_Hold_Msg()
{
    set_msg();
}



void Msg_Sender::initialize(char * _ip, int _port){
	this->port=_port;
	strcpy(this->ip,_ip);
    sockfd = 0;
    n = 0;
    slen = sizeof(serv_addr);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        diep("\n Error: Could not create socket\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
        diep("\n inet_pton error occured\n");

}

void Msg_Sender::send_msg(Drone_Msg* message){
    char* msg = message->get_msg();
	memset(buf, '0',sizeof(buf));
	//process msg
    sprintf(buf, msg);
	printf("Sending msg: %s\n",buf);
    if (sendto(sockfd, buf, BUFLEN, 0,(struct sockaddr *) &serv_addr, slen)==-1){
        diep("sendto()");
        
    }
}




