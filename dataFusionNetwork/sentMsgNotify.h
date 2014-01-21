#ifndef SENTMSGNOTIFY_H_
#define SENTMSGNOTIFY_H_

#include "ImNotifiable.h"

class sentMsgNotify : public BBItem
{
    BBITEM_METAINFO(BBItem)

public:
   bool sentMsg;
};

#endif
