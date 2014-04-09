#ifndef __BRAIN_H
#define __BRAIN_H
#include <cvd/thread.h>
#include "DroneController.h"

#include "Map.h"
#include <string>

class Brain : protected CVD::Thread 
{
public:
  Brain();
  ~Brain();
  void setPosition(float x, float y, float z);
  static void GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams);
protected:
  virtual void run();
 
private:
  Msg_Sender* sender;
   
  // Map information:
  
  // Drone info
  // position
  float mPos_x, mPos_y, mPos_z;


  // keyboard state
  float i_pressed; // move forward
  float k_pressed; // move backward
  float j_pressed; // move left
  float l_pressed; // move right
  float w_pressed; // climb up
  float s_pressed; // climb down
  float a_pressed; // rotate cclockwise
  float d_pressed; // rotate clockwise

};

#endif
