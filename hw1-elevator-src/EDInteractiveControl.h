#pragma once
#include <string>
#include <stdarg.h>
#include "EDShapeBase.h"
#include "EDAnimation.h"
class EDInteractiveControl {
public:
	int iStandardZIndex = 0;
	int iActiveDisplayPage;
	std::vector<EDShape*> vDisplayPage;
	EDVector3D vLeftTopCoord;
	GLfloat iWidth;
	GLfloat iHeight;
	int iMouseHoverStat = 0;
	int iTag1 = 0, iTag2 = 0, iTag3 = 0;

	bool bMouseInRangeFlag = 0;

	int (*hOnLoadHandler)(EDInteractiveControl&) = nullptr;
	int (*hOnUnloadHandler) (EDInteractiveControl&) = nullptr;
	int (*hOnMouseClickHandler) (EDInteractiveControl&, GLfloat, GLfloat) = nullptr;

	int (*hOnMouseHoverHandler) (EDInteractiveControl&, GLfloat, GLfloat) = nullptr;
	int (*hOnMouseHoverOutHandler) (EDInteractiveControl&, GLfloat, GLfloat) = nullptr;
	int (*hOnMouseDownHandler)(EDInteractiveControl&, GLfloat, GLfloat) = nullptr;
	int (*hOnFrameUpdateHandler)(EDInteractiveControl&) = nullptr;

	int (*hUsrOnMouseClickHandler) (void*, GLfloat, GLfloat) = nullptr;
	int (*hUsrOnMouseHoverHandler)(GLfloat, GLfloat) = nullptr;
	int (*hUsrOnMouseHoverOutHandler) (GLfloat, GLfloat) = nullptr;

	void edReceiveEventCall(int, ...);
	void edDraw();
	void edToggleDisplayPage(int);
	void edInsertDisplayPage();
	void edInsertShape(int, EDShape*);
	void edInsertShapeCom(int, EDShapeBase*);
	void edQuickSetup(float lx, float ly, float w, float h);
	void edSetZIndex(const int& z) { iStandardZIndex = z; }
	void edDemolish();

	virtual void ediCustomFunc() = 0;
	virtual void ediApplyDraw() = 0;
	virtual void ediUpdate() = 0;

	virtual void ediHoverAniController(int,...) = 0;

	~EDInteractiveControl();
};

class EDInteractiveControl_Button :public EDInteractiveControl {
public:
	static const int EDIBTN_HOVER_FRAMES = 15;

	std::string sCaption = "X";
	EDVector3D vForeColor;
	EDVector3D vBackColor;
	int iHoverAniFrame = 0;
	int iFrameCounter = 0;

	EDInteractiveControl_Button();
	static int edcMouseClickHandlerMaster(EDInteractiveControl&, GLfloat, GLfloat);
	static int edcMouseHoverHandlerMaster(EDInteractiveControl&, GLfloat, GLfloat);
	static int edcMouseHoverOutHandlerMaster(EDInteractiveControl&, GLfloat, GLfloat);
	static int edcFrameUpdateHandlerMaster(EDInteractiveControl&);

	void ediCustomFunc();
	void ediApplyDraw();

	void ediUpdate() {};

	void ediHoverAniController(int,...);
};

class EDInteractiveControl_PanelBack :public EDInteractiveControl {
public:
	static const int EDIPB_RESERVED_DISPPAGES = 150;
	static const int EDIPB_HOVER_FRAMES = 50;
	int iHoverAniFrame = 0;

	EDVector3D vAlarmColor;
	EDVector3D vFreeColor;
	EDVector3D vBusyColor;

	EDVector3D vAlarmColorF;
	EDVector3D vFreeColorF;
	EDVector3D vBusyColorF;

	EDInteractiveControl_PanelBack();
	void ediApplyDraw();
	void ediCustomFunc() {};
	void ediHoverAniController(int, ...);
	static int edcFrameUpdateHandlerMaster(EDInteractiveControl&);


	void ediUpdate() {};

};
class EDInteractiveControl_CircularProgressBar :public EDInteractiveControl {
public:
	GLfloat radi1, radi2;
	void ediApplyDraw();
	void ediCustomFunc() {};
	void ediHoverAniController(int, ...) {}
	void ediUpdate();
	void edQuickSetupR(float lx, float ly, float radius);
	EDInteractiveControl_CircularProgressBar();
};
class EDInteractiveControl_RoundButton :public EDInteractiveControl_Button {
public:
	int iArrowPattern = 0;
	int iOffestX = 0;
	void ediApplyDraw(); //overriding definition
	void edQuickSetupR(float lx, float ly, float radius);
};

class EDInteractiveControl_ArrowIndicator :public EDInteractiveControl {
public:
	void ediUpdate() {};
	bool bDirUp = true;
	EDInteractiveControl_ArrowIndicator();
	void ediApplyDraw();
	void ediCustomFunc() {};
	void ediHoverAniController(int, ...) {};
};


class EDInteractiveControl_Label :public EDInteractiveControl {
public:
	std::string sCaption = "Label";
	GLfloat iFontWidth = 1.0;
	EDVector3D vForeColor;
	GLfloat iFontScale = 0.15f;
	EDInteractiveControl_Label();
	void ediApplyDraw();
	void ediCustomFunc() {};
	void ediHoverAniController(int, ...) {};
	void ediUpdate();
};