/* -*- mode:c++ -*- ********************************************************
 * Energy Framework for Omnet++, version 0.9
 *
 * Author:  Laura Marie Feeney
 *
 * Copyright 2009 Swedish Institute of Computer Science.
 *
 * This software is provided `as is' and without any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose.
 *
 ***************************************************************************/
#ifndef MY_BATTERY_H
#define MY_BATTERY_H

#include "SimpleBattery.h"
#include "vector"

class myBattery: public SimpleBattery {

	public:
		std::vector<double> recordStuff();

};

#endif
