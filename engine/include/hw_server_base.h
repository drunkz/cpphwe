#ifndef HW_SERVERBASE_H
#define HW_SERVERBASE_H

#include "cfg_server_base.h"
#include "hw_config.h"
#include "hw_interface.h"

namespace hw {

class ServerBase : public IDataHandler {
  public:
    ServerBase(){};
    virtual ~ServerBase(){};

  public:
    bool Run(UINT16 initFlags);
    bool LoadCfg(Config* config, const std::string& filename);
    void Close();

  protected:
    virtual bool OnStart() = 0;
    virtual void OnClose() = 0;

  private:
    virtual bool OnDataHandle(IDataBuffer* dataBuffer, UINT32 connID);
    virtual bool OnCloseConnect(UINT32 connID);
    virtual bool OnNewConnect(UINT32 connID);

  protected:
    Cfg_ServerBase CfgServerBase;

  private:
    UINT64 m_startTime;
    UINT16 m_initFlags;
};

} // namespace hw

#endif // HW_SERVERBASE_H
