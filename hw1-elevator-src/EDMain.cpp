#include "EDMain.h"

void EDMain::edmGLDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f * EDGlobal::edglColorMultiplier, 1.0f * EDGlobal::edglColorMultiplier, 1.0f * EDGlobal::edglColorMultiplier, 0.0);
	edcm->edcmGLDisplay();
	glFlush();

}
EDMain::EDMain() {
	EDLogClass.edlgRecordLog(EDLogClass.edMain, EDLogClass.edINFO, "Instance Launched");
	edcm = new EDControlManager();
	edtc = new EDTimerController();
	edceElevator = new EDElevator[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edceElevator[i].iID = i + 1;
	}
	edtcArgvBuffer = new int[EDGlobal::EDGL_ARGV_BUFFER];
	ederList = new EDElevatorRequest[EDGlobal::EDGL_FLOORS * 2];
	//Fill ederList
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		ederList[i].iRequestFloor = (i / 2) + 1;
		ederList[i].iRequestDirection = ((i % 2 == 0) ? EDElevator::EDEL_ASCENDING : EDElevator::EDEL_DESCENDING);
		ederList[i].iRequestValid = 0;
		ederList[i].iRequestCreateTime = 0;
		ederList[i].iRequestRecipients = 0;
		ederList[i].iPostponeTime = 0;
	}
	EDElevator::vExternalList = ederList;
}

void EDMain::edmDispatchExternal(int x, const std::vector<void*>& argtmp) {
	EDGlobals.EDGL_Mutex.lock();
	int* privl = new int[ED_MAIN_VALID_ELEVATORS];
	int xr = 0;
	for (int k = 0; k < EDGlobal::EDGL_FLOORS * 2; k++) {
		if (ederList[k].iRequestValid && ederList[k].iRequestRecipients!=0 && ederList[k].iPostponeTime==0) {
			ederList[k].iReallocateTime--;
		}
		if (ederList[k].iRequestValid && ederList[k].iPostponeTime != 0) {
			ederList[k].iPostponeTime--;
			continue;
		}
		if (ederList[k].iReallocateTime == 0 && ederList[k].iRequestValid) {
			
			std::stringstream oss;
			oss << "Rescheduling the external task #" << k << ", which was originally occupied by elevator #"<< ederList[k].iRequestRecipients;
			EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());
			ederList[k].iRedispatchFlag = 1;

		}
		if (ederList[k].iRequestValid == 1 && ederList[k].iRequestRecipients != 0 &&
			edceElevator[ederList[k].iRequestRecipients - 1].iStatus == EDElevator::EDEL_ABORTED) {
			ederList[k].iRequestRecipients = 0;
			
			std::stringstream oss;
			oss << "Rescheduling external task #" << k << ", which was originally occupied by aborted elevator #" << ederList[k].iRequestRecipients;
			EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());

		}
		if (ederList[k].iRequestValid == 1 && (ederList[k].iRequestRecipients == 0 || ederList[k].iRedispatchFlag ==1)) {
			std::stringstream oss;
			int curl = 0x0ffffff;
			if (ederList[k].iRedispatchFlag == 1 && ederList[k].iRequestRecipients!=0) {
				curl = ederList[k].ederCalculatePriority(edceElevator[ederList[k].iRequestRecipients - 1]);
				curl -= EDGlobal::EDGL_REALLOCATION_TIMESLICE;
				ederList[k].iReallocateTime = EDGlobal::EDGL_REALLOCATION_TIMESLICE;
				ederList[k].iRedispatchFlag = 0;
			}
			for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
				privl[i] = ederList[k].ederCalculatePriority(edceElevator[i]) ;
				if (edceElevator[i].iStatus == EDElevator::EDEL_FREE) {
					//privl[i]++;
				}
				if (privl[i] < curl && privl[i]>=0) {
					curl = privl[i];
					ederList[k].iRequestRecipients = i + 1;// Added one
					ederList[k].iReallocateTime = EDGlobal::EDGL_REALLOCATION_TIMESLICE;
					ederList[k].iRedispatchFlag = 0;
				}
				xr += privl[i];
			}
			if (xr == -2*ED_MAIN_VALID_ELEVATORS) {
				oss << "Canceled the external tasks : all elevators are stopped";
				ederList[k].iRequestValid = 0;
				EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());

			}
			else {
				if (ederList[k].iRequestRecipients == 0) {
					ederList[k].iPostponeTime = EDGlobal::EDGL_POSTPONE_TIMESLICE;
					oss << "External task #"<<k<<" is postponed./ ETA:" << privl[0] << "," << privl[1] << "," << privl[2] << "," << privl[3] << "," << privl[4];
					EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());
				}
				else {
					oss << "Assigned the external task #"<<k<<" to elevator #" << ederList[k].iRequestRecipients << "/ ETA:" << privl[0] << "," << privl[1] << "," << privl[2] << "," << privl[3] << "," << privl[4];
					EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());
				}

			}
		}
	}
	
	delete[]privl;
	EDGlobals.EDGL_Mutex.unlock();
}

void EDMain::edmGLInit(int* argc, char** argv) {
	//Window Initializes
	glutInit(argc, argv);
	if (EDGlobal::edglAnimationQuality == EDGLM_LOWQUALITY) {
		EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edWARNING, "Running in Low Quality Mode");
		EDCM_GLUTANI_INTERVAL = 30;
	}
	if (EDGlobal::edglColorMode == EDGLM_GRAYSCALE) {
		EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edWARNING, "Running in Grayscale Mode");

	}

	if (EDGlobal::edglAnimationQuality != EDGLM_LOWQUALITY) {
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_MULTISAMPLE);
		glEnable(GLUT_MULTISAMPLE);
		EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edINFO, "Enabling Multisampling");

	}
	else {
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
	}
	glutInitWindowPosition(EDCM_GLUTPOS_LEFT, EDCM_GLUTPOS_TOP);
	glutInitWindowSize(EDCM_GLUTWINDOW_INIT_W, EDCM_GLUTWINDOW_INIT_H);
	glutCreateWindow(EDCM_TITLE);

	if (EDGlobal::edglAnimationQuality != EDGLM_LOWQUALITY) {
		//Transparency Settings
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//Coordinate Settings
	glClearColor(1.0f * EDGlobal::edglColorMultiplier, 1.0f * EDGlobal::edglColorMultiplier, 1.0f * EDGlobal::edglColorMultiplier, 0.0);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, static_cast<int>(EDCM_GLUTORTHO_W), static_cast<int>(EDCM_GLUTORTHO_H));
	glLoadIdentity();
	glOrtho(0.0, EDCM_GLUTORTHO_W, 0.0, EDCM_GLUTORTHO_H, -10.0, 10.0);

	//Quality Settings
	if (EDGlobal::edglAnimationQuality != EDGLM_LOWQUALITY) {
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POINT_SMOOTH, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH, GL_NICEST);
		glHint(GL_LINE_SMOOTH, GL_NICEST);
	}
	

	//Event Connections
	glutMouseFunc(EDMain::edmGLMouseKeyHandler);
	glutPassiveMotionFunc(EDMain::edmGLMouseMotionHandler);
	glutReshapeFunc(EDMain::edmGLResizeHandler);

	EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edINFO, "Graphics initialization was done.");

}

void EDMain::edmGLInitDisplay() {
	glutDisplayFunc(EDMain::edmGLDisplay);
}

void EDMain::edmGLStart(int* argc, char** argv) {
	edmGLInit(argc, argv);
	edmDeployControls();
	edmGLInitDisplay();

	glutTimerFunc(EDCM_GLUTANI_INTERVAL, edmGLFrameUpdate, 1);
	glutMainLoop();

}

void EDMain::edmStart(int* argc, char** argv) {
	//Thread create
	edmtUIThread = new std::thread(&EDMain::edmGLStart, argc, argv);
	edmtCONTestThread = new std::thread(&EDMain::edmDebug);

	//Dispatch Threads
	edmDispatchStart();

	//Thread join
	edmtUIThread->join();
	edtc->edtcThreadJoin();
	return;

}

void EDMain::edmDispatchStart() {
	edmtElevatorThreadPtr = new EDTimer * [ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		std::vector<void*> vPtr;
		vPtr.push_back((void*)&(edceElevator[i]));
		edmtElevatorThreadPtr[i] = new EDTimer();
		edmtElevatorThreadPtr[i]->edtSetup(EDElevator::edelDispatcher, 1, vPtr, 10, false);
		edtc->edtcAddTimer(edmtElevatorThreadPtr[i]);
	}
	edmtExternalDispatchThread = new EDTimer();
	edmtExternalDispatchThread->edtSetup(EDMain::edmDispatchExternal, 1, std::vector<void*>{}, 10, false);
	edtc->edtcAddTimer(edmtExternalDispatchThread);
}

void EDMain::edmSynchronizeUIAttrib() {
	if (edmgUIInitStatus == 0) {
		return;
	}
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelLED[i].sCaption = edceElevator[i].edelConvertFloor2Str();
		edcpElevatorCarPanelStatusBar[i].sCaption = edceElevator[i].edelConvertStatus2Str();
		edcpTimeLeftIndicator[i].iTag3 = 359 * edceElevator[i].iRemainingTimeSlice / edceElevator[i].edelGetStatusMaxTime();
		EDVector3D validcol, invalidcol;
		validcol.edvSetColor(255, 255, 255);
		invalidcol.edvSetColor(200, 200, 200);

		if (edceElevator[i].iDirection == EDElevator::EDEL_ASCENDING) {
			edcpElevatorCarUpIndicator[i].vDisplayPage[0]->vComponentContainer[0].edSetColor(validcol);
			edcpElevatorCarDwIndicator[i].vDisplayPage[0]->vComponentContainer[0].edSetColor(invalidcol);
		}
		else if (edceElevator[i].iDirection == EDElevator::EDEL_DESCENDING) {
			edcpElevatorCarDwIndicator[i].vDisplayPage[0]->vComponentContainer[0].edSetColor(validcol);
			edcpElevatorCarUpIndicator[i].vDisplayPage[0]->vComponentContainer[0].edSetColor(invalidcol);
		}
		else {
			edcpElevatorCarDwIndicator[i].vDisplayPage[0]->vComponentContainer[0].edSetColor(invalidcol);
			edcpElevatorCarUpIndicator[i].vDisplayPage[0]->vComponentContainer[0].edSetColor(invalidcol);
		}

		if (edceElevator[i].iStatus == EDElevator::EDEL_FREE) {
			edcpElevatorPanelBack[i].iTag3 = 0;
		}
		else if (edceElevator[i].iStatus == EDElevator::EDEL_ABORTED) {
			edcpElevatorPanelBack[i].iTag3 = 2;
		}
		else {
			edcpElevatorPanelBack[i].iTag3 = 1;
		}
		for (int j = 0; j < ED_MAIN_FLOORS; j++) {
			edcpElevatorCarPanelNormalBtn[i][j].iTag3 = edceElevator[i].iInternalRequests[j + 1];
		}

	}
	for (int i = 0; i < EDGlobal::EDGL_FLOORS; i++) {
		edcpElevatorFloorPanelUpBtn[i].iTag3 = ederList[(edcpElevatorFloorPanelUpBtn[i].iTag1 - 1001) * 2].iRequestValid;
		edcpElevatorFloorPanelDwBtn[i].iTag3 = ederList[(edcpElevatorFloorPanelDwBtn[i].iTag1 - 1001) * 2 + 1].iRequestValid;
	}
}

void EDMain::edmGLFrameUpdate(int x) {
	
	edcm->edcmActivateFrameUpdateEvent();
	edcm->edcmUpdate();
	edmSynchronizeUIAttrib();
	glutPostRedisplay();
	//EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edINFO, "Frame Updates");

	glutTimerFunc(EDCM_GLUTANI_INTERVAL, edmGLFrameUpdate, 1);
}

void a(int x, const std::vector<void*>& w) {
	//EDLogClass.edlgRecordLog(EDLogClass.edMain, EDLogClass.edINFO, "Func Activated");
	for (int i = 0; i < EDMain::ED_MAIN_VALID_ELEVATORS; i++)
	{
		EDMain::edcpElevatorCarPanelLED[i].sCaption = EDMain::edceElevator[i].edelConvertFloor2Str();
	}
}
void EDMain::edmLowQualityMode() {
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);

}
void EDMain::edmDebug() {
	//EDTimer* nt = new EDTimer();
	//nt->edtSetup(a, 0, std::vector<void*>(),5000);
	//edtc->edtcAddTimer(nt);
	//EDLogClass.edlgRecordLog(EDLogClass.edMain, EDLogClass.edINFO, "Debug Thread Created");
}

void EDMain::edmDeployControls() {
	//UI Initialization Flag
	edmgUIInitStatus = 1;

	//Car Panel Backgrounds
	edcpElevatorPanelBack = new EDInteractiveControl_PanelBack[ED_MAIN_ELEVATORS];
	edcpElevatorPanelBackShadow = new EDInteractiveControl_PanelBack[ED_MAIN_ELEVATORS];
	
	for (int i = 0; i < ED_MAIN_ELEVATORS; i++) {
		edcpElevatorPanelBack[i].edQuickSetup(ED_MAIN_ELEVATOR_PANEL_X[i]+0.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + 0.0f, ED_MAIN_ELEVATOR_PANEL_W + 0.0f, ED_MAIN_ELEVATOR_PANEL_H + 0.0f);
		edcpElevatorPanelBackShadow[i].edQuickSetup(ED_MAIN_ELEVATOR_PANEL_X[i] + 5.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] - 5.0f, ED_MAIN_ELEVATOR_PANEL_W + 0.0f, ED_MAIN_ELEVATOR_PANEL_H + 0.0f);

		edcpElevatorPanelBack[i].iTag3 = 0;
		edcpElevatorPanelBack[i].edSetZIndex(-7);
		edcpElevatorPanelBackShadow[i].iTag3 = 0;
		edcpElevatorPanelBackShadow[i].edSetZIndex(-6);


		edcpElevatorPanelBackShadow[i].vFreeColor.edvSetColor(220, 220, 220,100);
		edcpElevatorPanelBackShadow[i].vAlarmColor.edvSetColor(220, 220, 220, 100);
		edcpElevatorPanelBackShadow[i].vBusyColor.edvSetColor(220, 220, 220, 100);
		edcpElevatorPanelBackShadow[i].vFreeColorF.edvSetColor(220, 220, 220, 100);
		edcpElevatorPanelBackShadow[i].vAlarmColorF.edvSetColor(220, 220, 220, 100);
		edcpElevatorPanelBackShadow[i].vBusyColorF.edvSetColor(220, 220, 220, 100);
		edcm->edcmAddControl(&edcpElevatorPanelBackShadow[i]);
		edcm->edcmAddControl(&edcpElevatorPanelBack[i]);
	}
	for (int i = ED_MAIN_VALID_ELEVATORS; i < ED_MAIN_ELEVATORS; i++) {

		edcpElevatorPanelBack[i].vFreeColor.edvSetColor(234, 234, 234);
		edcpElevatorPanelBack[i].vAlarmColor.edvSetColor(234, 234, 234);
		edcpElevatorPanelBack[i].vBusyColor.edvSetColor(234, 234, 234);
		edcpElevatorPanelBack[i].vFreeColorF.edvSetColor(234, 234, 234);
		edcpElevatorPanelBack[i].vAlarmColorF.edvSetColor(234, 234, 234);
		edcpElevatorPanelBack[i].vBusyColorF.edvSetColor(234, 234, 234);
		edcm->edcmAddControl(&edcpElevatorPanelBackShadow[i]);
		edcm->edcmAddControl(&edcpElevatorPanelBack[i]);

	}



	//Floor Panel Background
	edcpElevatorFloorPanelBack = new EDInteractiveControl_PanelBack();
	edcpElevatorFloorPanelBack->edQuickSetup(ED_MAIN_ELEVATOR_FLOOR_PANEL_X, ED_MAIN_ELEVATOR_FLOOR_PANEL_Y,
		ED_MAIN_ELEVATOR_FLOOR_PANEL_W, ED_MAIN_ELEVATOR_FLOOR_PANEL_H);
	edcpElevatorFloorPanelBack->edSetZIndex(-7);
	edcpElevatorFloorPanelBack->vFreeColor.edvSetColor(147, 199, 252);
	edcpElevatorFloorPanelBack->vFreeColorF.edvSetColor(147, 225, 252);
	edcpElevatorFloorPanelBack->vAlarmColor.edvSetColor(147, 225, 252);
	edcpElevatorFloorPanelBack->vAlarmColorF.edvSetColor(147, 225, 252);
	edcpElevatorFloorPanelBack->vBusyColor.edvSetColor(147, 225, 252);
	edcpElevatorFloorPanelBack->vBusyColorF.edvSetColor(147, 225, 252);
	

	//Floor Panel Background Border Shadow
	edcpElevatorFloorPanelBackShadow = new EDInteractiveControl_PanelBack();
	edcpElevatorFloorPanelBackShadow->edQuickSetup(ED_MAIN_ELEVATOR_FLOOR_PANEL_X+5, ED_MAIN_ELEVATOR_FLOOR_PANEL_Y-5,
		ED_MAIN_ELEVATOR_FLOOR_PANEL_W, ED_MAIN_ELEVATOR_FLOOR_PANEL_H);
	edcpElevatorFloorPanelBackShadow->vFreeColor.edvSetColor(234, 234, 234);
	edcpElevatorFloorPanelBackShadow->vFreeColor.edvSetColor(234, 234, 234);;
	edcpElevatorFloorPanelBackShadow->vFreeColorF.edvSetColor(234, 234, 234);;
	edcpElevatorFloorPanelBackShadow->vAlarmColor.edvSetColor(234, 234, 234);
	edcpElevatorFloorPanelBackShadow->vAlarmColorF.edvSetColor(234, 234, 234);
	edcpElevatorFloorPanelBackShadow->vBusyColor.edvSetColor(234, 234, 234);
	edcpElevatorFloorPanelBackShadow->vBusyColorF.edvSetColor(234, 234, 234);

	edcpElevatorFloorPanelBackShadow->edSetZIndex(-6);
	edcm->edcmAddControl(edcpElevatorFloorPanelBackShadow);

	edcm->edcmAddControl(edcpElevatorFloorPanelBack);

	//Floor Panel Upward Button
	edcpElevatorFloorPanelUpBtn = new EDInteractiveControl_RoundButton[ED_MAIN_FLOORS];
	for (int i = 0; i < ED_MAIN_FLOORS; i++) {
		if (i == ED_MAIN_FLOORS - 1)
		{
			break;
		}
		edcpElevatorFloorPanelUpBtn[i].edQuickSetupR(ED_MAIN_ELEVATORS_FLOOR_UPBTN_X, i * 42 + 75.0f, 20);
		edcpElevatorFloorPanelUpBtn[i].sCaption = " ";
		edcpElevatorFloorPanelUpBtn[i].iArrowPattern = 1;
		edcpElevatorFloorPanelUpBtn[i].iTag1 = 1001 + i;
		edcpElevatorFloorPanelUpBtn[i].iTag2 = EDElevator::EDEL_ASCENDING;
		edcpElevatorFloorPanelUpBtn[i].hUsrOnMouseClickHandler = EDMain::edmCarBtnClickHandler;

		edcm->edcmAddControl(&edcpElevatorFloorPanelUpBtn[i]);
	}

	//Floor Panel Downward Button
	edcpElevatorFloorPanelDwBtn = new EDInteractiveControl_RoundButton[ED_MAIN_FLOORS];
	for (int i = 0; i < ED_MAIN_FLOORS; i++) {
		if (i == 0)continue;
		edcpElevatorFloorPanelDwBtn[i].edQuickSetupR(ED_MAIN_ELEVATORS_FLOOR_DWBTN_X, i * 42 + 75.0f, 20);
		edcpElevatorFloorPanelDwBtn[i].sCaption = " ";
		edcpElevatorFloorPanelDwBtn[i].iArrowPattern = 2;
		edcpElevatorFloorPanelDwBtn[i].iTag1 = 1001 + i;
		edcpElevatorFloorPanelDwBtn[i].iTag2 = EDElevator::EDEL_DESCENDING;
		edcpElevatorFloorPanelDwBtn[i].hUsrOnMouseClickHandler = EDMain::edmCarBtnClickHandler;

		edcm->edcmAddControl(&edcpElevatorFloorPanelDwBtn[i]);
	}

	//Floor Panel Label
	edcpElevatorFloorPanelLabel = new EDInteractiveControl_Label[ED_MAIN_FLOORS];
	for (int i = 0; i < ED_MAIN_FLOORS; i++) {
		std::stringstream converter;
		edcpElevatorFloorPanelLabel[i].edQuickSetup(ED_MAIN_ELEVATORS_FLOOR_LABEL_X, i * 42 + 70.0f, 0, 0);
		converter << (i + 1) << "F";
		edcpElevatorFloorPanelLabel[i].sCaption = converter.str();
		edcm->edcmAddControl(&edcpElevatorFloorPanelLabel[i]);
	}

	//Car Panel Normal Buttons
	edcpElevatorCarPanelNormalBtn = new EDInteractiveControl_RoundButton * [ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelNormalBtn[i] = new EDInteractiveControl_RoundButton[ED_MAIN_FLOORS];
		for (int j = 0; j < ED_MAIN_FLOORS; j++) {
			std::stringstream converter;
			GLfloat offsetX, offsetY;
			offsetX = static_cast<float>((j % 5) * 50 + 50);
			offsetY = static_cast<float>((j / 5) * 50 + 50);
			if (j + 1 < 10) {
				converter << "0";
			}
			converter << (j + 1);
			edcpElevatorCarPanelNormalBtn[i][j].iTag1 = i + 1;
			edcpElevatorCarPanelNormalBtn[i][j].iTag2 = j + 1;
			edcpElevatorCarPanelNormalBtn[i][j].hUsrOnMouseClickHandler = EDMain::edmCarBtnClickHandler;
			edcpElevatorCarPanelNormalBtn[i][j].sCaption = converter.str();
			edcpElevatorCarPanelNormalBtn[i][j].edQuickSetupR(ED_MAIN_ELEVATOR_PANEL_X[i] + offsetX,
				ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - offsetY, 20);
			edcm->edcmAddControl(&edcpElevatorCarPanelNormalBtn[i][j]);
		}
	}

	//Car Panel LED
	edcpElevatorCarPanelLED = new EDInteractiveControl_Label[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelLED[i].edQuickSetup(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 115.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 120.0f, 0, 0);
		edcpElevatorCarPanelLED[i].iFontScale = 0.4f;
		edcpElevatorCarPanelLED[i].iFontWidth = 4;
		edcpElevatorCarPanelLED[i].sCaption = "XX";
		edcm->edcmAddControl(&edcpElevatorCarPanelLED[i]);
	}

	//Car Panel Status Bar
	edcpElevatorCarPanelStatusBar = new EDInteractiveControl_Label[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelStatusBar[i].edQuickSetup(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 107.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 160.0f, 0, 0);
		edcpElevatorCarPanelStatusBar[i].sCaption = "FREE";
		edcpElevatorCarPanelStatusBar[i].iFontWidth = 2;
		edcm->edcmAddControl(&edcpElevatorCarPanelStatusBar[i]);
	}

	//Car Panel Up Indicator
	edcpElevatorCarUpIndicator = new EDInteractiveControl_ArrowIndicator[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarUpIndicator[i].edQuickSetup(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 100.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 50.0f + 15.0f, 0, 0);

		edcm->edcmAddControl(&edcpElevatorCarUpIndicator[i]);
	}

	//Car Panel Down Indicator
	edcpElevatorCarDwIndicator = new EDInteractiveControl_ArrowIndicator[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarDwIndicator[i].edQuickSetup(static_cast<float>(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 70.0f),
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 50.0f, 0, 0);
		edcpElevatorCarDwIndicator[i].bDirUp = false;
		edcm->edcmAddControl(&edcpElevatorCarDwIndicator[i]);
	}

	//Car Panel Alarm 
	edcpElevatorCarPanelAlarmBtn = new EDInteractiveControl_RoundButton[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelAlarmBtn[i].edQuickSetupR(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 120.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 200.0f, 20);
		edcpElevatorCarPanelAlarmBtn[i].iTag1 = i + 1;
		edcpElevatorCarPanelAlarmBtn[i].iTag2 = 100;
		edcpElevatorCarPanelAlarmBtn[i].hUsrOnMouseClickHandler = EDMain::edmCarBtnClickHandler;
		edcpElevatorCarPanelAlarmBtn[i].sCaption = " !";
		edcpElevatorCarPanelAlarmBtn[i].iOffestX = 5;
		edcm->edcmAddControl(&edcpElevatorCarPanelAlarmBtn[i]);
	}

	//Car Panel Open 
	edcpElevatorCarPanelOpenBtn = new EDInteractiveControl_RoundButton[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelOpenBtn[i].edQuickSetupR(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 75.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 200.0f, 20.0f);
		edcpElevatorCarPanelOpenBtn[i].sCaption = "<|>";
		edcpElevatorCarPanelOpenBtn[i].iTag1 = i + 1;
		edcpElevatorCarPanelOpenBtn[i].iTag2 = 200;
		edcpElevatorCarPanelOpenBtn[i].iOffestX = 2;
		edcpElevatorCarPanelOpenBtn[i].hUsrOnMouseClickHandler = EDMain::edmCarBtnClickHandler;
		edcm->edcmAddControl(&edcpElevatorCarPanelOpenBtn[i]);
	}

	//Car Panel Close 
	edcpElevatorCarPanelCloseBtn = new EDInteractiveControl_RoundButton[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorCarPanelCloseBtn[i].edQuickSetupR(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 30.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 200.0f, 20.0f);
		edcpElevatorCarPanelCloseBtn[i].sCaption = ">|<";
		edcpElevatorCarPanelCloseBtn[i].iTag1 = i + 1;
		edcpElevatorCarPanelCloseBtn[i].iTag2 = 300;
		edcpElevatorCarPanelCloseBtn[i].iOffestX = 1;
		edcpElevatorCarPanelCloseBtn[i].hUsrOnMouseClickHandler = EDMain::edmCarBtnClickHandler;

		edcm->edcmAddControl(&edcpElevatorCarPanelCloseBtn[i]);
	}

	//Title
	edcpTitle = new EDInteractiveControl_Label();
	edcpTitle->edQuickSetup(50, EDCM_GLUTORTHO_H - 75, 0, 0);
	edcpTitle->iFontScale = 0.35f;
	edcpTitle->iFontWidth = 4;
	edcpTitle->vForeColor.edvSetColor(200, 200, 200);
	edcpTitle->sCaption = "Elevator Dispatch Simulator";
	edcm->edcmAddControl(edcpTitle);

	//Author
	edcpAuthor = new EDInteractiveControl_Label();
	edcpAuthor->edQuickSetup(0, 5, 0, 0);
	edcpAuthor->iFontScale = 0.1f;
	edcpAuthor->iFontWidth = 1;
	edcpAuthor->vForeColor.edvSetColor(255, 0, 0);
	edcpAuthor->sCaption = "By 1950641 Huang Zhiwen | Tongji SSE Operating System Homework 1";
	edcm->edcmAddControl(edcpAuthor);

	//Elevator Labels
	edcpElevatorLabels = new EDInteractiveControl_Label[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpElevatorLabels[i].edQuickSetup(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 245.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 270.0f, 0, 0);
		edcpElevatorLabels[i].iFontScale = 0.1f;
		edcpElevatorLabels[i].iFontWidth = 1;
		std::stringstream conv;
		conv << "Elevator " << i + 1;
		edcpElevatorLabels[i].sCaption = conv.str();
		edcpElevatorLabels[i].vForeColor.edvSetColor(200, 200, 200);

		edcm->edcmAddControl(&edcpElevatorLabels[i]);
	}

	//Time Bar
	edcpTimeLeftIndicator = new EDInteractiveControl_CircularProgressBar[ED_MAIN_VALID_ELEVATORS];
	for (int i = 0; i < ED_MAIN_VALID_ELEVATORS; i++) {
		edcpTimeLeftIndicator[i].edQuickSetupR(ED_MAIN_ELEVATOR_PANEL_X[i] + ED_MAIN_ELEVATOR_PANEL_W - 85.0f,
			ED_MAIN_ELEVATOR_PANEL_Y[i] + ED_MAIN_ELEVATOR_PANEL_H - 100.0f, 40);
		edcpTimeLeftIndicator[i].iTag3 = 359;
		edcm->edcmAddControl(&edcpTimeLeftIndicator[i]);
	}
	

	//Debug



	//All done
	edcm->edcmDeploy();
}
int EDMain::edmCarBtnClickHandler(void* object, GLfloat x, GLfloat y) {
	std::stringstream oss,oss1;
	EDInteractiveControl_RoundButton* self = (EDInteractiveControl_RoundButton*)object;
	EDGlobals.EDGL_Mutex.lock();
	if (self->iTag2 <= ED_MAIN_FLOORS) {
		if (self->iTag1 > 1000) {
			for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
				if (self->iTag1 == ederList[i].iRequestFloor + 1000 && self->iTag2 == ederList[i].iRequestDirection) {
					ederList[i].iRequestValid = 1;
				}
			}
		}
		else {
			edceElevator[self->iTag1 - 1].iInternalRequests[self->iTag2] = 1;
		}
	}
	else {
		if (self->iTag2 == 100) {
			edceElevator[self->iTag1 - 1].iAborted = 1;
			if (edceElevator[self->iTag1 - 1].iStatus == EDElevator::EDEL_ABORTED) {
				edceElevator[self->iTag1 - 1].iAborted = 0;
				std::stringstream converter;
				converter << "Attempting to recover elevator #" << self->iTag1;
				EDLogClass.edlgRecordLog(EDLogClass.edDispatcher, EDLogClass.edINFO, converter.str());

			}
			else {
				std::stringstream converter;
				converter << "Attempting to abort elevator #" << self->iTag1;
				EDLogClass.edlgRecordLog(EDLogClass.edDispatcher, EDLogClass.edINFO, converter.str());
			}
		}else if(self->iTag2 == 300) {
			if (edceElevator[self->iTag1 - 1].iStatus == EDElevator::EDEL_WAITING) {
				edceElevator[self->iTag1 - 1].iRemainingTimeSlice -= EDGlobal::EDGL_CLOSE_BOOST_TIME;
				if (edceElevator[self->iTag1 - 1].iRemainingTimeSlice <= 0) {
					edceElevator[self->iTag1 - 1].iRemainingTimeSlice = 1;
				}
			}
		}
		else {
			if (edceElevator[self->iTag1 - 1].iStatus == EDElevator::EDEL_FREE ||
				edceElevator[self->iTag1 - 1].iStatus == EDElevator::EDEL_WAITING) {
				if (edceElevator[self->iTag1 - 1].iStatus == EDElevator::EDEL_FREE) {
					edceElevator[self->iTag1 - 1].iDirection = EDElevator::EDEL_FREE;
					edceElevator[self->iTag1 - 1].iRemainingTimeSlice = EDGlobal::EDGL_WAITING_TIME;
					edceElevator[self->iTag1 - 1].iStatus = EDElevator::EDEL_WAITING;

				}
				else {
					edceElevator[self->iTag1 - 1].iOpeningBoostTime = EDGlobal::EDGL_OPEN_EXTRA_TIME;
				}
			}
		}
	}
	EDGlobals.EDGL_Mutex.unlock();
	return 0;
}

void EDMain::edmGLMouseKeyHandler(int button, int state, int x, int y) {
	std::stringstream converter;
	converter << "Mouse Click: x=" << x << ",y=" << y;
	if (state == 1) {
		edcm->edcmActivateMouseClickEvent(x, static_cast<int>(EDCM_GLUTORTHO_H) - y);
		//EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edINFO, converter.str());
	}

}
void EDMain::edmGLMouseMotionHandler(int x, int y) {
	edcm->edcmActivateMouseMoveEvent(x, static_cast<int>(EDCM_GLUTORTHO_H) - y);
	
}

void EDMain::edmGLResizeHandler(int w, int h) {
	//Force Resizing
	if (w == static_cast<int>(EDCM_GLUTORTHO_W) && h == static_cast<int>(EDCM_GLUTORTHO_H)) {
		return;
	}
	glutReshapeWindow(static_cast<int>(EDCM_GLUTORTHO_W), static_cast<int>(EDCM_GLUTORTHO_H));

	std::stringstream converter;
	converter << "Reshaping Scene - " << w << "," << h;
	EDLogClass.edlgRecordLog(EDLogClass.edGLUTController, EDLogClass.edINFO, converter.str());

}


//Initialization of static variables

EDControlManager* EDMain::edcm = nullptr;
EDTimerController* EDMain::edtc = nullptr;
int EDMain::edmgUIInitStatus = 0;
const char* EDMain::EDCM_TITLE = "Elevator Dispatch";
const float EDMain::EDCM_GLUTORTHO_H = 1000;
const float EDMain::EDCM_GLUTORTHO_W = 1440;

int EDMain::edmgUICurrentWindowH = 1000;
int EDMain::edmgUICurrentWindowW = 1440;
int EDMain::EDCM_GLUTANI_INTERVAL = 15;

const int EDMain::ED_MAIN_ELEVATOR_PANEL_X[] = { 340,870,340,870,340,870 };
const int EDMain::ED_MAIN_ELEVATOR_PANEL_Y[] = { 650,650,350,350,50,50 };
EDInteractiveControl_PanelBack* EDMain::edcpElevatorPanelBack = nullptr;
EDInteractiveControl_PanelBack* EDMain::edcpElevatorPanelBackShadow = nullptr;
EDInteractiveControl_PanelBack* EDMain::edcpElevatorFloorPanelBack = nullptr;
EDInteractiveControl_PanelBack* EDMain::edcpElevatorFloorPanelBackShadow = nullptr;
EDInteractiveControl_RoundButton* EDMain::edcpElevatorFloorPanelUpBtn = nullptr;
EDInteractiveControl_RoundButton* EDMain::edcpElevatorFloorPanelDwBtn = nullptr;
EDInteractiveControl_Label* EDMain::edcpElevatorFloorPanelLabel = nullptr;
EDInteractiveControl_RoundButton** EDMain::edcpElevatorCarPanelNormalBtn = nullptr;
EDInteractiveControl_Label* EDMain::edcpElevatorCarPanelLED = nullptr;
EDInteractiveControl_Label* EDMain::edcpElevatorCarPanelStatusBar = nullptr;
EDInteractiveControl_ArrowIndicator* EDMain::edcpElevatorCarUpIndicator = nullptr;
EDInteractiveControl_ArrowIndicator* EDMain::edcpElevatorCarDwIndicator = nullptr;


EDInteractiveControl_RoundButton* EDMain::edcpElevatorCarPanelAlarmBtn = nullptr;
EDInteractiveControl_RoundButton* EDMain::edcpElevatorCarPanelOpenBtn = nullptr;
EDInteractiveControl_RoundButton* EDMain::edcpElevatorCarPanelCloseBtn = nullptr;

EDInteractiveControl_Label* EDMain::edcpTitle = nullptr;
EDInteractiveControl_Label* EDMain::edcpAuthor = nullptr;

std::thread* EDMain::edmtUIThread = nullptr;
std::thread* EDMain::edmtCONTestThread = nullptr;

EDTimer** EDMain::edmtElevatorThreadPtr = nullptr;
EDTimer* EDMain::edmtExternalDispatchThread = nullptr;

EDElevator* EDMain::edceElevator = nullptr;
int* EDMain::edtcArgvBuffer = nullptr;

EDElevatorRequest* EDMain::ederList = nullptr;

EDInteractiveControl_CircularProgressBar* EDMain::edcpTimeLeftIndicator = nullptr;

EDInteractiveControl_Button* EDMain::edcpLowQualityMode = nullptr;
EDInteractiveControl_Label* EDMain::edcpElevatorLabels = nullptr;