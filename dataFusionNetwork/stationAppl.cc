/***************************************************************************
 * file:        stationAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "stationAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>

Define_Module(stationAppl);

void stationAppl::initialize(int stage)
{
	myAppl::initialize(stage);
	if(stage == 0) {
		delayTimer = new cMessage( "delay-timer", SEND_BASE_ADDRESS_TIMER );
		rightDataPktsVec.setName("Num of Right Pkts");
		rightDataBytesVec.setName("Amount of Bytes for Right Pkts");
	}
	else if(stage==1) {
		scheduleAt(simTime() + dblrand() * 10, delayTimer);
	}

	timeout = 2.0;
	timeoutGlobal = timeout;
	numNodes = 0;
	threshold = 2;

	MACrequests = 0;
	VOTErequests = 0;
	DATArequests = 0;
	rightDATAreceived = 0;
	rightDATAbytes = 0;
	rightVotedIn = 0;
	wrongVotedIn = 0;
	received = 0;
	roundTime = 0;
	beginVotingTime = 0;
	completeTime = 0;
	numRounds = -1;
	endVotesNum = 100000;
}

void stationAppl::initializeRound() {

	if (VOTErequests > endVotesNum) {
		cancelEvent(delayTimer);
		completeTime = simTime().dbl();
		numRounds = VOTErequests;
		endSimulation();
	}

	if (roundTime == 0) {
		beginVotingTime = simTime().dbl();
	}

	rightVotes = 0;
	wrongVotes = 0;
	rightNode = 0;
	wrongNode = 0;
	negVotes = 0;
	firstRightMAC = false;
	firstWrongMAC = false;
	firstMAC = false;
	chosenNode = 0;
	++numRounds;
	roundTime = simTime();
	recordRoundPower();
}

void stationAppl::handleLowerMsg( cMessage* msg )
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
			timeout = 0.0001;
			m = delayTimeout(msg);
			EV << "Received fusion data from host["<<m->getSrcAddr()<<"]; delete msg\n";
			delete msg;
			increaseRightData();
			break;
		case RIGHT_MAC:
			m = delayTimeout(msg);
			delayTimer->setKind(DATA_REQ_TIMER);

			if (firstMAC == false) {
				firstMAC = true;
				sendBroadcastVote(RIGHT_MAC);
			}

			if (firstRightMAC == false) {
				firstRightMAC = true;
				rightNode = m->getSrcAddr();
			}
			else {
				++negVotes;
				if (negVotes > threshold) {
					chosenNode = rightNode;
					++rightVotedIn;
				}
			}
			++rightVotes;
			delete msg;
			break;
		case WRONG_MAC:
			m = delayTimeout(msg);
			delayTimer->setKind(DATA_REQ_TIMER);

			if (firstMAC == false) {
				firstMAC = true;
				sendBroadcastVote(WRONG_MAC);
			}

			if (firstWrongMAC == false) {
				firstWrongMAC = true;
				wrongNode = m->getSrcAddr();
			}
			else {
				++negVotes;
				if (negVotes > threshold) {
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

wsnPkt *stationAppl::delayTimeout(cMessage *msg) {
	wsnPkt *m;
	m = static_cast<wsnPkt *>(msg);
	cancelEvent(delayTimer);
	scheduleAt(simTime()+timeout, delayTimer);
	return m;
}

/**
 * A timer with kind = SEND_BROADCAST_TIMER indicates that a new
 * broadcast has to be send (@ref sendBroadcast).
 *
 * There are no other timer implemented for this module.
 *
 * @sa sendBroadcast
 **/
void stationAppl::handleSelfMsg(cMessage *msg) {

	myAppl::handleSelfMsg(msg);

	switch( msg->getKind() ){
		case RECORD_POWER:
			break;
		case SEND_BASE_ADDRESS_TIMER:
			sendBroadcastAddress();
			delayTimer->setKind(MAC_REQ_TIMER);
			scheduleAt(simTime()+timeout, delayTimer);
			break;
		case MAC_REQ_TIMER:
			chooseNode();
			timeout = 2.0;
			scheduleAt(simTime()+timeout, delayTimer);
			break;
		case DATA_REQ_TIMER:
			requestFusionData();
			delayTimer->setKind(MAC_REQ_TIMER);
			scheduleAt(simTime()+timeout, delayTimer);
			break;
		default:
			EV << "Unknown selfmessage! -> delete, kind: "<<msg->getKind() <<endl;
			delete msg;
	}
}

void stationAppl::sendBroadcastAddress() {

	wsnPkt *pkt = createPkt("BROADCAST_BASE_ADDRESS", BROADCAST_BASE_ADDRESS, -1);
	pkt->setTimeout(timeoutGlobal);
	EV << "Broadcasting Base Station's Address!\n";
	sendDown( pkt );
}

void stationAppl::sendBroadcastRequest() {

	wsnPkt *pkt = createPkt("REQUEST_FOR_DATA", REQUEST_FOR_DATA, -1);
	initializeRound();
	EV << "Broadcasting Request For Fusion Data!\n";
	sendDown( pkt );
}

void stationAppl::sendBroadcastVote(int MACtype) {

	//recordScalar("#receive MAC time", simTime() - roundTime);
	++VOTErequests;
	wsnPkt *pkt = createPkt("REQUEST_FOR_VOTES", REQUEST_FOR_VOTES, -1);
	pkt->setMACtype(MACtype);
	EV << "Broadcasting Request For Votes on MAC!\n";
	sendDown( pkt );

	ApplPkt x;
}

void stationAppl::chooseNode() {

	++MACrequests;
	initializeRound();
	int k = intuniform(0, numNodes-1);
	chosenNode = fusionNodes[k];
	std::string str = "REQUEST_MAC_FROM_";
	strAppend(str, chosenNode);

	wsnPkt *pkt = createPkt(str.c_str(), REQUEST_FOR_MAC, chosenNode);
	EV << "Requesting Node " << chosenNode << " For its MAC!\n";
	sendDown( pkt );
}

void stationAppl::requestFusionData() {

	//recordScalar("#receive Votes time", simTime() - roundTime);
	++DATArequests;
	std::string str = "REQUEST_DATA_FROM_";
	strAppend(str, chosenNode);

	wsnPkt *pkt = createPkt(str.c_str(), REQUEST_FOR_DATA, chosenNode);
	EV << "Requesting Node " << chosenNode << " For its DATA!\n";
	sendDown( pkt );
}

void stationAppl::finish()
{
	if (completeTime == 0) {
		completeTime = simTime().dbl();
	}


	EV << "time:     " << completeTime << endl;
	EV << "Average Power Per Round:     " << totalPower/numRounds << endl;
	EV << "Average Time Per Round:     " << (completeTime-beginVotingTime)/numRounds << endl;
    EV << "MAC requests Sent:     " << MACrequests << endl;
    EV << "Vote requests Sent:     " << VOTErequests << endl;
    EV << "Data requests Sent:     " << DATArequests << endl;
    EV << "Wrong Voted In:     " << wrongVotedIn << endl;
    EV << "Right Voted In:     " << rightVotedIn << endl;
    EV << "right Data received:     " << rightDATAreceived << endl;
    EV << "Received: " << received << endl;

    recordScalar("#AvgPower", totalPower/numRounds);
	recordScalar("#AvgRXPower", rxPower/numRounds);
	recordScalar("#AvgTXPower", txPower/numRounds);
    recordScalar("#AvgTime", (completeTime-beginVotingTime)/numRounds);
    recordScalar("#MACrequests", MACrequests);
    recordScalar("#VOTErequests", VOTErequests);
    recordScalar("#DATArequests", DATArequests);
    recordScalar("#wrongVotedIn", wrongVotedIn);
    recordScalar("#rightVotedIn", rightVotedIn);
    recordScalar("#rightDATAreceived", rightDATAreceived);
    recordScalar("#received", received);
    EV << endl;
}

void stationAppl::increaseRightData() {
	++rightDATAreceived;
	rightDATAbytes += 1024;
	rightDataPktsVec.recordWithTimestamp(simTime(),rightDATAreceived);
	rightDataBytesVec.recordWithTimestamp(simTime(),rightDATAbytes);
}

stationAppl::~stationAppl()
{
	cancelAndDelete(delayTimer);
}
