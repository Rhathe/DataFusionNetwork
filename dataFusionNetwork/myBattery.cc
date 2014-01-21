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
/*
 * A simple linear model of battery consumption.  Simple Battery
 * receives DrawMsg's from one or more devices, updates residual
 * capacity (total current * voltage * time), publishes HostState
 * notification on battery depletion, and provides time series and
 * summary information to Battery Stats module.
 */

#include "FWMath.h"
#include "myBattery.h"
#include "BatteryStats.h"

Define_Module(myBattery);

std::vector<double> myBattery::recordStuff() {
	deductAndCheck();

	for (int i = 0; i < numDevices; i++) {

		std::vector<double> power;
		double total = 0;
		power.push_back(total);
		double dummy;
		for (int j = 0; j < devices[i].numAccts; j++) {
			dummy = devices[i].accts[j];
			total += dummy;
			power.push_back(dummy);
		}

		power[0] = total;
		return power;

		/*
		for (int j = 0; j < devices[i].numAccts; j++) {
			debugEV << "activity " << j << ": " << devices[i].accts[j] << " mWs and "
			<< devices[i].times[j] << "sec" << endl;
		}

		// check that total time in all states matches simulation time
		simtime_t sum = 0;
		for (int j = 0; j < devices[i].numAccts; j++)
		sum += devices[i].times[j];
		if (FWMath::round(sum.dbl() * 1000000) - FWMath::round(simTime().dbl() * 1000000) != 0)
		{
			EV << "WARNING: device " << devices[i].name << " total time " << sum
			<< " != sim time " << simTime() << " (may not matter)" << endl;
		}*/
	}
}

