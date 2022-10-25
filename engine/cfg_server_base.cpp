#include "include/cfg_server_base.h"

namespace hw {

Cfg_ServerBase::Cfg_ServerBase() {
    LogConsoleLevel = 0;
    LogFileLevel = 0;
    // 其他默认值由 FillData() 中设置。
}

Cfg_ServerBase::~Cfg_ServerBase() {
}

void Cfg_ServerBase::FillData() {
    ServerName = ini.GetValue("server", "ServerName", "server");
    ServerAddr = ini.GetValue("server", "ServerAddr", "0.0.0.0");
    ServerPort = ini.GetLongValue("server", "ServerPort", 0);
    MaxConn = ini.GetLongValue("server", "MaxConn", 1);
    LogConsoleLevel = ini.GetLongValue("log", "ConsoleLevel", 0);
    LogFileLevel = ini.GetLongValue("log", "FileLevel", 0);
}

} // namespace hw
