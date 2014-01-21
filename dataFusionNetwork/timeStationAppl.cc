/***************************************************************************
 * file:        timeStationAppl.cc
 *
 * author:      Ramon Sandoval
 ***************************************************************************/


#include "timeStationAppl.h"
#include <NetwControlInfo.h>

#include <SimpleAddress.h>

Define_Module(timeStationAppl);

void timeStationAppl::initialize(int stage)
{
	myAppl::initialize(stage);

	beginRounds = 10;
	roundNum = 0;

	if(stage == 0) {
		delayTimer = new cMessage( "delay-timer", SEND_BASE_ADDRESS_TIMER );
		beginRoundTimer = new cMessage( "begin-round-timer", BEGIN_ROUNDS );
		rightDataPktsVec.setName("Num of Right Pkts");
		rightDataBytesVec.setName("Amount of Bytes for Right Pkts");
	}
	else if(stage==1) {
		scheduleAt(simTime() + dblrand() * 10, delayTimer);
		scheduleAt(beginRounds + SCALE*5*roundNum, beginRoundTimer);
	}

	timeout = 2.0;
	timeoutGlobal = timeout;
	numNodes = 0;
	threshold = 2;

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

void timeStationAppl::handleLowerMsg( cMessage* msg )
{
	wsnPkt *m;
	m = static_cast<wsnPkt *>(msg);
	++received;
	if (checkAddr(msg) == false) {
		delete msg;
		return;
	}

	switch( msg->getKind() ){
		case SEND_BASE_OWN_ADDRESS:
			EV << "Received address from host["<<m->getSrcAddr()<<"]; delete msg\n";
			fusionNodes.push_back(m->getSrcAddr());
			numNodes++;
			delete msg;
			break;
		case RIGHT_FUSION_DATA:
		case RIGHT_VOTE:
			++rightVotes;
			if (rightVotes >= threshold) {
				increaseRightData();
				++rightVotedIn;
			}
			delete msg;
			break;
		case WRONG_FUSION_DATA:
		case WRONG_VOTE:
			++wrongVotes;
			if (wrongVotes >= threshold) {
				++wrongVotedIn;
			}
			delete msg;
			break;
		default:
			EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
			delete msg;
	}
}

void timeStationAppl::handleSelfMsg(cMessage *msg) {

	myAppl::handleSelfMsg(msg);

	switch( msg->getKind() ){
		case RECORD_POWER:
			break;
		case SEND_BASE_ADDRESS_TIMER:
			sendBroadcastAddress();
			break;
		case BEGIN_ROUNDS:
			recordRoundPower();
			roundNum++;
			numRounds++;
			rightVotes = 0;
			wrongVotes = 0;
			scheduleAt(beginRounds + SCALE*5*roundNum, beginRoundTimer);
			break;
		default:
			EV << "Unknown selfmessage! -> delete, kind: "<<msg->getKind() <<endl;
			delete msg;
	}
}

void timeStationAppl::sendBroadcastAddress() {

	wsnPkt *pkt = createPkt("BROADCAST_BASE_ADDRESS", BROADCAST_BASE_ADDRESS, -1);
	pkt->setTimeout(timeoutGlobal);
	EV << "Broadcasting Base Station's Address!\n";
	sendDown( pkt );
}

void timeStationAppl::finish()
{
	if (completeTime == 0) {
		completeTime = simTime().dbl();
	}


	EV << "time:     " << completeTime << endl;
	EV << "Average Power Per Round:     " << totalPower/numRounds << endl;
	EV << "Average Time Per Round:     " << (completeTime-beginVotingTime)/numRounds << endl;
    EV << "Wrong Voted In:     " << wrongVotedIn << endl;
    EV << "Right Voted In:     " << rightVotedIn << endl;
    EV << "right Data received:     " << rightDATAreceived << endl;
    EV << "Received: " << received << endl;

    recordScalar("#AvgPower", totalPower/numRounds);
	recordScalar("#AvgRXPower", rxPower/numRounds);
	recordScalar("#AvgTXPower", txPower/numRounds);
    recordScalar("#AvgTime", (completeTime-beginVotingTime)/numRounds);
    recordScalar("#wrongVotedIn", wrongVotedIn);
    recordScalar("#rightVotedIn", rightVotedIn);
    recordScalar("#rightDATAreceived", rightDATAreceived);
    recordScalar("#received", received);
    EV << endl;
}

void timeStationAppl::increaseRightData() {
	++rightDATAreceived;
	rightDATAbytes += 1024;
	rightDataPktsVec.recordWithTimestamp(simTime(),rightDATAreceived);
	rightDataBytesVec.recordWithTimestamp(simTime(),rightDATAbytes);
}

