#pragma once
#include<string>
#include<vector>
#include "EDGlobal.h"
#include "EDLog.h"
#include<sstream>

class EDElevator;

//External request
class EDElevatorRequest {
public:
	int iRequestFloor;
	int iRequestDirection;
	int iRequestCreateTime;
	int iRequestRecipients; // Indicates which elevator occupies the request
	int iRequestValid; //0-invalid, 1-invalid
	int iReallocateTime; // Remaining time slice
	int iRedispatchFlag;
	int iPostponeTime;
	void ederCreateRequest(int iFloor, int iDirection, int iCreateTime);
	int ederCalculatePriority(const EDElevator& el);
};

class EDElevator
{
public:
	static EDElevatorRequest* vExternalList;

	const static int EDEL_ASCENDING = 1;
	const static int EDEL_DESCENDING = 2;
	const static int EDEL_FREE = 3;
	const static int EDEL_WAITING = 4;
	const static int EDEL_ABORTED = 5;
	const static int EDEL_NON_DIRECTION = 0;

	int iInternalRequests[EDGlobal::EDGL_FLOORS + 1];
	int iExternalRequests[EDGlobal::EDGL_FLOORS + 1];

	int iRemainingTimeSlice = 0;
	int iCurFloor = 1;
	int iDirection = EDEL_FREE;
	int iStatus = EDEL_FREE;
	int iAborted = 0;
	int iOpeningBoostTime = 0;	//Time boosts for pressing down opening-door button
	int iID;

	std::string edelConvertFloor2Str();
	std::string edelConvertStatus2Str();

	int edelGetStatusMaxTime();

	void edelAscend();
	void edelDescend();
	void edelFree();
	void edelInitialize();
	void edelEnteringWaiting();

	//Estimate the time if the new external request is dispatched to this elevator
	int edelEstimateTime(int floor, int direction)const;

	//Check if the elevator has external request that can be satisfied just now
	static bool edelCheckExtArrival(EDElevator* p, bool bCancel = false);
	static bool edelCheckExtArrivalAbove(EDElevator* p, bool bCancel = false);
	static bool edelCheckExtArrivalBelow(EDElevator* p, bool bCancel = false);

	//Check if the elevator has ascending request
	static bool edelCheckExtArrivalUW(EDElevator* p, bool bCancel = false);
	//Check if the elevator has descending request
	static bool edelCheckExtArrivalDW(EDElevator* p, bool bCancel = false);
	//Check if the elevator has ascending request on the current floor
	static bool edelCheckExtArrivalEUW(EDElevator* p, bool bCancel = false);
	//Check if the elevator has descending request on the current floor
	static bool edelCheckExtArrivalEDW(EDElevator* p, bool bCancel = false); 
	
	//[IMPORTANT] Dispatcher for exclusive requests and internal requests
	static void edelDispatcher(int argc, const std::vector<void*>& argv);

	EDElevator();

};

