#include "EDElevator.h"

void EDElevatorRequest::ederCreateRequest(int iFloor, int iDirection, int iCreateTime) {
	iRequestFloor = iFloor;
	iRequestDirection = iDirection;
	iRequestCreateTime = iCreateTime;
}

std::string EDElevator::edelConvertFloor2Str() {
	std::stringstream oss;
	if (iCurFloor < 10) {
		oss << "0";
	}

	oss << iCurFloor;
	return oss.str();
}

std::string EDElevator::edelConvertStatus2Str() {
	std::stringstream converter;
	switch (iStatus) {
		case EDEL_WAITING:
			return "WAIT";
			break;
		case EDEL_ASCENDING:
			return "BUSY";
			break;
		case EDEL_DESCENDING:
			return "BUSY";
			break;
		case EDEL_FREE:
			return "FREE";
			break;
		case EDEL_ABORTED:
			return "STOP";
			break;
		default:
			converter << "Entering Unknown Status" << iStatus << "- ElevId=" << iID;
			EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edWARNING,converter.str() );
			return "----";
			break;
	}
}

void EDElevator::edelAscend() {
	iDirection = EDEL_ASCENDING;
	iCurFloor++;
	if (iCurFloor == 21) {
		iCurFloor = 1;
	}
}

int EDElevator::edelGetStatusMaxTime() {
	switch (iStatus) {
		case EDEL_FREE:
			return 999999;
		case EDEL_ABORTED:
			return 999999;
		case EDEL_ASCENDING:
			return 999999;
			//return EDGlobal::EDGL_ASCENDING_TIME;
		case EDEL_DESCENDING:
			return 999999;
			//return EDGlobal::EDGL_DESCENDING_TIME;
		case EDEL_WAITING:
			return EDGlobal::EDGL_WAITING_TIME;
	}
	return 99999;
}

void EDElevator::edelDescend() {
	iDirection = EDEL_DESCENDING;
	iCurFloor--;
	if (iCurFloor == 0) {
		iCurFloor = 20;
	}
}

void EDElevator::edelInitialize() {
	for (int i = 0; i <= EDGlobal::EDGL_FLOORS; i++) {
		iInternalRequests[i] = 0;
		iExternalRequests[i] = 0;
	}
}

void EDElevator::edelFree() {
	iDirection = EDEL_FREE;
}
bool EDElevator::edelCheckExtArrival(EDElevator* p, bool bCancel) {
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor == EDElevator::vExternalList[i].iRequestFloor) {
			if (p->iDirection == EDElevator::vExternalList[i].iRequestDirection || p->iDirection == EDEL_FREE) {
				if (bCancel) {
					EDGlobals.EDGL_Mutex.lock();
					EDElevator::vExternalList[i].iRequestValid = 0;
					EDElevator::vExternalList[i].iRequestRecipients = 0;
					EDElevator::vExternalList[i].iRedispatchFlag = 0;
					EDElevator::vExternalList[i].iReallocateTime = 0;
					std::stringstream oss;
					oss << "External task #" << i << " has been accomplished by elevator #" << p->iID;
					EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());
					EDGlobals.EDGL_Mutex.unlock();
				}
				return true;
			}
		}
	}
	return false;
}
void EDElevator::edelEnteringWaiting() {
	this->iStatus = EDEL_WAITING;
	this->iRemainingTimeSlice = EDGlobal::EDGL_WAITING_TIME;
	edelCheckExtArrival(this, true);
}
bool EDElevator::edelCheckExtArrivalAbove(EDElevator* p, bool bCancel) {
	for (int i = p->iCurFloor + 1; i <= EDGlobal::EDGL_FLOORS; i++) {
		if (p->iInternalRequests[i])
			return true;
	}
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor < EDElevator::vExternalList[i].iRequestFloor) {
			return true;
		}
	}
	return false;
}

bool EDElevator::edelCheckExtArrivalBelow(EDElevator* p, bool bCancel) {
	for (int i = 0; i < p->iCurFloor; i++) {
		if (p->iInternalRequests[i])
			return true;
	}
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor > EDElevator::vExternalList[i].iRequestFloor) {
			return true;
		}
	}
	return false;
}

bool EDElevator::edelCheckExtArrivalUW(EDElevator* p, bool bCancel) {
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor < EDElevator::vExternalList[i].iRequestFloor) {
			if (p->iDirection == EDElevator::vExternalList[i].iRequestDirection || p->iDirection == EDEL_FREE) {
				return true;
			}
		}
	}
	return false;
}
bool EDElevator::edelCheckExtArrivalDW(EDElevator* p, bool bCancel) {
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor > EDElevator::vExternalList[i].iRequestFloor) {
			if (p->iDirection == EDElevator::vExternalList[i].iRequestDirection || p->iDirection == EDEL_FREE) {
				return true;
			}
		}
	}
	return false;
}
bool EDElevator::edelCheckExtArrivalEUW(EDElevator* p, bool bCancel) {
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor == EDElevator::vExternalList[i].iRequestFloor) {
			if (EDElevator::EDEL_ASCENDING == EDElevator::vExternalList[i].iRequestDirection && p->iDirection == EDEL_FREE) {
				if (bCancel) {
					EDGlobals.EDGL_Mutex.lock();
					EDElevator::vExternalList[i].iRequestValid = 0;
					EDElevator::vExternalList[i].iRequestRecipients = 0;
					EDElevator::vExternalList[i].iRedispatchFlag = 0;
					EDElevator::vExternalList[i].iReallocateTime = 0;
					EDGlobals.EDGL_Mutex.unlock();
				}
				return true;
			}
		}
	}
	return false;
}
bool EDElevator::edelCheckExtArrivalEDW(EDElevator* p, bool bCancel) {
	for (int i = 0; i < EDGlobal::EDGL_FLOORS * 2; i++) {
		if (p->iID != EDElevator::vExternalList[i].iRequestRecipients)continue;
		if (EDElevator::vExternalList[i].iRequestValid == 0)continue;
		if (p->iCurFloor == EDElevator::vExternalList[i].iRequestFloor) {
			if (EDElevator::EDEL_DESCENDING == EDElevator::vExternalList[i].iRequestDirection && p->iDirection == EDEL_FREE) {
				if (bCancel) {
					EDGlobals.EDGL_Mutex.lock();
					EDElevator::vExternalList[i].iRequestValid = 0;
					EDElevator::vExternalList[i].iRequestRecipients = 0;
					EDElevator::vExternalList[i].iRedispatchFlag = 0;
					EDElevator::vExternalList[i].iReallocateTime = 0;
					EDGlobals.EDGL_Mutex.unlock();
				}
				return true;
			}
		}
	}
	return false;
}
void EDElevator::edelDispatcher(int argc, const std::vector<void*>& argv) {
	//EDLogClass.edlgRecordLog(EDLog::edDispatcher,EDLog::edINFO,"Dispatch Procedure Called");

	EDElevator* self = (EDElevator*)(argv[0]);
	//The function should be called in timer threads
	if (self->iRemainingTimeSlice > 0) {
		if (self->iOpeningBoostTime > 0) {
			self->iOpeningBoostTime--;
			self->iRemainingTimeSlice++;
			if (self->iRemainingTimeSlice > EDGlobal::EDGL_WAITING_TIME) {
				self->iRemainingTimeSlice = EDGlobal::EDGL_WAITING_TIME;
			}
		}
		else {
			self->iRemainingTimeSlice--;
		}
		
		return;
	}
	//if actions are finished, then dispatch the elevator

	if (self->iStatus == EDEL_ASCENDING) {
		self->edelAscend();
		bool bExt = edelCheckExtArrival(self);
		bool bHaveMore = edelCheckExtArrivalAbove(self);
		if (self->iInternalRequests[self->iCurFloor] == 0 && !bExt && bHaveMore && self->iAborted != 1) {
			self->iInternalRequests[self->iCurFloor] = 0;//Finishing Request
			self->iRemainingTimeSlice = EDGlobal::EDGL_ASCENDING_TIME;
			self->iStatus = EDEL_ASCENDING;

		}
		else {
			self->iInternalRequests[self->iCurFloor] = 0;//Finishing Request
			self->edelEnteringWaiting();
			if (!bHaveMore && !bExt) {
				self->iDirection = EDEL_DESCENDING;
				bool bExt2 = edelCheckExtArrival(self);
				if (!bExt2) {
					self->iDirection = EDEL_ASCENDING;
				}
			}
		}
	}
	else if (self->iStatus == EDEL_DESCENDING) {
		self->edelDescend();
		bool bExt = edelCheckExtArrival(self);
		bool bHaveMore = edelCheckExtArrivalBelow(self);

		if (self->iInternalRequests[self->iCurFloor] == 0 && !bExt && bHaveMore && self->iAborted != 1) {
			self->iInternalRequests[self->iCurFloor] = 0;//Finishing Request
			self->iRemainingTimeSlice = EDGlobal::EDGL_DESCENDING_TIME;
			self->iStatus = EDEL_DESCENDING;
		}
		else {
			self->iInternalRequests[self->iCurFloor] = 0;//Finishing Request
			self->edelEnteringWaiting();
			if (!bHaveMore && !bExt) {
				self->iDirection = EDEL_ASCENDING;
				bool bExt2 = edelCheckExtArrival(self);
				if (!bExt2) {
					self->iDirection = EDEL_DESCENDING;
				}
			}
		}
	}
	else if (self->iStatus == EDEL_WAITING) {
		edelCheckExtArrival(self, true);
		if (self->iAborted) {
			self->iDirection = EDEL_FREE;
			self->iStatus = EDEL_FREE;
			self->iRemainingTimeSlice = EDGlobal::EDGL_FREE_TIME;
			return;
		}
		if (self->iDirection == EDEL_ASCENDING) {
			self->iStatus = EDEL_ASCENDING;
			self->iRemainingTimeSlice = EDGlobal::EDGL_ASCENDING_TIME;
			bool bHasAscendingInternalReq = false;
			if (self->iCurFloor != EDGlobal::EDGL_FLOORS) {
				for (int i = self->iCurFloor + 1; i <= EDGlobal::EDGL_FLOORS; i++) {
					if (self->iInternalRequests[i] != 0) {
						bHasAscendingInternalReq = true;
					}
				}
			}
			bHasAscendingInternalReq |= edelCheckExtArrivalAbove(self);
			//Check if descending internal requests exist
			if (!bHasAscendingInternalReq) {
				
				bool bHasDescendingInternalReq = false;
				for (int i = 1; i < self->iCurFloor; i++) {
					if (self->iInternalRequests[i] != 0) {
						bHasDescendingInternalReq = true;
					}
				}
				bHasDescendingInternalReq |= edelCheckExtArrivalBelow(self);
				if (bHasDescendingInternalReq) {
					self->iDirection = EDEL_DESCENDING;
					self->iStatus = EDEL_DESCENDING;
					self->iRemainingTimeSlice = EDGlobal::EDGL_DESCENDING_TIME;
					edelCheckExtArrival(self, true);
				}
				else {
					self->iDirection = EDEL_FREE;
					self->iStatus = EDEL_FREE;
					self->iRemainingTimeSlice = EDGlobal::EDGL_FREE_TIME;
				}
			}
		}
		//if the elevator goes down originally
		else {
			self->iStatus = EDEL_DESCENDING;
			self->iRemainingTimeSlice = EDGlobal::EDGL_DESCENDING_TIME;
			bool bHasDescendingInternalReq = false;
			//Check if descending internal requests exist
			if (self->iCurFloor != 0) {
				for (int i = 1; i < self->iCurFloor; i++) {
					if (self->iInternalRequests[i] != 0) {
						bHasDescendingInternalReq = true;
					}
				}
			}
			bHasDescendingInternalReq = bHasDescendingInternalReq | edelCheckExtArrivalBelow(self);
			if (!bHasDescendingInternalReq) {
				bool bHasAscendingInternalReq = false;
				for (int i = self->iCurFloor + 1; i <= EDGlobal::EDGL_FLOORS; i++) {
					if (self->iInternalRequests[i] != 0) {
						bHasAscendingInternalReq = true;
					}
				}
				bHasAscendingInternalReq |= edelCheckExtArrivalAbove(self);

				if (bHasAscendingInternalReq) {
					self->iDirection = EDEL_ASCENDING;
					self->iStatus = EDEL_ASCENDING;
					self->iRemainingTimeSlice = EDGlobal::EDGL_ASCENDING_TIME;
					edelCheckExtArrival(self, true);
				}
				else {
					self->iDirection = EDEL_FREE;
					self->iStatus = EDEL_FREE;
					self->iRemainingTimeSlice = EDGlobal::EDGL_FREE_TIME;
				}
			}
		}
	}
	else if (self->iStatus == EDEL_FREE) {
		if (edelCheckExtArrivalUW(self)) {
			self->iDirection = EDEL_ASCENDING;
			self->iStatus = EDEL_ASCENDING;
			self->iRemainingTimeSlice = EDGlobal::EDGL_ASCENDING_TIME;
		}
		if (edelCheckExtArrivalDW(self)) {
			self->iDirection = EDEL_DESCENDING;
			self->iStatus = EDEL_DESCENDING;
			self->iRemainingTimeSlice = EDGlobal::EDGL_DESCENDING_TIME;
		}
		for (int i = 1; i < self->iCurFloor; i++) {
			if (self->iInternalRequests[i] != 0) {
				self->iDirection = EDEL_DESCENDING;
				self->iStatus = EDEL_DESCENDING;
				self->iRemainingTimeSlice = EDGlobal::EDGL_DESCENDING_TIME;
			}
		}
		for (int i = self->iCurFloor + 1; i <= EDGlobal::EDGL_FLOORS; i++) {
			if (self->iInternalRequests[i] != 0) {
				self->iDirection = EDEL_ASCENDING;
				self->iStatus = EDEL_ASCENDING;
				self->iRemainingTimeSlice = EDGlobal::EDGL_ASCENDING_TIME;
			}
		}
		if (edelCheckExtArrivalEUW(self)) {
			edelCheckExtArrivalEUW(self, true);
			self->iDirection = EDEL_ASCENDING;
			self->iStatus = EDEL_WAITING;
			self->iRemainingTimeSlice = EDGlobal::EDGL_WAITING_TIME;
		}
		else if (edelCheckExtArrivalEDW(self)) {
			edelCheckExtArrivalEDW(self, true);
			self->iDirection = EDEL_DESCENDING;
			self->iStatus = EDEL_WAITING;
			self->iRemainingTimeSlice = EDGlobal::EDGL_WAITING_TIME;
		}

		if (self->iAborted) {
			self->iStatus = EDEL_ABORTED;
			self->iDirection = EDEL_FREE;
			std::stringstream oss;
			oss << "Elevator #" << self->iID << " has been aborted";
			EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());
		}
		self->iInternalRequests[self->iCurFloor] = 0;
	}
	else if (self->iStatus == EDEL_ABORTED) {
		for (int i = 0; i <= EDGlobal::EDGL_FLOORS; i++) {
			self->iInternalRequests[i] = 0;
		}
		if (self->iAborted == 0) {
			self->iStatus = EDEL_FREE;
			self->iDirection = EDEL_FREE;
			std::stringstream oss;
			oss << "Elevator #" << self->iID << " has been recovered";
			EDLogClass.edlgRecordLog(EDLog::edDispatcher, EDLog::edINFO, oss.str());
		}
	}
}

int EDElevator::edelEstimateTime(int floor, int direction) const{
	int iWaitingTime = 0;
	int iBusyTime = 0;
	int iLimitFloor = iCurFloor + 1;
	int iLimitFloor2 = 0;
	int iAccepted = 0;
	if (this->iDirection == EDEL_ASCENDING ) {
		//Go Up to the Top Floor
		for (int i = this->iCurFloor + 1; i <= EDGlobal::EDGL_FLOORS;i++) {
			if (direction == EDEL_ASCENDING && i == floor) {
				iLimitFloor = i;
				iAccepted = 1;
				break;
			}
			if (iInternalRequests[i] == 1 ||
				(EDElevator::vExternalList[(i - 1) * 2].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2].iRequestValid == 1)) {
				iWaitingTime += EDGlobal::EDGL_WAITING_TIME;
				iLimitFloor = i;
			}
			if (iInternalRequests[i] == 1 ||
				(EDElevator::vExternalList[(i - 1) * 2 + 1].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2 + 1].iRequestValid == 1)) {
				iLimitFloor = i;
			}
			
		}
		iBusyTime += (iLimitFloor - this->iCurFloor) * EDGlobal::EDGL_ASCENDING_TIME;

		if (iAccepted != 1) {
			//Go Down to the Bottom Floor 
			for (int i = iLimitFloor; i >= 1; i--) {
				if (direction == EDEL_DESCENDING && i == floor) {
					iLimitFloor2 = i;
					iAccepted = 1;
					break;
				}
				if ((EDElevator::vExternalList[(i - 1) * 2 + 1].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2 + 1].iRequestValid == 1)) {
					iWaitingTime += EDGlobal::EDGL_WAITING_TIME;
					iLimitFloor2 = i;
					
				}
				if ((EDElevator::vExternalList[(i - 1) * 2 ].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2].iRequestValid == 1)) {
					iLimitFloor2 = i;

				}
			}
			iBusyTime += (iLimitFloor - iLimitFloor2) * EDGlobal::EDGL_DESCENDING_TIME;
			if (iAccepted != 1) {
				//Go Up to the dest Floor
				for (int i = iLimitFloor2; i < floor; i++)
				{
					if ((EDElevator::vExternalList[(i - 1) * 2 ].iRequestRecipients == this->iID &&
						EDElevator::vExternalList[(i - 1) * 2 ].iRequestValid == 1)) {
						iWaitingTime += EDGlobal::EDGL_WAITING_TIME;
					}
				}
				iBusyTime += (floor - iLimitFloor2) * EDGlobal::EDGL_ASCENDING_TIME;

			}
		}
	}
	else if(this->iDirection==EDEL_DESCENDING){

		//Go Down to the Bottom Floor
		for (int i = this->iCurFloor + 1; i >=1; i--) {
			if (direction == EDEL_DESCENDING && i == floor) {
				iLimitFloor = i;
				iAccepted = 1;
				break;
			}
			if (iInternalRequests[i] == 1 ||
				(EDElevator::vExternalList[(i - 1) * 2+1].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2+1].iRequestValid == 1)) {
				iWaitingTime += EDGlobal::EDGL_WAITING_TIME;
				iLimitFloor = i;
			}
			if (iInternalRequests[i] == 1 ||
				(EDElevator::vExternalList[(i - 1) * 2 ].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2 ].iRequestValid == 1)) {
				iLimitFloor = i;
			}
			
		}
		iBusyTime += -(iLimitFloor - this->iCurFloor) * EDGlobal::EDGL_DESCENDING_TIME;

		//Go Up to the Top Floor
		if (iAccepted != 1) {
			for (int i = iLimitFloor; i <= EDGlobal::EDGL_FLOORS; i++) {
				if (direction == EDEL_ASCENDING && i == floor) {
					iLimitFloor2 = i;
					iAccepted = 1;
					break;
				}
				if ((EDElevator::vExternalList[(i - 1) * 2].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2].iRequestValid == 1)) {
					iWaitingTime += EDGlobal::EDGL_WAITING_TIME;
					iLimitFloor2 = i;
				}
				if ((EDElevator::vExternalList[(i - 1) * 2+1].iRequestRecipients == this->iID &&
					EDElevator::vExternalList[(i - 1) * 2+1].iRequestValid == 1)) {
					iLimitFloor2 = i;
				}
				
			}
			iBusyTime += -(iLimitFloor - iLimitFloor2) * EDGlobal::EDGL_ASCENDING_TIME;
			//Go Down to the dest floor
			if (iAccepted != 1) {
				for (int i = iLimitFloor2; i > floor; i--)
				{
					if ((EDElevator::vExternalList[(i - 1) * 2 + 1].iRequestRecipients == this->iID &&
						EDElevator::vExternalList[(i - 1) * 2 + 1].iRequestValid == 1)) {
						iWaitingTime += EDGlobal::EDGL_WAITING_TIME;
					}
				}
				iBusyTime += (floor - iLimitFloor2) * EDGlobal::EDGL_ASCENDING_TIME;
			}
		}
	}
	else {
		iBusyTime += abs(this->iCurFloor - floor) * EDGlobal::EDGL_ASCENDING_TIME;
	}
	return iWaitingTime + iBusyTime;
}
int EDElevatorRequest::ederCalculatePriority(const EDElevator& el) {
	if (el.iAborted == 1 || el.iStatus == EDElevator::EDEL_ABORTED) {
		return -2;
	}
	if (el.iStatus != EDElevator::EDEL_FREE &&
		((el.iDirection == EDElevator::EDEL_ASCENDING && el.iCurFloor > iRequestFloor) ||
			(el.iDirection == EDElevator::EDEL_DESCENDING && el.iCurFloor < iRequestFloor))) {
		return -1;
	}
	else {
		return el.edelEstimateTime(this->iRequestFloor, this->iRequestDirection);
	}
}
EDElevator::EDElevator() {
	edelInitialize();
}

EDElevatorRequest* EDElevator::vExternalList = nullptr;