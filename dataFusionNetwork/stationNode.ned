package mywsn;

import org.mixim.modules.nic.NicCSMA;

import org.mixim.base.modules.*;

module stationNode extends BaseNode {
 	parameters:
    		int fusionNum;
    	
	appl.headerLength = 512bit;
}
