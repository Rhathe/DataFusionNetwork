#ifndef TIME_STATION_APPL_H
#define TIME_STATION_APPL_H

#include <BaseApplLayer.h>
#include "myAppl.h"
#include <vector>

/**
 * @brief Data fusion Base Station class for the application layer
 *
 * @ingroup applLayer
 * @author Ramon Sandoval
 **/

class timeStationAppl : public myAppl
{
	public:

		/** @brief Initialization of the module and some variables*/
		virtual void initialize(int);


	protected:
		/** @brief Handle self messages such as timer... */
		virtual void handleSelfMsg(cMessage*);

		/** @brief Handle messages from lower layer */
		virtual void handleLowerMsg(cMessage*);

		virtual void sendBroadcastAddress();

		virtual void increaseRightData();

		virtual void finish();

		std::vector<int> fusionNodes;

		simtime_t timeout;  // timeout
		simtime_t timeoutGlobal;
		cMessage *timeoutEvent;
		int numNodes;
		int rightVotes;
		int rightNode;
		int rightVotedIn;
		int wrongNode;
		int wrongVotes;
		int wrongVotedIn;
		int negVotes;
		int threshold;
		int chosenNode;

		int DATArequests;
		int rightDATAreceived;
		int rightDATAbytes;
		int received;
		int endVotesNum;

		simtime_t roundTime;

		cOutVector rightDataPktsVec;
		cOutVector rightDataBytesVec;

		simtime_t beginRounds;
		cMessage *beginRoundTimer;
		int roundNum;
};

#endif
