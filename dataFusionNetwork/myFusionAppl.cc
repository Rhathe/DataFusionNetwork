/***************************************************************************
 * file:        fusionAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "myFusionAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>

Define_Module(myFusionAppl);

void myFusionAppl::initialize(int stage) {
	fusionAppl::initialize(stage);
	timeout = 0;
	if(stage == 0) {
		controlTimer = new cMessage( "control-timer", WAKEUP );
	}
}


void myFusionAppl::handleLowerMsg( cMessage* msg )
{
	wsnPkt *m = static_cast<wsnPkt *>(msg);
	++received;
	if (checkAddr(msg) == false) {
		if (msg->getKind() == REQUEST_FOR_DATA) {
			recordRoundPower();
		}
		delete msg;
		return;
	}

	myData dummyData;
	std::vector<int> blah(2,5);
	dummyData.order = blah;

	std::string s("SEND_BASE_ADDRESS: ");
	dummyData.test = s;

	m->setData(dummyData);

	restTime = timeout - (simTime() - msg->getTimestamp());

	switch( msg->getKind() ){
		case BROADCAST_BASE_ADDRESS:
			EV << "Received Address from Base Station["<<m->getSrcAddr()<<"]\n";
			baseStationAddr = m->getSrcAddr();

			timeout = m->getTimeout();
			strAppend(s, myApplAddr());

			sendToBase(s.c_str(), SEND_BASE_OWN_ADDRESS);
			delete msg;
			break;
		case REQUEST_FOR_DATA:
			EV << "Received Request from Base Station["<<m->getSrcAddr()<<"]\n";
			recordRoundPower();
			MACrand = intuniform(0,99);

			if (MACrand <= threshOfAttack) {
				sendToBase("WRONG_FUSION_DATA", WRONG_FUSION_DATA);
			}
			else {
				sendToBase("RIGHT_FUSION_DATA", RIGHT_FUSION_DATA);
			}

			sleep(timeout);

			delete msg;
			break;
		case REQUEST_FOR_DATA_REPEAT:
			EV << "Received Request from Base Station["<<m->getSrcAddr()<<"]\n";

			if (MACrand <= threshOfAttack) {
				sendToBase("WRONG_FUSION_DATA", WRONG_FUSION_DATA);
			}
			else {
				sendToBase("RIGHT_FUSION_DATA", RIGHT_FUSION_DATA);
			}

			delete msg;
			break;
		case REQUEST_FOR_VOTES:
			EV << "Received Request for vote from Base Station["<<m->getSrcAddr()<<"]\n";
			MACrand = intuniform(0,99);

			if (MACrand <= threshOfAttack && m->getMACtype() == RIGHT_MAC) {
				sendToBase("WRONG_MAC", WRONG_MAC);
				++wrongVOTEsent;
			}
			else if (MACrand > threshOfAttack && m->getMACtype() == WRONG_MAC) {
				sendToBase("RIGHT_MAC", RIGHT_MAC);
				++rightVOTEsent;
			}
			else {
				;sleep(restTime);
			}

			delete msg;
			break;
		default:
			EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
			delete msg;
	}
}/**/

/*
void myFusionAppl::handleLowerMsg( cMessage* msg )
{
	wsnPkt *m = static_cast<wsnPkt *>(msg);
	++received;
	if (checkAddr(msg) == false) {
		delete msg;
		return;
	}

	std::string s("SEND_BASE_ADDRESS: ");
	restTime = timeout - (simTime() - msg->getTimestamp());

	switch( msg->getKind() ){
		case BROADCAST_BASE_ADDRESS:
			EV << "Received Address from Base Station["<<m->getSrcAddr()<<"]\n";
			baseStationAddr = m->getSrcAddr();

			timeout = m->getTimeout();
			strAppend(s, myApplAddr());

			sendToBase(s.c_str(), SEND_BASE_OWN_ADDRESS);
			delete msg;
			break;
		case REQUEST_FOR_DATA:
			EV << "Received Request from Base Station["<<m->getSrcAddr()<<"]\n";
			MACrand = intuniform(0,99);

			if (MACrand <= threshOfAttack) {
				sendToBase("WRONG_FUSION_DATA", WRONG_FUSION_DATA);
			}
			else {
				sendToBase("RIGHT_FUSION_DATA", RIGHT_FUSION_DATA);
			}

			canVote = false;
			delete msg;
			break;
		case REQUEST_FOR_DATA_REPEAT:
			EV << "Received Request from Base Station["<<m->getSrcAddr()<<"]\n";

			if (MACrand <= threshOfAttack) {
				sendToBase("WRONG_FUSION_DATA", WRONG_FUSION_DATA);
			}
			else {
				sendToBase("RIGHT_FUSION_DATA", RIGHT_FUSION_DATA);
			}

			delete msg;
			break;
		case REQUEST_FOR_VOTES:
			EV << "Received Request for vote from Base Station["<<m->getSrcAddr()<<"]\n";

			if (canVote == true) {
				MACrand = intuniform(0,99);

				if (MACrand <= threshOfAttack && m->getMACtype() == RIGHT_MAC) {
					sendToBase("WRONG_MAC", WRONG_MAC);
					++wrongVOTEsent;
				}
				else if (MACrand > threshOfAttack && m->getMACtype() == WRONG_MAC) {
					sendToBase("RIGHT_MAC", RIGHT_MAC);
					++rightVOTEsent;
				}
			}
			canVote = true;

			delete msg;
			break;
		default:
			EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
			delete msg;
	}
}/**/

void myFusionAppl::handleSelfMsg(cMessage *msg) {

	myAppl::handleSelfMsg(msg);

	switch( msg->getKind() ){
		case RECORD_POWER:
			break;
		case WAKEUP:
			wakeUp();
			break;
		default:
			EV << "Unknown selfmessage! -> delete, kind: "<<msg->getKind() <<endl;
			delete msg;
	}
}

void myFusionAppl::sleep(simtime_t time) {
	if (time > 0.00001) {
		fusionAppl::sleep();
		scheduleAt(simTime() + time, controlTimer);
	}
}

myFusionAppl::~myFusionAppl()
{
	cancelAndDelete(delayTimer);
}
