#include "csmaControl.h"

Define_Module(csmaControl);

// ----------------- Public member functions ----------------------------------

void csmaControl::initialize(int stage) {
	bool startsOn = getParentModule()->par("startsOn").boolValue();
	if(startsOn) {
	    status = IControllable::TURNED_ON;
		csma::initialize(stage);
	} else {
	    status = IControllable::TURNED_OFF;
		csma::initialize(stage);
	}
}

// ----------------- Protected member functions -------------------------------

bool csmaControl::turnOn()
{

    if (isOn()) {
        throw cRuntimeError("Trying to turn ON interface while already ON");
        return false;
    }
    if (internalState == WAITING_FOR_PHY_ON) {
        throw cRuntimeError("Received TURN_ON while waiting for phy to become ready");
        return false;
    }
	status = IControllable::TURNED_ON;
	////////////////////////////
	//TODO
	resetCSMA();
	//////////////////////////
	ev << "csmaControl is now TURNED_ON" << endl;
	return true;
}

bool csmaControl::turnOff()
{
    if(status == IControllable::TURNED_OFF) {
        throw cRuntimeError("Trying to turn OFF while already OFF");
        return false;
    }

    status = IControllable::TURNED_OFF;
    ev << "csmaControl is now TURNED_OFF" << endl;
    return true;
}

void csmaControl::handleUpperMsg(cMessage *msg) {
	if (status == IControllable::TURNED_OFF || status == IControllable::SLEEP ) {
		ev << "Dropped packet, nic not on" << endl;
	}
	else {
		csma::handleUpperMsg(msg);
	}
}

void csmaControl::handleUpperControl(cMessage* msg) {
	ev<<"csmaControl received upper control "<<msg->getName()<<endl;
    switch(msg->getKind()) {
	case (IControllable::TURN_ON):
	{
	    internalState = WAITING_FOR_PHY_ON;
		sendControlDown(msg);
	}
	break;
	case (IControllable::TURN_OFF):
	{
		internalState = WAITING_FOR_PHY_OFF;
		sendControlDown(msg);
	}
	break;
	case (IControllable::SLEEP):
	{
		internalState = WAITING_FOR_PHY_SLEEP;
		sendControlDown(msg);
	}
	break;
	case (IControllable::WAKE_UP):
    {
		msg->setKind(IControllable::TURN_ON);
        internalState = WAITING_FOR_PHY_ON;
        sendControlDown(msg);
    }
    break;
	default:
		throw cRuntimeError("Received unknown control message from upper layer!");
	}
}

void csmaControl::handleLowerControl(cMessage *msg) {
    ev<<"csmaControl received lower control "<<msg->getName()<<endl;
	switch(msg->getKind()) {
		case (IControllable::TURNED_ON):
			if (internalState != WAITING_FOR_PHY_ON) {
				throw cRuntimeError("Received TURNED_ON from PHY but not WAITING_FOR_PHY_ON");
			}
			internalState = IDLE;
			if(turnOn() == false) {
				throw cRuntimeError("Could not turn on csma");
			}
			sendControlUp(msg);
			break;
		case (IControllable::TURNED_OFF):
			if (internalState != WAITING_FOR_PHY_OFF) {
				throw cRuntimeError("Received TURNED_OFF from PHY but not WAITING_FOR_PHY_OFF");
			}
			if(turnOff() == false) {
				throw cRuntimeError("Could not turn off csma");
			}
			internalState = IDLE;
			sendControlUp(msg);
			break;
		case (IControllable::SLEEPING):
			if (internalState != WAITING_FOR_PHY_SLEEP) {
				throw cRuntimeError("Received SLEEP from PHY but not WAITING_FOR_PHY_SLEEP");
			}
			if(sleep() == false) {
				throw cRuntimeError("Could not sleep csma");
			}
			internalState = IDLE;
			sendControlUp(msg);
			break;
		default:
			csma::handleLowerControl(msg);
			break;
	}
}

void csmaControl::handleLowerMsg(cMessage *msg)
{
    if (status != IControllable::TURNED_ON) {
        throw cRuntimeError("Received message %s from PHY while not ON", msg->getFullName());
    }
    csma::handleLowerMsg(msg);
}

bool csmaControl::sleep()
{
	ev << "csmaControl Received a sleep" << endl;
	if(status == IControllable::TURNED_OFF) {
		throw cRuntimeError("Trying to sleep while OFF");
		return false;
	}
	else if(status == IControllable::SLEEPING) {
		throw cRuntimeError("Trying to sleep while already asleep");
		return false;
	}

	status = IControllable::SLEEPING;
	ev << "csmaControl is now SLEEPING" << endl;
	return true;
}

bool csmaControl::wakeUp()
{
    ev << "csmaControl Received a wakeup";
    if(status == IControllable::TURNED_OFF) {
		throw cRuntimeError("Trying to wake up while OFF");
		return false;
	}
	else if(status == IControllable::TURNED_ON) {
		throw cRuntimeError("Trying to wake up while already awake");
		return false;
	}

	status = IControllable::TURNED_ON;
	////////////////////////////
	//TODO
	resetCSMA();
	//////////////////////////
	ev << "csmaControl is now TURNED_ON" << endl;
	return true;
}

void csmaControl::resetCSMA() {
		cancelEvent(backoffTimer);
		cancelEvent(ccaTimer);
		cancelEvent(sifsTimer);
		cancelEvent(rxAckTimer);
		if (ackMessage)
			delete ackMessage;
		MacQueue::iterator it;
		for (it = macQueue.begin(); it != macQueue.end(); ++it) {
			delete (*it);
		}
		macQueue.clear();
		updateMacState(IDLE_1);
}
