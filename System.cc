// Copyright 2008 Isis Innovation Limited
#include "System.h"
#include "OpenGL.h"
#include <gvars3/instances.h>
#include <stdlib.h>
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include "ARDriver.h"
#include "MapViewer.h"
#include "Brain.h"
#include <iostream>

using namespace CVD;
using namespace std;
using namespace GVars3;


System::System()
  : mGLWindow(mVideoSource.Size(), "PTAM")
{
  GUI.RegisterCommand("exit", GUICommandCallBack, this);
  GUI.RegisterCommand("quit", GUICommandCallBack, this);
  
  mimFrameBW.resize(mVideoSource.Size());
  mimFrameRGB.resize(mVideoSource.Size());
  // First, check if the camera is calibrated.
  // If not, we need to run the calibration widget.
  Vector<NUMTRACKERCAMPARAMETERS> vTest;
  
  vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
  mpCamera = new ATANCamera("Camera");
  Vector<2> v2;
  if(v2==v2) ;
  if(vTest == ATANCamera::mvDefaultParams)
    {
      cout << endl;
      cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << endl;
      cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << endl;
      exit(1);
    }
  
  mpMap = new Map;
  mpMapMaker = new MapMaker(*mpMap, *mpCamera);
  mpTracker = new Tracker(mVideoSource.Size(), *mpCamera, *mpMap, *mpMapMaker);
  mpARDriver = new ARDriver(*mpCamera, mVideoSource.Size(), mGLWindow);
  mpMapViewer = new MapViewer(*mpMap, mGLWindow);
  mpBrain = new Brain;


  GUI.ParseLine("GLWindow.AddMenu Menu Menu");
  GUI.ParseLine("Menu.ShowMenu Root");
  GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
  GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
  GUI.ParseLine("DrawAR=0");
  GUI.ParseLine("DrawMap=0");
  GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
  GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");
  
  mbDone = false;
};

void System::Run()
{
  while(!mbDone) // this is the condition flag of the whole main loop
    {
      
      // We use two versions of each video frame:
      cout << "System runing, try to grab a new frame. " << endl;
      // Grab new video frame...
      mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW, mimFrameRGB);  
      static bool bFirstFrame = true;
      if(bFirstFrame)
	{
      mpARDriver->Init();//useless code
	  bFirstFrame = false;
	}
      
      mGLWindow.SetupViewport();
      mGLWindow.SetupVideoOrtho();
      mGLWindow.SetupVideoRasterPosAndZoom();
      
      if(!mpMap->IsGood())
	mpARDriver->Reset();
      
      static gvar3<int> gvnDrawMap("DrawMap", 0, HIDDEN|SILENT);
      static gvar3<int> gvnDrawAR("DrawAR", 0, HIDDEN|SILENT);
      
      bool bDrawMap = mpMap->IsGood() && *gvnDrawMap;
      bool bDrawAR = mpMap->IsGood() && *gvnDrawAR;
      
      mpTracker->TrackFrame(mimFrameBW, !bDrawAR && !bDrawMap);
      
      if(bDrawMap)
	mpMapViewer->DrawMap(mpTracker->GetCurrentPose());
      else if(bDrawAR)
	mpARDriver->Render(mimFrameRGB, mpTracker->GetCurrentPose());

      // Pass latest position to Brain
      SE3<> se3CamFromWorld = mpTracker->GetCurrentPose();
      Vector<3> position = se3CamFromWorld.inverse().get_translation();       
      mpBrain->setPosition(position[0], position[1], position[2]);
      

      //      mGLWindow.GetMousePoseUpdate();
      string sCaption;
      if(bDrawMap)
	sCaption = mpMapViewer->GetMessageForUser();
      else
	sCaption = mpTracker->GetMessageForUser();
      mGLWindow.DrawCaption(sCaption);
      mGLWindow.DrawMenus();
      mGLWindow.swap_buffers();
      mGLWindow.HandlePendingEvents();
    }

    delete mpBrain;
}

void System::GUICommandCallBack(void *ptr, string sCommand, string sParams)
{
  if(sCommand=="quit" || sCommand == "exit")
    static_cast<System*>(ptr)->mbDone = true;
}








