#include "myPhyLayerControl.h"

Define_Module(myPhyLayerControl);

// ----------------- Public member functions ----------------------------------

void myPhyLayerControl::initialize(int stage) {

	PhyLayerControl::initialize(stage);
	pktNum = 0;
	byteNum = 0;

	if (stage == 0) {
		cModule* host = findHost();
		if(host == 0) {
			throw cRuntimeError("Host pointer is NULL in myPhyLayerControl");
		}
		hostId = host->getId();

		// Obtain category for publishing phy commands on
		catSentMsgNotify = utility->getCategory(&notification);

		pktNumVec.setName("Total packets sent");
		byteNumVec.setName("Total bytes sent");
	}
}

// ----------------- Protected member functions -------------------------------



void myPhyLayerControl::handleUpperMessage(cMessage* msg)
{
	cPacket *m = static_cast<cPacket *>(msg);
	++pktNum;
	byteNum += m->getByteLength() + headerLength/8;

	pktNumVec.recordWithTimestamp(simTime(), pktNum);
	byteNumVec.recordWithTimestamp(simTime(), byteNum);

	PhyLayerControl::handleUpperMessage(msg);
	utility->publishBBItem(catSentMsgNotify, &notification, hostId);
}

bool myPhyLayerControl::sleep()
{
    return turnOff();
}

bool myPhyLayerControl::wakeUp()
{
    return turnOn();
}

