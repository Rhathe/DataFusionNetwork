/***************************************************************************
 * file:        timeFusionAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "timeFusionAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>
#include <string>

Define_Module(timeFusionAppl);

void timeFusionAppl::initialize(int stage)
{
	myAppl::initialize(stage);

	baseStationAddr = 0;
	MACrand = 99;
	rightVotes = 0;
	wrongVotes = 0;
	rightVOTEsent = 0;
	wrongVOTEsent = 0;
	rightDATAsent = 0;
	wrongDATAsent = 0;
	received = 0;
	threshOfAttack = 20;
	threshold = 3;
	beginRounds = 10;
	order = myApplAddr();
	roundNum = 0;

	if (stage == 0) {
		sendVoteTimer = new cMessage( "send-vote-timer", SCHEDULED_VOTE );
		beginRoundTimer = new cMessage( "begin-round-timer", BEGIN_ROUNDS );
	}
	else if(stage==1) {
		scheduleAt(beginRounds + SCALE*5*roundNum + SCALE*order + 0.0001, sendVoteTimer);
		scheduleAt(beginRounds + SCALE*5*roundNum, beginRoundTimer);
	}
}

void timeFusionAppl::handleLowerMsg( cMessage* msg )
{
	wsnPkt *m = static_cast<wsnPkt *>(msg);
	++received;
	if (checkAddr(msg) == false) {
		delete msg;
		return;
	}

	std::string s("SEND_BASE_ADDRESS: ");

	switch( msg->getKind() ){
		case BROADCAST_BASE_ADDRESS:
			EV << "Received Address from Base Station["<<m->getSrcAddr()<<"]\n";
			baseStationAddr = m->getSrcAddr();

			strAppend(s, myApplAddr());

			sendToBase(s.c_str(), SEND_BASE_OWN_ADDRESS);
			delete msg;
			break;
		case RIGHT_FUSION_DATA:
		case RIGHT_VOTE:
			++rightVotes;
			delete msg;
			break;
		case WRONG_FUSION_DATA:
		case WRONG_VOTE:
			++wrongVotes;
			delete msg;
			break;
	}
}


void timeFusionAppl::handleSelfMsg(cMessage *msg) {

	myAppl::handleSelfMsg(msg);

	switch( msg->getKind() ){
		case RECORD_POWER:
			break;
		case SCHEDULED_VOTE:
			MACrand = intuniform(0,99);

			if (MACrand <= threshOfAttack) {
				++wrongVOTEsent;
				if (wrongVotes > 0 && wrongVotes < threshold) {
					std::string s = "WRONG_VOTE: ";
					strAppend(s, roundNum);
					sendOutData(s.c_str(), WRONG_VOTE);
				}
				else if (wrongVotes == 0){
					std::string s = "WRONG_FUSION_DATA: ";
					strAppend(s, roundNum);
					sendOutData(s.c_str(), WRONG_FUSION_DATA);
				}
			}
			else if (MACrand > threshOfAttack) {
				++rightVOTEsent;
				if (rightVotes > 0 && rightVotes < threshold) {
					std::string s = "RIGHT_VOTE: ";
					strAppend(s, roundNum);
					sendOutData(s.c_str(), RIGHT_VOTE);
				}
				else if (rightVotes == 0) {
					std::string s = "RIGHT_FUSION_DATA: ";
					strAppend(s, roundNum);
					sendOutData(s.c_str(), RIGHT_FUSION_DATA);
				}
			}
			order = (order+1)%5;
			scheduleAt(beginRounds + SCALE*5*roundNum + SCALE*order + 0.0001, sendVoteTimer);
			break;
		case BEGIN_ROUNDS:
			recordRoundPower();
			roundNum++;
			rightVotes = 0;
			wrongVotes = 0;
			scheduleAt(beginRounds + SCALE*5*roundNum, beginRoundTimer);
			break;
		default:
			EV << "Unknown selfmessage! -> delete, kind: "<<msg->getKind() <<endl;
			delete msg;
	}
}

void timeFusionAppl::sendToBase(const char* messageID, int messageType) {

	wsnPkt *pkt = createPkt(messageID, messageType, baseStationAddr);
	sendDown (pkt);
}

void timeFusionAppl::sendOutData(const char* messageID, int messageType) {

	wsnPkt *pkt = createPkt(messageID, messageType, -1);
	switch(messageType) {
		case RIGHT_FUSION_DATA:
			++rightDATAsent;
			break;
		case WRONG_FUSION_DATA:
			++wrongDATAsent;
			break;
	}
	sendDown (pkt);
}

void timeFusionAppl::finish()
{
	EV << "Average Power Per Round:     " << totalPower/numRounds << endl;
	EV << "Right VOTE Sent:     " << rightVOTEsent << endl;
	EV << "Wrong VOTE Sent:     " << wrongVOTEsent << endl;
	EV << "Right DATA Sent:     " << rightDATAsent << endl;
	EV << "Wrong DATA Sent:     " << wrongDATAsent << endl;
	EV << "Queued messages:     " << sentMsgs << endl;
	EV << "Received: " << received << endl;

	recordScalar("#AvgPower", totalPower/numRounds);
	recordScalar("#AvgRXPower", rxPower/numRounds);
	recordScalar("#AvgTXPower", txPower/numRounds);
	recordScalar("#rightVOTEsent", rightVOTEsent);
	recordScalar("#wrongVOTEsent", wrongVOTEsent);
	recordScalar("#rightDATAsent", rightDATAsent);
	recordScalar("#wrongDATAsent", wrongDATAsent);
	recordScalar("#received", received);
	EV << endl;
}

