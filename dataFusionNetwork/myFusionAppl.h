#ifndef MY_FUSION_APPL_H
#define MY_FUSION_APPL_H

#include "fusionAppl.h"

/**
 * @brief Data fusion class for the application layer
 *
 * @ingroup applLayer
 * @author Ramon Sandoval
 **/

class myFusionAppl : public fusionAppl
{
	public:
		virtual ~myFusionAppl();
		virtual void initialize(int);

	protected:
		/** @brief Handle self messages such as timer... */
		virtual void handleSelfMsg(cMessage*);

		/** @brief Handle messages from lower layer */
		virtual void handleLowerMsg(cMessage*);

		virtual void sleep(simtime_t);

		simtime_t restTime;
		simtime_t timeout;
};

#endif
