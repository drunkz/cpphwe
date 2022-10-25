// clang-format off
#include "hw_net_mgr.h"
#include "include/hw_server_base.h"
#include "../common/system.h"
#include "include/hw_internals.h"
#include "include/hw_log.h"
// clang-format on

namespace hw {

bool ServerBase::Run(UINT16 initFlags) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (!hw::Log::GetInstance().Log_Init()) {
        return false;
    }
    HW_LOG_INFO("---------- 服务开始 ----------");
    if (!LoadCfg(&CfgServerBase, "config.ini")) {
        return false;
    }
    IF_TRUE_LOG_ERR_RET_FALSE(hw::IsAlreadyRun(CfgServerBase.ServerName), "不可重复运行");
    hw::Log::GetInstance().Log_SetLevel(CfgServerBase.LogFileLevel, CfgServerBase.LogConsoleLevel);
#ifdef _WIN32
    // 设置窗口标题，禁止关闭按钮。
    // size_t size = cfg_server_base.ServerName.length();
    // wchar_t *buffer = new wchar_t[size];
    // memset(buffer, 0, size * sizeof(wchar_t));
    // MultiByteToWideChar(CP_UTF8, 0, cfg_server_base.ServerName.c_str(), size, buffer, size *
    // sizeof(wchar_t)); SetConsoleTitleW(buffer); delete[] buffer;
    uv_set_process_title(CfgServerBase.ServerName.c_str());
    HMENU consoleMenu = GetSystemMenu(GetConsoleWindow(), false);
    RemoveMenu(consoleMenu, SC_CLOSE, 0);
#else
    // dkz:linux 初始化各种信号， ctrl + c 处理
#endif
    if (initFlags & kInitDefault) {
        initFlags = initFlags | kInitUV | kInitDB | kInitV8;
    }
    m_initFlags = initFlags;
    if (initFlags & kInitUV) {
        IF_TRUE_LOG_ERR_RET_FALSE(!hw::NetMgr::GetInstance().Init(CfgServerBase.ServerAddr, CfgServerBase.ServerPort, CfgServerBase.MaxConn, this),
                                  "网络监听初始化失败");
    }
    m_startTime = uv_hrtime();
    IF_TRUE_LOG_ERR_RET_FALSE(!OnStart(), "服务启动失败");
    // uv_run(hw::net::loop, UV_RUN_DEFAULT);
    return true;
}

void ServerBase::Close() {
    OnClose();
    HW_LOG_INFO("---------- 服务关闭 ----------");
    spdlog::shutdown();
}

bool ServerBase::OnDataHandle(IDataBuffer* dataBuffer, UINT32 connID) {
    return true;
}
bool ServerBase::OnCloseConnect(UINT32 connID) {
    return true;
}
bool ServerBase::OnNewConnect(UINT32 connID) {
    return true;
}

bool ServerBase::LoadCfg(Config* pConfig, const std::string& sFilename) {
    IF_TRUE_LOG_ERR_RET_FALSE(!FileExists(sFilename), "文件({})不存在", sFilename);
    IF_TRUE_LOG_ERR_RET_FALSE(!pConfig->Load(sFilename), "文件({})读取错误", sFilename);
    return true;
}

} // namespace hw
