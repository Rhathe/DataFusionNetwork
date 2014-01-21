//
// Generated file, do not edit! Created by opp_msgc 4.1 from wsnPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "wsnPkt_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(wsnPkt);

wsnPkt::wsnPkt(const char *name, int kind) : ApplPkt(name,kind)
{
    this->MACtype_var = 0;
    this->timeout_var = 0;
}

wsnPkt::wsnPkt(const wsnPkt& other) : ApplPkt()
{
    setName(other.getName());
    operator=(other);
}

wsnPkt::~wsnPkt()
{
}

wsnPkt& wsnPkt::operator=(const wsnPkt& other)
{
    if (this==&other) return *this;
    ApplPkt::operator=(other);
    this->MACtype_var = other.MACtype_var;
    this->timeout_var = other.timeout_var;
    this->data_var = other.data_var;
    return *this;
}

void wsnPkt::parsimPack(cCommBuffer *b)
{
    ApplPkt::parsimPack(b);
    doPacking(b,this->MACtype_var);
    doPacking(b,this->timeout_var);
    doPacking(b,this->data_var);
}

void wsnPkt::parsimUnpack(cCommBuffer *b)
{
    ApplPkt::parsimUnpack(b);
    doUnpacking(b,this->MACtype_var);
    doUnpacking(b,this->timeout_var);
    doUnpacking(b,this->data_var);
}

int wsnPkt::getMACtype() const
{
    return MACtype_var;
}

void wsnPkt::setMACtype(int MACtype_var)
{
    this->MACtype_var = MACtype_var;
}

simtime_t wsnPkt::getTimeout() const
{
    return timeout_var;
}

void wsnPkt::setTimeout(simtime_t timeout_var)
{
    this->timeout_var = timeout_var;
}

myData& wsnPkt::getData()
{
    return data_var;
}

void wsnPkt::setData(const myData& data_var)
{
    this->data_var = data_var;
}

class wsnPktDescriptor : public cClassDescriptor
{
  public:
    wsnPktDescriptor();
    virtual ~wsnPktDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(wsnPktDescriptor);

wsnPktDescriptor::wsnPktDescriptor() : cClassDescriptor("wsnPkt", "ApplPkt")
{
}

wsnPktDescriptor::~wsnPktDescriptor()
{
}

bool wsnPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<wsnPkt *>(obj)!=NULL;
}

const char *wsnPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int wsnPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int wsnPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *wsnPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "MACtype",
        "timeout",
        "data",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int wsnPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='M' && strcmp(fieldName, "MACtype")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeout")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *wsnPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "simtime_t",
        "myData",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *wsnPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int wsnPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    wsnPkt *pp = (wsnPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string wsnPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    wsnPkt *pp = (wsnPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getMACtype());
        case 1: return double2string(pp->getTimeout());
        case 2: {std::stringstream out; out << pp->getData(); return out.str();}
        default: return "";
    }
}

bool wsnPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    wsnPkt *pp = (wsnPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setMACtype(string2long(value)); return true;
        case 1: pp->setTimeout(string2double(value)); return true;
        default: return false;
    }
}

const char *wsnPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        "myData",
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *wsnPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    wsnPkt *pp = (wsnPkt *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getData()); break;
        default: return NULL;
    }
}


