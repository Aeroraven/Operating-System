#pragma once

#include <sstream>
#include <thread>
#include "EDControlManager.h"
#include "EDTimer.h"
#include "EDElevator.h"

//Elevator Dispatch - Main Procedure

//Only once instance can be instantiated

class EDMain {
public:
	static const int EDCM_GLUTPOS_LEFT = 0;
	static const int EDCM_GLUTPOS_TOP = 0;
	static const int EDCM_GLUTWINDOW_INIT_W = 1440;
	static const int EDCM_GLUTWINDOW_INIT_H = 1000;
	static const float EDCM_GLUTORTHO_H;
	static const float EDCM_GLUTORTHO_W;

	static int EDCM_GLUTANI_INTERVAL; //frame rate

	static const char* EDCM_TITLE;

	static const int ED_MAIN_ELEVATORS = 6; //constant for GUI(to draw elevators backgrounds)
	static const int ED_MAIN_VALID_ELEVATORS = 5; //total elevators
	static const int ED_MAIN_ELEVATOR_PANEL_W = 425; //constant for GUI
	static const int ED_MAIN_ELEVATOR_PANEL_H = 250; //constant for GUI

	static const int ED_MAIN_ELEVATOR_PANEL_X[]; //constant for GUI
	static const int ED_MAIN_ELEVATOR_PANEL_Y[]; //constant for GUI

	static const int ED_MAIN_ELEVATOR_FLOOR_PANEL_X = 50; //constant for GUI
	static const int ED_MAIN_ELEVATOR_FLOOR_PANEL_Y = 50; //constant for GUI
	static const int ED_MAIN_ELEVATOR_FLOOR_PANEL_W = 190; //constant for GUI
	static const int ED_MAIN_ELEVATOR_FLOOR_PANEL_H = 850; //constant for GUI

	static const int ED_MAIN_FLOORS = 20; //total floors
	static const int ED_MAIN_ELEVATORS_FLOOR_UPBTN_X = 140; //constant for GUI
	static const int ED_MAIN_ELEVATORS_FLOOR_DWBTN_X = 190; //constant for GUI
	static const int ED_MAIN_ELEVATORS_FLOOR_LABEL_X = 70; //constant for GUI

	//Control Manager
	static EDControlManager* edcm;

	//Timer Controller
	static EDTimerController* edtc;
	static int* edtcArgvBuffer;

	//External Request Array
	static EDElevatorRequest* ederList;

	//Multi-threading
	static std::thread* edmtUIThread; //GUI thread
	static std::thread* edmtDispatchMainThread;
	static EDTimer* edmtExternalDispatchThread; //external dispatcher thread
	static std::thread* edmtCONTestThread; 
	static EDTimer** edmtElevatorThreadPtr; //elevator internal dispatcher thread


	//OGL UI
	static int edmgUIInitStatus; //indicates if UI is initialized
	static int edmgUICurrentWindowW; //window width
	static int edmgUICurrentWindowH; //window height

	static void edmGLDisplay();
	static void edmGLInit(int* argc, char** argv);
	static void edmGLInitDisplay();
	static void edmGLStart(int* argc, char** argv);
	//Handler for recording mouse click
	static void edmGLMouseKeyHandler(int button, int state, int x, int y);

	//Handler for recording mouse motion
	static void edmGLMouseMotionHandler(int x, int y);

	//Frame updates 
	static void edmGLFrameUpdate(int);

	//Deploy interactive controls in the window
	static void edmDeployControls();

	//Tackling window resizing requests
	static void edmGLResizeHandler(int w, int h);

	//Enter low quality mode
	static void edmLowQualityMode();

	//Synchronization of user interface attributes
	static void edmSynchronizeUIAttrib();

	//Handler for clicking buttons
	static int edmCarBtnClickHandler(void* object, GLfloat x, GLfloat y);

	//Dispatch Core
	static void edmDispatchStart();

	//[IMPORTANT] external dispatch
	static void edmDispatchExternal(int x,const std::vector<void*>& argtmp);

	//Debug
	static void edmDebug();

	//Entry
	static void edmStart(int* argc, char** argv);


	static EDElevator* edceElevator;

	//Controls

	//background panels
	static EDInteractiveControl_PanelBack* edcpElevatorPanelBack;
	//background panels - border shadow
	static EDInteractiveControl_PanelBack* edcpElevatorPanelBackShadow;
	//background panels
	static EDInteractiveControl_PanelBack* edcpElevatorFloorPanelBack;
	//background panels - border shadow
	static EDInteractiveControl_PanelBack* edcpElevatorFloorPanelBackShadow;

	//ascending button
	static EDInteractiveControl_RoundButton* edcpElevatorFloorPanelUpBtn;
	//descending button
	static EDInteractiveControl_RoundButton* edcpElevatorFloorPanelDwBtn;
	//floor labels
	static EDInteractiveControl_Label* edcpElevatorFloorPanelLabel;
	//floor buttons in the car
	static EDInteractiveControl_RoundButton** edcpElevatorCarPanelNormalBtn;
	//floor LED
	static EDInteractiveControl_Label* edcpElevatorCarPanelLED;
	//florr status label
	static EDInteractiveControl_Label* edcpElevatorCarPanelStatusBar;
	//ascending indicator
	static EDInteractiveControl_ArrowIndicator* edcpElevatorCarUpIndicator;
	//descending indicator
	static EDInteractiveControl_ArrowIndicator* edcpElevatorCarDwIndicator;

	//alarm button
	static EDInteractiveControl_RoundButton* edcpElevatorCarPanelAlarmBtn;
	//open button
	static EDInteractiveControl_RoundButton* edcpElevatorCarPanelOpenBtn;
	//close button
	static EDInteractiveControl_RoundButton* edcpElevatorCarPanelCloseBtn;

	//title
	static EDInteractiveControl_Label* edcpTitle;
	//author - 1950641 hzw
	static EDInteractiveControl_Label* edcpAuthor;
	//elevator label
	static EDInteractiveControl_Label* edcpElevatorLabels;
	//time left indicator
	static EDInteractiveControl_CircularProgressBar* edcpTimeLeftIndicator;
	static EDInteractiveControl_Button* edcpLowQualityMode;
	EDMain();
};

