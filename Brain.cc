#include "Brain.h"
#include <iostream>
#include <unistd.h>
#include "DroneController.h"

#include <gvars3/instances.h>
#include <gvars3/GStringUtil.h>

using namespace std;
using namespace CVD;
using namespace GVars3;

Brain::Brain()
{
  GUI.RegisterCommand("DroneControlKeyPress", GUICommandCallBack, this);
  GUI.RegisterCommand("DroneControlKeyRelease", GUICommandCallBack, this);
  sender = new Msg_Sender;

  i_pressed = 0.0;
  k_pressed = 0.0;
  j_pressed = 0.0;
  l_pressed = 0.0;
  w_pressed = 0.0;
  s_pressed = 0.0;
  a_pressed = 0.0;
  d_pressed = 0.0;

  start();
}

Brain::~Brain()
{
  stop();
  join();
  delete sender;
}

void Brain::setPosition(float x, float y, float z)
{
  mPos_x = x;
  mPos_y = y;
  mPos_z = z; 
}

void Brain::run()
{
  sender->initialize("192.168.1.1", 5556);
  Drone_Msg *takeoff_msg = new Drone_Takeoff_Msg(1);
  //sender->send_msg(takeoff_msg);
  //delete takeoff_msg;

  int i = 0;  
  bool bTakeOff = false;
  int take_off_i = 0;

  while(!shouldStop())
  {

     //cout << mPos_x << "," << mPos_y << "," << mPos_z << endl;

     if (mPos_x < -1.0) 
     {
	if (!bTakeOff) 
        {
	    sender->send_msg(takeoff_msg);
            delete takeoff_msg;
            take_off_i = i;
            bTakeOff = true;
        }
     }

     i++;
     Drone_Msg *msg;   
     msg = new Drone_Hold_Msg();
        
     if (i > take_off_i + 400) // 16s later
     {
         if (mPos_x < -0.01)
             msg = new Drone_Move_Msg(-0.1,0,0,0);
         if (mPos_x > 0.01)
             msg = new Drone_Move_Msg(0.1, 0, 0, 0);
     }
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

void Brain::GUICommandCallBack(void* ptr, string sCommand, string sParams)
{
    if (sCommand == "DroneControlKeyPress")
    {
        if (sParams == "i")
            cout << "i is pressed" << endl;
        if (sParams == "k")
            cout << "k is pressed" << endl;
        if (sParams == "j")
            cout << "j is pressed" << endl;
        if (sParams == "l")
            cout << "l is pressed" << endl;
        if (sParams == "w")
            cout << "w is pressed" << endl;
        if (sParams == "s")
            cout << "s is pressed" << endl;
        if (sParams == "a")
            cout << "a is pressed" << endl;
        if (sParams == "d")
            cout << "d is pressed" << endl;  
    }
   
    if (sCommand == "DroneControlKeyRelease")
    {
        if (sParams == "i")
            cout << "i is released" << endl;
        if (sParams == "k")
            cout << "k is released" << endl;
        if (sParams == "j")
            cout << "j is released" << endl;
        if (sParams == "l")
            cout << "l is released" << endl;
        if (sParams == "w")
            cout << "w is released" << endl;
        if (sParams == "s")
            cout << "s is released" << endl;
        if (sParams == "a")
            cout << "a is released" << endl;
        if (sParams == "d")
            cout << "d is released" << endl;
    }
}




