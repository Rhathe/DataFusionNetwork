#ifndef CSMACONTROL_H_
#define CSMACONTROL_H_

#include "csma.h"
#include "IControllable.h"

class csmaControl : public csma, public IControllable
{
public:
	enum InternalMacState {IDLE, WAITING_FOR_PHY_ON, WAITING_FOR_PHY_OFF, WAITING_FOR_PHY_SLEEP, WAITING_FOR_PHY_AWAKE};

	virtual void initialize(int);
	//virtual void finish();

protected:
	virtual bool turnOn();
	virtual bool turnOff();

	virtual bool sleep();
	virtual bool wakeUp();

	/**
	 * @brief Overridden to handle messages from above.
	 */
	virtual void handleUpperMsg(cMessage* msg);

	/**
	 * @brief Overridden to handle IControllable control messages from above.
	 */
	virtual void handleUpperControl(cMessage* msg);

	/**
	 * @brief Overriden to handle IControllable status notifications from
	 * the PHY.
	 */
	virtual void handleLowerControl(cMessage *msg);

	/** @brief Override to silently drop any frames that are received while
	 * we are in off state */
	virtual void handleLowerMsg(cMessage *msg);

	virtual void resetCSMA();

	InternalMacState internalState;
	int status;
};

#endif /* CSMACONTROL_H_ */
