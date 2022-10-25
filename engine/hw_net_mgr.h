#ifndef HW_NET_MGR_H
#define HW_NET_MGR_H

#include "hw_conn_mgr.h"
#include "include/hw_interface.h"
#include "include/hw_singleton.h"
#include <basetsd.h>

namespace hw {
class NetMgr : public Singleton<NetMgr> {
    friend class Singleton<NetMgr>;

  public:
    bool Init(std::string& listenIP, UINT16 port, UINT32 maxConn, IDataHandler* dataHandler);
    void RunMainLoop();
    void HandleAccept(Conn* conn);

  private:
    NetMgr() = default;
    virtual ~NetMgr() = default;

  public:
    uv_loop_t* MainLoop; // uv主循环

  private:
    uv_tcp_t m_TCPServer;         // uv_tcp服务
    uv_thread_t m_ThreadMainLoop; // uv主循环线程
    IDataHandler* m_DataHandler;  // 数据分发
};
} // namespace hw

#endif // HW_NET_MGR_H