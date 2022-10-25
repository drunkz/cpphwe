#include "hw_net_mgr.h"
#include "include/hw_log.h"

namespace hw {

void _RunMainLoop(void* arg) {
    NetMgr* pNetNetMgr = (NetMgr*)arg;
    pNetNetMgr->RunMainLoop();
    return;
}

void _OnConnection(uv_stream_t* server, int status) {
    if (status < 0) {
        HW_LOG_ERROR("New connection error {}", uv_strerror(status));
        return;
    }
    Conn* conn = ConnMgr::GetInstance().NewConn();
    IF_TRUE_LOG_ERR_RET(conn == nullptr, "新建连接失败");
    uv_tcp_init(NetMgr::GetInstance().MainLoop, conn->GetHandle());

    if (uv_accept(server, (uv_stream_t*)conn->GetHandle()) == 0) {
        NetMgr::GetInstance().HandleAccept(conn);
    } else {
        conn->Close();
    }
}

void NetMgr::RunMainLoop() {
    HW_LOG_DEBUG("MainLoop Start");
    uv_run(MainLoop, UV_RUN_DEFAULT);
    return;
}

void NetMgr::HandleAccept(Conn* conn) {
    sockaddr_in clientAddr;
    socklen_t namelen = sizeof(clientAddr);
    uv_tcp_getpeername(conn->GetHandle(), (sockaddr*)&clientAddr, &namelen);
    conn->IpAddr = clientAddr.sin_addr.s_addr;
    conn->SetDataHandler(m_DataHandler);

    // dataHandler->OnNewConnect(pConn->GetConnID());

    conn->DoReceive();

    // 判断黑白名单等安全，否则关闭？
    // conn->Close();
}

bool NetMgr::Init(std::string& listenIP, UINT16 port, UINT32 maxConn, IDataHandler* dataHandler) {
    if (!ConnMgr::GetInstance().SetMaxConn(maxConn)) {
        return false;
    }
    m_DataHandler = dataHandler;
    MainLoop = uv_default_loop();
    uv_tcp_init(MainLoop, &m_TCPServer);
    sockaddr_in addr;
    uv_ip4_addr(listenIP.c_str(), port, &addr);
    uv_tcp_bind(&m_TCPServer, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&m_TCPServer, 20, _OnConnection);
    if (r) {
        HW_LOG_ERROR("Listen error {}", uv_strerror(r));
        return false;
    }
    uv_thread_create(&m_ThreadMainLoop, _RunMainLoop, this);
    return true;
}

} // namespace hw