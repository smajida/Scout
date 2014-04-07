#include "Brain.h"
#include <iostream>
#include <unistd.h>
#include "DroneController.h"

using namespace std;

Brain::Brain()
{
  sender = new Msg_Sender;
  start();
}

Brain::~Brain()
{
  stop();
  join();
  delete sender;
}

void Brain::run()
{
  sender->initialize("192.168.1.1", 5556);
  Drone_Msg *takeoff_msg = new Drone_Takeoff_Msg(1);
  sender->send_msg(takeoff_msg);
  delete takeoff_msg;

  int i = 0;  
  while(!shouldStop())
  {
     i++;
     Drone_Msg *msg;   
     msg = new Drone_Hold_Msg();
        
     if (i > 250)
         msg = new Drone_Move_Msg(0.1,0,0,0);
     if (i > 325)
         msg = new Drone_Move_Msg(-0.1,0,0,0);
        //msg= new Drone_Takeoff_Msg(1);
     if (i > 400)
         msg = new Drone_Hold_Msg();

      sender->send_msg(msg);

     if (msg!=NULL) delete msg;

    //Drone_Msg *msg = new Drone_Takeoff_Msg(1);
    //sender->send_msg(takeoff_msg);
    usleep(40000);
    //Drone_Msg *msg = new Drone_Move_Msg(0, 0, 0, 0);
    //sender->send_msg(msg);
    //cout << "Send command here!" << endl;
    //delete msg;
  }
  

  Drone_Msg *land_msg = new Drone_Takeoff_Msg(0);
  cout << "Send Land Command" << endl;
  sender->send_msg(land_msg);
  delete land_msg;
}


