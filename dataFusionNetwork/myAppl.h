#ifndef MY_APPL_H
#define MY_APPL_H

#include <BaseApplLayer.h>
#include "wsnPkt_m.h"
#include "IControllable.h"
#include "NicController.h"
#include "NicCommandNotify.h"
#include "sentMsgNotify.h"
#include <string>
#include <sstream>
#include "myBattery.h"

#define SCALE 1

/**
 * @brief Data fusion class for the application layer
 *
 * @ingroup applLayer
 * @author Ramon Sandoval
 **/
class myAppl : public BaseApplLayer
{
  public:
    virtual ~myAppl();

    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int);

    void strAppend(std::string &str, int x);

    virtual void receiveBBItem(int category, const BBItem *details, int scopeModuleId);

    /**
	 * @brief Return a pointer to the primary interface.  Returns 0 if the
	 * interface is not controllable.
	 */
	virtual IControllable* getIface() const {return controller;};

    /** @brief Message kinds used by this layer.*/
    enum myApplMessageKinds{
		BROADCAST_BASE_ADDRESS,
		SEND_BASE_OWN_ADDRESS,
		SEND_BASE_ADDRESS_TIMER,
		SEND_REQUEST_TIMER,
		CONTROL_TIMER,
		MAC_REQ_TIMER,
		VOTE_REQ_TIMER,
		DATA_REQ_TIMER,
		DATA_CONFIRM_TIMER,
		DATA_REQ_REPEAT_TIMER,
		BASE_REPLY,
		REQUEST_FOR_DATA,
		REQUEST_FOR_DATA_REPEAT,
		REQUEST_FOR_VOTES,
		REQUEST_FOR_MAC,
		RIGHT_FUSION_DATA,
		WRONG_FUSION_DATA,
		RIGHT_MAC,
		WRONG_MAC,
		RIGHT_VOTE,
		WRONG_VOTE,
		BEGIN_ROUNDS,
		WAKEUP,
		TIMEOUT,
		RECORD_POWER,
		SCHEDULED_VOTE
    };

protected:
    /** @brief Timer message for scheduling next message.*/
    cMessage *delayTimer;
    cMessage *controlTimer;
    cMessage *powerTimer;

    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

    bool checkAddr(cMessage*);

    void BaseModuleInit(int stage);
    void otherInit(int stage);

    /** @brief send a reply to a broadcast message */
    wsnPkt *createPkt(const char *messageID, int messageType, int dest);

    /** @brief Called when the state of the NIC changes */
	virtual void nicStateUpdate(IControllable::Status newState);

	virtual void sendDown(cMessage*);

	void sendXLControl(IControllable::Controls command);
	virtual void turnOn();
	virtual void turnOff();
	virtual void sleep();
	virtual void wakeUp();

	virtual void recordRoundPower();

    int nicId;

	NicController* controller;

	/** @brief Blackboard category for nic state update notifications */
	int catNicStateNotify;

	/** @brief Blackboard category for sending commands to nic's*/
	int catNicCommandNotify;

	/** @brief Blackboard category for phy state update notifications */
	int catPhyStateNotify;

	int nicState;
	int sentMsgs;
	IControllable::Controls delayedControl;
	bool delayedControlPresent;

	myBattery* battery;
	cOutVector powerVector;
	cOutVector powerPerRoundVector;
	double totalPower;
	double rxPower;
	double txPower;
	double pastPower;
	static double completeTime;
	static double beginVotingTime;
	static int numRounds;
};

#endif
