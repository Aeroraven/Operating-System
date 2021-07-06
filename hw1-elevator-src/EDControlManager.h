#pragma once
#include"EDInteractiveControl.h"
#include"EDShapeBase.h"
#include "EDLog.h"
#include <sstream>
// Elevator Dispatch - Control Manager

// The instance should be only exemplified once

class EDControlManager{
public:
	static const int EDCM_FRAME_UPDATE = 0;
	static const int EDCM_MOUSE_CLICK = 1;
	static const int EDCM_MOUSE_HOVER = 2;
	static const int EDCM_MOUSE_HOVER_OUT = 3;

	std::vector<EDInteractiveControl*> vControlList;

	void edcmAddControl(EDInteractiveControl*);
	void edcmGLDisplay();

	static void edcmGLDisplayStatic(const EDControlManager&);
	
	void edcmActivateMouseMoveEvent(int x, int y);
	void edcmActivateMouseClickEvent(int x, int y);
	void edcmActivateFrameUpdateEvent();

	void edcmDeploy();
	void edcmUpdate();
};

