/***************************************************************************
 * file:        fusionAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "fusionAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>

Define_Module(fusionAppl);

void fusionAppl::initialize(int stage)
{
	myAppl::initialize(stage);
	baseStationAddr = 0;
	MACrand = 99;
	canVote = true;
	rightMACSsent = 0;
	wrongMACSsent = 0;
	rightVOTEsent = 0;
	wrongVOTEsent = 0;
	rightDATAsent = 0;
	wrongDATAsent = 0;
	received = 0;
	threshOfAttack = 20;
}

void fusionAppl::handleLowerMsg( cMessage* msg )
{
	wsnPkt *m = static_cast<wsnPkt *>(msg);
	++received;
	if (checkAddr(msg) == false) {
		if (msg->getKind() == REQUEST_FOR_MAC) {
			recordRoundPower();
		}
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
		case REQUEST_FOR_DATA:
			EV << "Received Request from Base Station["<<m->getSrcAddr()<<"]\n";

			if (MACrand <= threshOfAttack) {
				sendToBase("WRONG_FUSION_DATA", WRONG_FUSION_DATA);
			}
			else {
				sendToBase("RIGHT_FUSION_DATA", RIGHT_FUSION_DATA);
			}

			delete msg;
			break;
		case REQUEST_FOR_MAC:
			EV << "Received Request for MAC from Base Station["<<m->getSrcAddr()<<"]\n";
			recordRoundPower();
			MACrand = intuniform(0,99);

			if (MACrand <= threshOfAttack) {
				sendToBase("WRONG_MAC", WRONG_MAC);
				++wrongMACSsent;
			}
			else {
				sendToBase("RIGHT_MAC", RIGHT_MAC);
				++rightMACSsent;
			}

			canVote = false;
			delete msg;
			break;
		case REQUEST_FOR_VOTES:
			if (canVote == false) {
				canVote = true;
			}
			else {
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
			}
			delete msg;
			break;
		default:
			EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
			delete msg;
	}
}

/**
 * A timer with kind = SEND_BROADCAST_TIMER indicates that a new
 * broadcast has to be send (@ref sendBroadcast).
 *
 * There are no other timer implemented for this module.
 *
 * @sa sendBroadcast
 **/
void fusionAppl::handleSelfMsg(cMessage *msg) {

	myAppl::handleSelfMsg(msg);

	switch( msg->getKind() ){
		case RECORD_POWER:
			break;
		default:
			EV << "Unknown selfmessage! -> delete, kind: "<<msg->getKind() <<endl;
			delete msg;
	}
}

void fusionAppl::sendToBase(const char* messageID, int messageType) {

	wsnPkt *pkt = createPkt(messageID, messageType, baseStationAddr);
	switch(messageType) {
		case RIGHT_FUSION_DATA:
			++rightDATAsent;
			break;
		case WRONG_FUSION_DATA:
			++wrongDATAsent;
			break;
	}
	sendDown (pkt);

	EV << "sent reply to base station" << endl;
}

void fusionAppl::finish()
{
	EV << "Average Power Per Round:     " << totalPower/numRounds << endl;
	EV << "Right MACs Sent:     " << rightMACSsent << endl;
	EV << "Wrong MACs Sent:     " << wrongMACSsent << endl;
	EV << "Right VOTE Sent:     " << rightVOTEsent << endl;
	EV << "Wrong VOTE Sent:     " << wrongVOTEsent << endl;
	EV << "Right DATA Sent:     " << rightDATAsent << endl;
	EV << "Wrong DATA Sent:     " << wrongDATAsent << endl;
	EV << "Queued messages:     " << sentMsgs << endl;
	EV << "Received: " << received << endl;

	recordScalar("#AvgPower", totalPower/numRounds);
	recordScalar("#AvgRXPower", rxPower/numRounds);
	recordScalar("#AvgTXPower", txPower/numRounds);
	recordScalar("#rightMACSsent", rightMACSsent);
	recordScalar("#wrongMACSsent", wrongMACSsent);
	recordScalar("#rightVOTEsent", rightVOTEsent);
	recordScalar("#wrongVOTEsent", wrongVOTEsent);
	recordScalar("#rightDATAsent", rightDATAsent);
	recordScalar("#wrongDATAsent", wrongDATAsent);
	recordScalar("#received", received);
	EV << endl;
}

fusionAppl::~fusionAppl()
{
	cancelAndDelete(delayTimer);
}
