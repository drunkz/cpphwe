#ifndef HW_DBSERVER_H
#define HW_DBSERVER_H

#include "../engine/include/hw_server_base.h"
#include "../engine/include/hw_singleton.h"

namespace hw {
class DBServer : public ServerBase, public Singleton<DBServer> {
    friend class Singleton<DBServer>;

  public:
    virtual bool OnStart();
    virtual void OnClose();

  private:
    DBServer() = default;
    virtual ~DBServer() = default;
};
} // namespace hw

#endif // HW_DBSERVER_H
