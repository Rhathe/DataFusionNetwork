cplusplus {{
#include "ApplPkt_m.h"
#include "myData.h"
}} 

class ApplPkt;
class noncobject myData;

message wsnPkt extends ApplPkt  {
	int MACtype;
	simtime_t timeout;
	myData data;
};
