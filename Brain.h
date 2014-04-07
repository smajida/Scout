#ifndef __BRAIN_H
#define __BRAIN_H
#include <cvd/thread.h>
#include "DroneController.h"

class Brain : protected CVD::Thread 
{
public:
  Brain();
  ~Brain();

protected:
  virtual void run();

private:
  Msg_Sender* sender;
};

#endif
