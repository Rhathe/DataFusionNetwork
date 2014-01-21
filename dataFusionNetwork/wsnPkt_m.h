//
// Generated file, do not edit! Created by opp_msgc 4.1 from wsnPkt.msg.
//

#ifndef _WSNPKT_M_H_
#define _WSNPKT_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0401
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "ApplPkt_m.h"
#include "myData.h"
// }}



/**
 * Class generated from <tt>wsnPkt.msg</tt> by opp_msgc.
 * <pre>
 * message wsnPkt extends ApplPkt  {
 * 	int MACtype;
 * 	simtime_t timeout;
 * 	myData data;
 * };
 * </pre>
 */
class wsnPkt : public ::ApplPkt
{
  protected:
    int MACtype_var;
    simtime_t timeout_var;
    ::myData data_var;

    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const wsnPkt&);

  public:
    wsnPkt(const char *name=NULL, int kind=0);
    wsnPkt(const wsnPkt& other);
    virtual ~wsnPkt();
    wsnPkt& operator=(const wsnPkt& other);
    virtual wsnPkt *dup() const {return new wsnPkt(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getMACtype() const;
    virtual void setMACtype(int MACtype_var);
    virtual simtime_t getTimeout() const;
    virtual void setTimeout(simtime_t timeout_var);
    virtual myData& getData();
    virtual const myData& getData() const {return const_cast<wsnPkt*>(this)->getData();}
    virtual void setData(const myData& data_var);
};

inline void doPacking(cCommBuffer *b, wsnPkt& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, wsnPkt& obj) {obj.parsimUnpack(b);}


#endif // _WSNPKT_M_H_
