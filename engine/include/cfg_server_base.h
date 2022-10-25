#ifndef CFG_SERVER_BASE_H
#define CFG_SERVER_BASE_H

#include "hw_config.h"
#include <string>

namespace hw {
class Cfg_ServerBase : public Config {
  public:
    Cfg_ServerBase();
    virtual ~Cfg_ServerBase();

  protected:
    virtual void FillData();

  public:
    std::string ServerName; // 服务名称
    std::string ServerAddr; // 服务绑定IP
    UINT16 ServerPort;      // 服务绑定端口
    UINT32 MaxConn;         // 最大连接数
    UINT8 LogConsoleLevel;  // 控制台日志输出等级
    UINT8 LogFileLevel;     // 日志文件输出等级
    // std::string DBServiceAddr;
    // int DBServicePort;
    // std::string DBServiceAccount;
    // std::string DBServicePassWord;
    // std::string DBName;
};

} // namespace hw

#endif // CFG_SERVER_BASE_H
