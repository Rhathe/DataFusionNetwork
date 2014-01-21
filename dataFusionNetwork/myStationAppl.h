#ifndef MY_STATION_APPL_H
#define MY_STATION_APPL_H

#include "stationAppl.h"

/**
 * @brief Data fusion Base Station class for the application layer
 *
 * @ingroup applLayer
 * @author Ramon Sandoval
 **/

class myStationAppl : public stationAppl
{
	public:

	protected:
		/** @brief Handle self messages such as timer... */
		virtual void handleSelfMsg(cMessage*);

		/** @brief Handle messages from lower layer */
		virtual void handleLowerMsg(cMessage*);

		virtual void chooseNode();

		void requestRepeatFusionData();

		bool firstData;
		enum rightOrWrong { WAS_RIGHT, WAS_WRONG, WAS_NEITHER };
		int typeOfData;
};

#endif
