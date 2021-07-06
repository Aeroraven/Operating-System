#include "EDInteractiveControl.h"

// EDInteractiveControl Base Class

void EDInteractiveControl::edDraw() {
	if (vDisplayPage.size() == 0) {
		return;
	}
	vDisplayPage[iActiveDisplayPage]->edDraw();
}

void EDInteractiveControl::edToggleDisplayPage(int p) {
	iActiveDisplayPage = p;
}

void EDInteractiveControl::edReceiveEventCall(int iCallerFlag, ...) {
	va_list aArguments;
	va_start(aArguments, iCallerFlag);
	switch (iCallerFlag)
	{
		case 0: //FrameUpdate
			if (hOnFrameUpdateHandler != nullptr) {
				hOnFrameUpdateHandler(*this);
			}
			break;
		case 1: //MouseClick
			if (hOnMouseClickHandler != nullptr) {
				GLfloat fMouseX, fMouseY;
				fMouseX = static_cast<float>(va_arg(aArguments, GLdouble));
				fMouseY = static_cast<float>(va_arg(aArguments, GLdouble));
				hOnMouseClickHandler(*this, fMouseX, fMouseY);
			}
			break;
		case 2: //MouseHover
			if (hOnMouseHoverHandler != nullptr) {
				GLfloat fMouseX, fMouseY;
				fMouseX = static_cast<float>(va_arg(aArguments, GLdouble));
				fMouseY = static_cast<float>(va_arg(aArguments, GLdouble));
				hOnMouseHoverHandler(*this, fMouseX, fMouseY);
			}
			break;
		case 3: //MouseHover - Exit
			if (hOnMouseHoverOutHandler != nullptr) {
				GLfloat fMouseX, fMouseY;
				fMouseX = static_cast<float>(va_arg(aArguments, GLdouble));
				fMouseY = static_cast<float>(va_arg(aArguments, GLdouble));
				hOnMouseHoverOutHandler(*this, fMouseX, fMouseY);
			}
			break;
		default:
			//Nothing here
			break;
	}
	va_end(aArguments);
}
EDInteractiveControl_CircularProgressBar::EDInteractiveControl_CircularProgressBar() {
	radi1 = 15;
	radi2 = 10;
	iTag3 = 270 - 1;
	for (int i = 0; i < 360; i++) {
		edInsertDisplayPage();
	}
	edToggleDisplayPage(210);
}
void EDInteractiveControl_CircularProgressBar::ediApplyDraw() {
	EDShape_Arc* r = new EDShape_Arc[360];
	EDAnimation aAni;
	EDVector3D  wcol, wcol2;

	wcol.edvSetColor(255, 255, 255, 0);
	wcol2.edvSetColor(255, 255, 255, 100);

	EDVector3D center;
	EDVector3D centerx;
	center.x = this->vLeftTopCoord.x + 0.5f * iWidth;
	center.y = this->vLeftTopCoord.y + 0.5f * iHeight;
	aAni.edaColorLinearTransform(180, r, wcol2, wcol2);
	aAni.edaColorCustomTransform1(180, r+180, wcol2, wcol);
	for (int i = 0; i < 360; i++) {
		r[i].edPresetArc(center, radi1, radi1, i);
		edInsertShapeCom(i, &r[i]);
	}
	delete[]r;
}

void EDInteractiveControl::edInsertDisplayPage() {
	EDShape* tmp = new EDShape();
	vDisplayPage.push_back(tmp);
}
void EDInteractiveControl::edDemolish() {
	for (int i = 0; i < vDisplayPage.size(); i++) {
		delete vDisplayPage[i];
	}
}
void EDInteractiveControl::edInsertShape(int iDisplayPage, EDShape* sShape) {
	vDisplayPage[iDisplayPage]->edPaste(sShape);
}

void EDInteractiveControl::edInsertShapeCom(int iDisplayPage, EDShapeBase* sShape) {
	vDisplayPage[iDisplayPage]->edAdd(sShape);
}

EDInteractiveControl::~EDInteractiveControl() {
	edDemolish();
}


//EDInteractiveControl Button

int EDInteractiveControl_Button::edcMouseClickHandlerMaster(EDInteractiveControl& self, GLfloat x, GLfloat y) {
	//Style Controller

	//User-defined Procedures
	if (self.hUsrOnMouseClickHandler != nullptr) {
		self.hUsrOnMouseClickHandler((void*)&self,x, y);
	}
	return 1;
}

int EDInteractiveControl_Button::edcMouseHoverHandlerMaster(EDInteractiveControl& self, GLfloat x, GLfloat y) {
	//Style Controller
	self.iMouseHoverStat = 1;

	//User-defined Procedures
	if (self.hUsrOnMouseHoverHandler != nullptr) {
		self.hUsrOnMouseHoverHandler(x, y);
	}
	return 1;
}

int EDInteractiveControl_Button::edcMouseHoverOutHandlerMaster(EDInteractiveControl& self, GLfloat x, GLfloat y) {
	//Style Controller
	self.iMouseHoverStat = 0;


	//User-defined Procedures
	if (self.hUsrOnMouseHoverHandler != nullptr) {
		self.hUsrOnMouseHoverHandler(x, y);
	}
	return 1;
}

int EDInteractiveControl_Button::edcFrameUpdateHandlerMaster(EDInteractiveControl& self) {
	//Style Controller
	self.ediHoverAniController(self.iMouseHoverStat);

	//User-defined Procedures
	
	return 1;
}

EDInteractiveControl_Button::EDInteractiveControl_Button() {
	this->hOnMouseClickHandler = &(EDInteractiveControl_Button::edcMouseClickHandlerMaster);
	this->hOnMouseHoverHandler = &(EDInteractiveControl_Button::edcMouseHoverHandlerMaster);
	this->hOnMouseHoverOutHandler = &(EDInteractiveControl_Button::edcMouseHoverOutHandlerMaster);
	this->hOnFrameUpdateHandler = &(EDInteractiveControl_Button::edcFrameUpdateHandlerMaster);

	this->vForeColor.edvSetVal(0.0, 0.0, 0.0);
	this->vBackColor.edvSetVal(1.0, 1.0, 1.0);
	edInsertDisplayPage(); //Main Page
	for (int i = 0; i <= EDIBTN_HOVER_FRAMES; i++) {
		edInsertDisplayPage(); //Hover Change Page
	}
	edToggleDisplayPage(0);
}

void EDInteractiveControl_Button::ediCustomFunc() {
	return;
}

void EDInteractiveControl_Button::ediApplyDraw() {
	EDShape_RoundedRectangle* r = new EDShape_RoundedRectangle();
	EDShape_RoundedRectangle* s = new EDShape_RoundedRectangle();
	EDShape_RoundedRectangle* bf = new EDShape_RoundedRectangle[EDIBTN_HOVER_FRAMES];

	EDAnimation aAni;
	

	EDVector3D redcol, wcol;
	wcol.edvSetVal(1, 1, 1);
	redcol.edvSetVal(1, 0, 0);
	EDVector3D lb = this->vLeftTopCoord;
	EDVector3D rt = lb;
	rt.x += this->iWidth;
	rt.y += this->iHeight;

	EDVector3D lt = lb;
	lt.y += this->iHeight;
	EDVector3D rb = lb;
	rb.x += this->iWidth;
	std::cout << rb.x << std::endl;
	r->edPresetRoundRect(lb, rb, lt, rt,10);
	r->edSetColor(wcol);

	aAni.edaDuplicateCopies(EDIBTN_HOVER_FRAMES, bf, r);
	aAni.edaColorLinearTransform(EDIBTN_HOVER_FRAMES, bf, wcol, redcol);

	for (int i = 0; i < EDIBTN_HOVER_FRAMES; i++) {
		std::cout << i << std::endl;
		edInsertShapeCom(i, &bf[i]);
	}
	delete r;
	delete s;
	delete[] bf;
}

void EDInteractiveControl_Button::ediHoverAniController(int iFlag, ...) {
	if (EDGlobal::edglAnimationQuality == EDGLM_LOWQUALITY) {
		if (iFlag == 0) {
			iHoverAniFrame = 0;
		}
		else {
			iHoverAniFrame = EDIBTN_HOVER_FRAMES - 1;
		}
		this->edToggleDisplayPage(this->iHoverAniFrame);
	}
	else {
		if (iFlag == 0 && iTag3 != 1) {
			if (iHoverAniFrame == 0) {
				return;
			}
			this->edToggleDisplayPage(--iHoverAniFrame);
		}
		else if (iFlag == 1 || iTag3 == 1) {
			if (iHoverAniFrame + 1 >= EDIBTN_HOVER_FRAMES) {
				return;
			}
			this->edToggleDisplayPage(++iHoverAniFrame);
		}
	}
	
}

int EDInteractiveControl_PanelBack::edcFrameUpdateHandlerMaster(EDInteractiveControl& self) {
	//Style Controller
	self.ediHoverAniController(0);

	//User-defined Procedures

	return 1;
}


void EDInteractiveControl_PanelBack::ediHoverAniController(int iFlag, ...) {
	if (EDGlobal::edglAnimationQuality == EDGLM_LOWQUALITY) {
		switch (iTag3) {
			case 0:
				iHoverAniFrame = 0;
				break;
			case 1:
				iHoverAniFrame = EDIPB_HOVER_FRAMES - 1;
				break;
			case 2:
				iHoverAniFrame = EDIPB_HOVER_FRAMES * 2 - 1;
				break;
		}
		this->edToggleDisplayPage(this->iHoverAniFrame);
	}
	else {
		if (iTag3 != 1 && iTag3 != 2 && iHoverAniFrame < EDIPB_HOVER_FRAMES) {
			if (iHoverAniFrame == 0) {
				return;
			}
			this->edToggleDisplayPage(--iHoverAniFrame);
		}
		else if (iTag3 == 1 && iHoverAniFrame < EDIPB_HOVER_FRAMES) {
			if (iHoverAniFrame + 1 >= EDIPB_HOVER_FRAMES) {
				return;
			}
			this->edToggleDisplayPage(++iHoverAniFrame);
		}
		else if (iTag3 == 2) {
			if (iHoverAniFrame < EDIPB_HOVER_FRAMES) {
				iHoverAniFrame = EDIPB_HOVER_FRAMES;
			}
			if (iHoverAniFrame + 1 >= EDIPB_HOVER_FRAMES * 2) {
				return;
			}
			this->edToggleDisplayPage(++iHoverAniFrame);
		}
		else {
			this->edToggleDisplayPage(--iHoverAniFrame);
			if (iHoverAniFrame == EDIPB_HOVER_FRAMES) {
				iHoverAniFrame = 0;
			}
		}
	}
	
}

EDInteractiveControl_PanelBack::EDInteractiveControl_PanelBack() 
{
	this->hOnFrameUpdateHandler = EDInteractiveControl_PanelBack::edcFrameUpdateHandlerMaster;
	for (int i = 0; i < EDIPB_RESERVED_DISPPAGES; i++) {
		edInsertDisplayPage();
	}
	edToggleDisplayPage(0);
	vAlarmColor.edvSetColor(255, 133, 133,100);
	vFreeColor.edvSetColor(152, 220, 130, 100);
	vBusyColor.edvSetColor(243, 172, 83, 100);

	//vAlarmColorF.edvSetColor(255, 178, 153, 100);
	vAlarmColorF.edvSetColor(251, 197, 157, 100);
	vFreeColorF.edvSetColor(197, 224, 180, 100);
	vBusyColorF.edvSetColor(246, 216, 95, 100);
}

void EDInteractiveControl_PanelBack::ediApplyDraw() {
	EDShape_RoundedRectangle* r = new EDShape_RoundedRectangle();
	EDShape_RoundedRectangle* s = new EDShape_RoundedRectangle();
	EDShape_RoundedRectangle* t = new EDShape_RoundedRectangle();
	EDShape_RoundedRectangle* bf = new EDShape_RoundedRectangle[EDIPB_HOVER_FRAMES];
	EDShape_RoundedRectangle* bf2 = new EDShape_RoundedRectangle[EDIPB_HOVER_FRAMES];
	r->edSetZIndex(iStandardZIndex);
	s->edSetZIndex(iStandardZIndex);
	t->edSetZIndex(iStandardZIndex);
	for (int i = 0; i < EDIPB_HOVER_FRAMES; i++) {
		bf[i].edSetZIndex(iStandardZIndex);
		bf2[i].edSetZIndex(iStandardZIndex);
	}
	EDAnimation aAni;
	EDVector3D lb = this->vLeftTopCoord;
	EDVector3D rt = lb;
	rt.x += this->iWidth;
	rt.y += this->iHeight;
	EDVector3D lt = lb;
	lt.y += this->iHeight;
	EDVector3D rb = lb;
	rb.x += this->iWidth;
	EDVector3D defaultCol, finalCol;
	defaultCol.edvClone(vFreeColor);
	finalCol.edvClone(vBusyColor);

	r->edPresetRoundRect(lb, rb, lt, rt, 10);
	s->edPresetRoundRect(lb, rb, lt, rt, 10);
	t->edPresetRoundRect(lb, rb, lt, rt, 10);

	r->iFillMode = EDShapeBase::EDS_FILL_BY_VERTEX;
	s->iFillMode = EDShapeBase::EDS_FILL_BY_VERTEX;
	t->iFillMode = EDShapeBase::EDS_FILL_BY_VERTEX;

	aAni.edabColorSetByDistance(r, rt, vFreeColor, vFreeColorF);
	aAni.edabColorSetByDistance(s, rt, vAlarmColor, vAlarmColorF);
	aAni.edabColorSetByDistance(t, rt, vBusyColor, vBusyColorF);


	aAni.edaDuplicateCopies(EDIPB_HOVER_FRAMES, bf, r);
	aAni.edaDuplicateCopies(EDIPB_HOVER_FRAMES, bf2, r);

	aAni.edabColorLinearTransformAll(EDIPB_HOVER_FRAMES,static_cast<int>( r->vVertexVector.size()), bf, r, t);
	aAni.edabColorLinearTransformAll(EDIPB_HOVER_FRAMES,static_cast<int>( r->vVertexVector.size()), bf2, r, s);
	for (int i = 0; i < EDIPB_HOVER_FRAMES; i++) {
		edInsertShapeCom(i, &bf[i]);
	}
	for (int i = 0; i < EDIPB_HOVER_FRAMES; i++) {
		edInsertShapeCom(i+ EDIPB_HOVER_FRAMES, &bf2[i]);
	}

	delete r;
	delete s;
	delete t;
	delete[] bf;
	delete[] bf2;
}

void EDInteractiveControl::edQuickSetup(float lx, float ly, float w, float h) {
	vLeftTopCoord.x = lx;
	vLeftTopCoord.y = ly;
	iWidth = w;
	iHeight = h;
}
void EDInteractiveControl_RoundButton::ediApplyDraw() {
	EDShape_Circle* r = new EDShape_Circle();
	EDShape_Circle* s = new EDShape_Circle();
	EDShape_Circle* bf = new EDShape_Circle[EDIBTN_HOVER_FRAMES];
	EDShape_Circle* bf2 = new EDShape_Circle[EDIBTN_HOVER_FRAMES];
	EDShape_Circle* bf3 = new EDShape_Circle[EDIBTN_HOVER_FRAMES];
	EDShape_Arrow* bf4 = new EDShape_Arrow[EDIBTN_HOVER_FRAMES];
	EDShape_Arrow* arw = new EDShape_Arrow();

	EDShape_Text* s2 = new EDShape_Text();


	EDAnimation aAni;


	EDVector3D redcol, wcol, wcol2;

	redcol.edvSetColor(255, 100, 100);
	wcol.edvSetColor(200, 200, 200);
	wcol2.edvSetColor(255, 255, 255);
	EDVector3D center;
	EDVector3D centerx;
	center.x = this->vLeftTopCoord.x + 0.5f * iWidth;
	center.y = this->vLeftTopCoord.y + 0.5f * iHeight;

	r->edPresetCircle(center, 0.5f * iWidth);
	s->edPresetCircle(center, 0.45f * iWidth);
	r->edSetColor(wcol);
	s->edSetColor(wcol2);

	centerx.edvClone(center);
	centerx.x -= 12 + iOffestX;
	centerx.y -= 8;

	s2->edAddVertex(centerx);
	s2->edPresetText(sCaption);
	s2->edSetColor(wcol);

	if (iArrowPattern == 1) {
		EDVector3D center2;
		center2.edvClone(center);
		center2.y += 10;
		arw->edPresetArrowA(center2);
		arw->edSetColor(wcol);
		aAni.edaDuplicateCopies(EDIBTN_HOVER_FRAMES, bf4, arw);
	}
	else if (iArrowPattern == 2) {
		EDVector3D center2;
		center2.edvClone(center);
		center2.y -= 10;
		arw->edPresetArrowB(center2);
		arw->edSetColor(wcol);
		aAni.edaDuplicateCopies(EDIBTN_HOVER_FRAMES, bf4, arw);
	}

	aAni.edaDuplicateCopies(EDIBTN_HOVER_FRAMES, bf, r);
	aAni.edaDuplicateCopies(EDIBTN_HOVER_FRAMES, bf2, s);
	aAni.edaDuplicateCopies(EDIBTN_HOVER_FRAMES, bf3, s2);

	aAni.edaColorLinearTransform(EDIBTN_HOVER_FRAMES, bf, wcol, redcol);
	aAni.edaColorLinearTransform(EDIBTN_HOVER_FRAMES, bf3, wcol, redcol);
	if (iArrowPattern) {
		aAni.edaColorLinearTransform(EDIBTN_HOVER_FRAMES, bf4, wcol, redcol);
	}

	for (int i = 0; i < EDIBTN_HOVER_FRAMES; i++) {
		edInsertShapeCom(i, &bf[i]);
		edInsertShapeCom(i, &bf2[i]);
		edInsertShapeCom(i, &bf3[i]);
		if (iArrowPattern) {
			edInsertShapeCom(i,&bf4[i] );
		}
	}
	delete r;
	delete s;
	delete[] bf;
	delete[] bf2;
	delete[] bf3;
	delete[] bf4;
	delete arw;
	delete s2;
}
void EDInteractiveControl_CircularProgressBar::ediUpdate() {
	edToggleDisplayPage(iTag3);
}
void EDInteractiveControl_CircularProgressBar::edQuickSetupR(float lx, float ly, float radius) {
	vLeftTopCoord.x = lx - radius;
	vLeftTopCoord.y = ly - radius;
	iHeight = 2.0f * radius;
	iWidth = 2.0f * radius;
	radi1 = radius;
}

void EDInteractiveControl_RoundButton::edQuickSetupR(float lx, float ly, float radius){
	vLeftTopCoord.x = lx - radius;
	vLeftTopCoord.y = ly - radius;
	iHeight = 2.0f * radius;
	iWidth = 2.0f * radius;
}
EDInteractiveControl_Label::EDInteractiveControl_Label() {
	edInsertDisplayPage();
	edToggleDisplayPage(0);
	EDVector3D redcol, wcol, wcol2;
	redcol.edvSetColor(255, 255, 255);
	vForeColor.edvClone(redcol);
}

void EDInteractiveControl_Label::ediApplyDraw() {
	EDShape_Text* s2 = new EDShape_Text();
	
	s2->edAddVertex(vLeftTopCoord);
	s2->edPresetText(sCaption);
	s2->edSetColor(vForeColor);
	s2->iFontBold = iFontWidth;
	s2->iFontScale = iFontScale;
	edInsertShapeCom(0,s2);
	delete s2;
	
}

void EDInteractiveControl_Label::ediUpdate() {
	this->vDisplayPage[0]->vComponentContainer.clear();
	this->ediApplyDraw();
}
void EDInteractiveControl_ArrowIndicator::ediApplyDraw() {
	EDShape_IsoscelesRightTriangle* s2 = new EDShape_IsoscelesRightTriangle();
	if (bDirUp) {
		s2->edPresetTriangleA(vLeftTopCoord, 15);
	}
	else {
		s2->edPresetTriangleB(vLeftTopCoord, 15);
	}
	EDVector3D redcol, wcol, wcol2;
	redcol.edvSetColor(255, 255, 255);
	s2->edSetColor(redcol);
	edInsertShapeCom(0, s2);
	delete s2;
}

EDInteractiveControl_ArrowIndicator::EDInteractiveControl_ArrowIndicator() {
	edInsertDisplayPage();
	edToggleDisplayPage(0);
}