#ifndef FUSION_APPL_H
#define FUSION_APPL_H

#include <BaseApplLayer.h>
#include "myAppl.h"
#include <myData.h>
#include <string>
#include <sstream>

/**
 * @brief Data fusion class for the application layer
 *
 * @ingroup applLayer
 * @author Ramon Sandoval
 **/

class fusionAppl : public myAppl
{
	public:
		virtual ~fusionAppl();

		/** @brief Initialization of the module and some variables*/
		virtual void initialize(int);

	protected:
		/** @brief Handle self messages such as timer... */
		virtual void handleSelfMsg(cMessage*);

		/** @brief Handle messages from lower layer */
		virtual void handleLowerMsg(cMessage*);

		virtual void sendToBase(const char* messageID, int messageType);

		virtual void finish();

		int baseStationAddr;
		int MACrand;
		int rightMACSsent;
		int wrongMACSsent;
		int rightVOTEsent;
		int wrongVOTEsent;
		int rightDATAsent;
		int wrongDATAsent;
		int received;
		bool canVote;

		int threshOfAttack;
};

#endif
