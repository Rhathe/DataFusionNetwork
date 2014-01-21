#ifndef TIME_FUSION_APPL_H
#define TIME_FUSION_APPL_H

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

class timeFusionAppl : public myAppl
{
	public:

		/** @brief Initialization of the module and some variables*/
		virtual void initialize(int);

	protected:
		/** @brief Handle self messages such as timer... */
		virtual void handleSelfMsg(cMessage*);

		/** @brief Handle messages from lower layer */
		virtual void handleLowerMsg(cMessage*);

		virtual void sendOutData(const char* messageID, int messageType);
		virtual void sendToBase(const char* messageID, int messageType);

		virtual void finish();

		int baseStationAddr;
		int MACrand;
		int rightVOTEsent;
		int wrongVOTEsent;
		int rightDATAsent;
		int wrongDATAsent;
		int rightVotes;
		int wrongVotes;
		int received;
		int threshold;
		bool canVote;

		int threshOfAttack;

		int roundNum;
		int order;
		simtime_t beginRounds;
		cMessage *sendVoteTimer;
		cMessage *beginRoundTimer;
};

#endif
