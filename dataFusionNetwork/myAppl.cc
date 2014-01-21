/***************************************************************************
 * file:        myAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "myAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>

Define_Module(myAppl);

double myAppl::completeTime;
double myAppl::beginVotingTime;
int myAppl::numRounds;

void myAppl::initialize(int stage)
{
    BaseModuleInit(stage);
    otherInit(stage);

    if (stage == 0) {
		cModule* host = findHost();
		if(host == 0) {
			throw cRuntimeError("Host pointer is NULL in myAppl");
		}

		cModule* tempBattery = host->getSubmodule("battery");
		battery = static_cast<myBattery *>(tempBattery);

		if(battery == 0) {
			throw cRuntimeError("Could not find module 'battery' in myAppl");
		}
		cModule* nic = host->getSubmodule("nic");
		if(nic == 0) {
			throw cRuntimeError("Could not find module 'nic' in myAppl");
		}
		cModule* nicController = nic->getSubmodule("nicController");
		if (nicController != 0) {
			controller = check_and_cast<NicController*>(nicController);
			if(controller == 0) {
				throw cRuntimeError("Could not find module nic 'nicController' in myAppl");
			}
			nicId = controller->getNicId();
		} else {
			controller = 0;
			nicId = -1;
		}

		// Subscribe to nic state changes.
		NicStateNotify stateChangeInfo;
		catNicStateNotify = utility->subscribe(this, &stateChangeInfo, host->getId());

		// Subscribe to phy state changes.
		sentMsgNotify sentMsgInfo;
		catPhyStateNotify = utility->subscribe(this, &sentMsgInfo, host->getId());

		// Obtain category for publishing nic commands on
		NicCommandNotify nicCommandNotify;
		catNicCommandNotify = utility->getCategory(&nicCommandNotify);

		if (nic->par("startsOn"))
			nicState = IControllable::TURNED_ON;
		else
			nicState = IControllable::TURNED_OFF;

		delayedControlPresent = false;
		sentMsgs = 0;
    }

    if(stage == 0) {
		powerTimer = new cMessage( "power-timer", RECORD_POWER );
		powerVector.setName("Total Power");
		powerPerRoundVector.setName("Power Per Round");
		totalPower = 0;
		rxPower = 0;
		txPower = 0;
		pastPower = 0;
	}
	else if(stage==1) {
		scheduleAt(simTime() + 1.0, powerTimer);
	}
}

bool myAppl::checkAddr(cMessage* msg) {
	wsnPkt *m = static_cast<wsnPkt *>(msg);

	int dest = m->getDestAddr();

	if (dest == -1 || dest == myApplAddr())
		return true;
	else
		return false;
}

void myAppl::handleLowerMsg( cMessage* msg ) {

}

void myAppl::handleSelfMsg(cMessage *msg) {
	switch( msg->getKind() ){
		case RECORD_POWER:
			std::vector<double> dummy = battery->recordStuff();
			totalPower = dummy[0];
			rxPower = dummy[2];
			txPower = dummy[3];
			powerVector.recordWithTimestamp(simTime(), totalPower);
			scheduleAt(simTime()+1.0, powerTimer);
			break;
	}
}


wsnPkt *myAppl::createPkt(const char *messageID=NULL, int messageType=0, int dest=0) {
	wsnPkt *pkt = new wsnPkt(messageID, messageType);

	pkt->setDestAddr(dest);
	pkt->setSrcAddr( myApplAddr() );

	pkt->setBitLength(headerLength);

	pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );

	switch(messageType) {
		case SEND_BASE_OWN_ADDRESS:
		case BROADCAST_BASE_ADDRESS:
		case REQUEST_FOR_DATA:
		case REQUEST_FOR_DATA_REPEAT:
		case REQUEST_FOR_MAC:
			pkt->addByteLength(1);
			break;
		case RIGHT_FUSION_DATA:
		case WRONG_FUSION_DATA:
			pkt->addByteLength(1024);
			break;
		case RIGHT_MAC:
		case WRONG_MAC:
		case REQUEST_FOR_VOTES:
			pkt->addByteLength(64);
			break;
	}

	return pkt;
}

void myAppl::strAppend(std::string &str, int x) {
	std::stringstream oss;
	oss << x;
	str += oss.str();
}

void myAppl::sendDown(cMessage* msg) {
	msg->setTimestamp();
	BaseLayer::sendDown(msg);
	++sentMsgs;
}

void myAppl::BaseModuleInit(int stage) {
    if (stage == 0) {
    	notAffectedByHostState = 	hasPar("notAffectedByHostState")
								 && par("notAffectedByHostState").boolValue();

        hasPar("debug") ? debug = par("debug").boolValue() : debug = false;
        utility = FindModule<BaseUtility*>::findSubModule(findHost());

        if(!utility) {
        	error("No BaseUtility module found!");
        }

        hostId = findHost()->getId();
    }
}

void myAppl::otherInit(int stage) {
	if(stage==0){
		if (hasPar("stats") && par("stats").boolValue()) {
			doStats = true;
			passedMsg = new PassedMessage();
			catPassedMsg = utility->getCategory(passedMsg);
			passedMsg->fromModule = getId();
			hostId = findHost()->getId();
		}
		else {
			doStats = false;
		}

		upperGateIn  = findGate("upperGateIn");
		upperGateOut = findGate("upperGateOut");
		lowerGateIn  = findGate("lowerGateIn");
		lowerGateOut = findGate("lowerGateOut");
		upperControlIn  = findGate("upperControlIn");
		upperControlOut = findGate("upperControlOut");
		lowerControlIn  = findGate("lowerControlIn");
		lowerControlOut = findGate("lowerControlOut");

		headerLength= par("headerLength");
	}
}

void myAppl::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
    if (category == catNicStateNotify) {
        NicStateNotify* notify = check_and_cast<NicStateNotify*>(details);
        if (nicId != -1 && notify->nicId == nicId) {
            nicStateUpdate(notify->status);
        } else {
            throw cRuntimeError("NIC status notify on unknown nicId %d", notify->nicId);
        }
    }
    if (category == catPhyStateNotify) {
    	--sentMsgs;

    	if (sentMsgs <= 0 && delayedControlPresent == true) {
    		sendXLControl(delayedControl);
    	}
    }
    BaseModule::receiveBBItem(category, details, scopeModuleId);
}

void myAppl::sendXLControl(IControllable::Controls command)
{
	if (sentMsgs > 0) {
		delayedControl = command;
		delayedControlPresent = true;
	}
	else {
		delayedControlPresent = false;
		if (nicId == -1) {
			throw cRuntimeError("Trying to control primary nic but it is not controllable");
		}
		NicCommandNotify nicCommandNotify;
		nicCommandNotify.command = command;
		nicCommandNotify.nicId = nicId;
		utility->publishBBItem(catNicCommandNotify, &nicCommandNotify, findHost()->getId());
	}
}

void myAppl::turnOn() {
	sendXLControl(IControllable::TURN_ON);
}

void myAppl::turnOff() {
	sendXLControl(IControllable::TURN_OFF);
}

void myAppl::sleep() {
	sendXLControl(IControllable::SLEEP);
}

void myAppl::wakeUp() {
	sendXLControl(IControllable::WAKE_UP);
}

void myAppl::nicStateUpdate(IControllable::Status newState)
{
    ev << "host" << myApplAddr() << " nic state changed to ";
    nicState = newState;
    switch( newState ){
    case IControllable::TURNED_ON:
        ev<<"TURNED_ON"<<endl;
        break;
    case IControllable::TURNED_OFF:
        ev<<"TURNED_OFF"<<endl;
        break;
    case IControllable::SLEEPING:
        ev<<"SLEEPING"<<endl;
        break;
    default:
        throw cRuntimeError("Unknown state %d received in nicStateUpdate", newState);
    }
}

void myAppl::recordRoundPower() {
	std::vector<double> dummy = battery->recordStuff();
	totalPower = dummy[0];
	rxPower = dummy[2];
	txPower = dummy[3];
	powerPerRoundVector.recordWithTimestamp(simTime(), totalPower-pastPower);
	pastPower = totalPower;
}

myAppl::~myAppl()
{
	cancelAndDelete(delayTimer);
}
