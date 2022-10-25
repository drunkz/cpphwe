#ifndef HW_INTERFACE_H
#define HW_INTERFACE_H

#include <basetsd.h>

namespace hw {

class IDataBuffer {};

class IDataHandler {
  public:
    virtual bool OnDataHandle(IDataBuffer* dataBuffer, UINT32 connID) = 0;
    virtual bool OnCloseConnect(UINT32 connID) = 0;
    virtual bool OnNewConnect(UINT32 connID) = 0;
};

} // namespace hw

#endif // HW_INTERFACE_H