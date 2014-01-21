/***************************************************************************
 * file:        myStationAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "myStationAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>

Define_Module(myStationAppl);

void myStationAppl::handleLowerMsg( cMessage* msg )
{
	wsnPkt *m;
	++received;
	if (checkAddr(msg) == false) {
		delete msg;
		return;
	}

	switch( msg->getKind() ){
		case SEND_BASE_OWN_ADDRESS:
			m = delayTimeout(msg);
			EV << "Received address from host["<<m->getSrcAddr()<<"]; delete msg\n";
			fusionNodes.push_back(m->getSrcAddr());
			numNodes++;
			delete msg;
			break;
		case RIGHT_FUSION_DATA:
			if (firstData == true) {
				typeOfData = WAS_RIGHT;
				sendBroadcastVote(RIGHT_MAC);
			}
			else {
				timeout = 0.0001;
				increaseRightData();
			}
			firstData = false;
			m = delayTimeout(msg);
			timeout = 2.0;
			EV << "Received fusion data from host["<<m->getSrcAddr()<<"]; delete msg\n";
			delete msg;
			break;
		case WRONG_FUSION_DATA:
			if (firstData == true) {
				typeOfData = WAS_WRONG;
				sendBroadcastVote(WRONG_MAC);
			}
			else {
				timeout = 0.0001;
			}
			firstData = false;
			m = delayTimeout(msg);
			timeout = 2.0;
			EV << "Received fusion data from host["<<m->getSrcAddr()<<"]; delete msg\n";
			delete msg;
			break;
		case RIGHT_MAC:
			m = delayTimeout(msg);
			delayTimer->setKind(DATA_CONFIRM_TIMER);
			++negVotes;

			if (firstRightMAC == false) {
				firstRightMAC = true;
				rightNode = m->getSrcAddr();
			}
			else {
				if (negVotes > threshold) {
					delayTimer->setKind(DATA_REQ_REPEAT_TIMER);
					chosenNode = rightNode;
					++rightVotedIn;
				}
			}
			++rightVotes;
			delete msg;
			break;
		case WRONG_MAC:
			m = delayTimeout(msg);
			delayTimer->setKind(DATA_CONFIRM_TIMER);
			++negVotes;

			if (firstWrongMAC == false) {
				firstWrongMAC = true;
				wrongNode = m->getSrcAddr();
			}
			else {
				if (negVotes > threshold) {
					delayTimer->setKind(DATA_REQ_REPEAT_TIMER);
					chosenNode = wrongNode;
					++wrongVotedIn;
				}
			}
			++wrongVotes;
			delete msg;
			break;
		default:
			EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
			delete msg;
	}
}

void myStationAppl::handleSelfMsg(cMessage *msg) {

	myAppl::handleSelfMsg(msg);

	switch( msg->getKind() ){
		case RECORD_POWER:
			break;
		case SEND_BASE_ADDRESS_TIMER:
			sendBroadcastAddress();
			delayTimer->setKind(DATA_REQ_TIMER);
			scheduleAt(simTime()+timeout, delayTimer);
			break;
		case DATA_REQ_TIMER:
			chooseNode();
			delayTimer->setKind(DATA_CONFIRM_TIMER);
			scheduleAt(simTime()+timeout, delayTimer);
			break;
		case DATA_CONFIRM_TIMER:
			if (typeOfData == WAS_RIGHT) {
				increaseRightData();
			}
			delayTimer->setKind(DATA_REQ_TIMER);
			scheduleAt(simTime()+0.0001, delayTimer);
			break;
		case DATA_REQ_REPEAT_TIMER:
			requestRepeatFusionData();
			delayTimer->setKind(DATA_REQ_TIMER);
			scheduleAt(simTime()+timeout, delayTimer);
			break;
		default:
			EV << "Unknown selfmessage! -> delete, kind: "<<msg->getKind() <<endl;
			delete msg;
	}
}

void myStationAppl::chooseNode() {

	initializeRound();
	firstData = true;
	typeOfData = WAS_NEITHER;
	int k = intuniform(0, numNodes-1);
	chosenNode = fusionNodes[k];
	std::string str = "REQUEST_DATA_FROM_";
	strAppend(str, chosenNode);

	wsnPkt *pkt = createPkt(str.c_str(), REQUEST_FOR_DATA, chosenNode);
	EV << "Requesting Node " << chosenNode << " For its DATA!\n";
	sendDown( pkt );
}

void myStationAppl::requestRepeatFusionData() {

	++DATArequests;
	std::string str = "REQUEST_DATA_REPEAT_FROM_";
	strAppend(str, chosenNode);

	wsnPkt *pkt = createPkt(str.c_str(), REQUEST_FOR_DATA_REPEAT, chosenNode);
	EV << "Requesting Node " << chosenNode << " For its DATA!\n";
	sendDown( pkt );
}

