#include "EDControlManager.h"



void EDControlManager::edcmGLDisplayStatic(const EDControlManager& p) {
	for (int i = 0; i < p.vControlList.size(); i++) {
		p.vControlList[i]->edDraw();
	}
}


void EDControlManager::edcmGLDisplay() {
	edcmGLDisplayStatic(*this);
}

void EDControlManager::edcmAddControl(EDInteractiveControl* p) {
	vControlList.push_back(p);
}
void EDControlManager::edcmUpdate() {
	for (int i = 0; i < vControlList.size(); i++) {
		vControlList[i]->ediUpdate();
	}
}
void EDControlManager::edcmDeploy() {
	for (int i = 0; i < vControlList.size(); i++) {
		vControlList[i]->ediApplyDraw();
	}
}

void EDControlManager::edcmActivateFrameUpdateEvent() {
	for (int i = 0; i < vControlList.size(); i++) {
		vControlList[i]->edReceiveEventCall(EDCM_FRAME_UPDATE);
	}
}

void EDControlManager::edcmActivateMouseMoveEvent(int x, int y) {
	for (int i = 0; i < vControlList.size(); i++) {
		if (x >= vControlList[i]->vLeftTopCoord.x && x <= vControlList[i]->vLeftTopCoord.x + vControlList[i]->iWidth) {
			if (y >= vControlList[i]->vLeftTopCoord.y && y <= vControlList[i]->vLeftTopCoord.y + vControlList[i]->iHeight) {
				vControlList[i]->edReceiveEventCall(EDCM_MOUSE_HOVER, (GLdouble)x, (GLdouble)y);
			}
			else {
				vControlList[i]->edReceiveEventCall(EDCM_MOUSE_HOVER_OUT, (GLdouble)x, (GLdouble)y);
			}
		}
		else {
			vControlList[i]->edReceiveEventCall(EDCM_MOUSE_HOVER_OUT, (GLdouble)x, (GLdouble)y);
		}
	}
}
void EDControlManager::edcmActivateMouseClickEvent(int x, int y) {
	for (int i = 0; i < vControlList.size(); i++) {
		if (x >= vControlList[i]->vLeftTopCoord.x && x <= vControlList[i]->vLeftTopCoord.x + vControlList[i]->iWidth) {
			if (y >= vControlList[i]->vLeftTopCoord.y && y <= vControlList[i]->vLeftTopCoord.y + vControlList[i]->iHeight) {
				vControlList[i]->edReceiveEventCall(EDCM_MOUSE_CLICK, (GLdouble)x, (GLdouble)y);
				std::stringstream oss;
				oss << "Mouse Click Event Activated  - MouseArgv: " << x << "," << y<<" ControlPtr:"<<std::hex<<(long long)vControlList[i]<<std::dec;
				//EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edINFO, oss.str());

			}

		}
	}
}