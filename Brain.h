#ifndef __BRAIN_H
#define __BRAIN_H
#include <cvd/thread.h>
#include "DroneController.h"

#include "Map.h"

class Brain : protected CVD::Thread 
{
public:
  Brain();
  ~Brain();
  void setPosition(float x, float y, float z);

protected:
  virtual void run();

private:
  Msg_Sender* sender;
   
  // Map information:
  
  // Drone info
  // position
  float mPos_x, mPos_y, mPos_z;

};

#endif
