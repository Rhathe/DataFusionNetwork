#ifndef MYPHYLAYERCONTROL_H_
#define MYPHYLAYERCONTROL_H_

#include "PhyLayerControl.h"
#include "sentMsgNotify.h"

class myPhyLayerControl : public PhyLayerControl {
public:
	virtual void initialize(int stage);

protected:

	virtual void handleUpperMessage(cMessage* msg);

	virtual bool sleep();
	virtual bool wakeUp();

	sentMsgNotify notification;
	int catSentMsgNotify;
	int hostId;
	int pktNum;
	int byteNum;
	cOutVector pktNumVec;
	cOutVector byteNumVec;
};

#endif /* MYPHYLAYERCONTROL_H_ */
