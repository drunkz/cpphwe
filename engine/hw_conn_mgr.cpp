#include "hw_conn_mgr.h"
#include "include/hw_log.h"

namespace hw {

void _OnShutdown(uv_shutdown_t* req, int status) {
    Conn* conn = (Conn*)req->data;
    // Conn* conn = (Conn*)uv_handle_get_data(handle);
    if (status == 0) {
        // 成功
    } else {
        // 失败
    }
    HW_LOG_DEBUG("连接Shutdown");
}

void _OnClose(uv_handle_t* handle) {
    Conn* conn = (Conn*)handle->data;
    // Conn* conn = (Conn*)uv_handle_get_data(handle);
    ConnMgr::GetInstance().RemoveConn(conn);
    delete conn;
    HW_LOG_DEBUG("连接Close");
}

void _AllocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    // Conn* conn = (Conn*)uv_handle_get_data(handle);
    // buf->base = conn->RecvBuf + conn->m_nDataLen;
    // buf->len = RECV_BUF_SIZE - conn->m_nDataLen;
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void _free_write_req(uv_write_t* req) {
    write_req_t* wr = (write_req_t*)req;
    free(wr->buf.base);
    free(wr);
}

void _echo_write(uv_write_t* req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    _free_write_req(req);
}

void _OnReadData(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    // Conn* conn = (Conn*)uv_handle_get_data((uv_handle_t*)stream);
    // if (nread >= 0) {
    //     // conn->HandReaddata(nread);
    //     return;
    // }
    // conn->Close();
    if (nread > 0) {
        write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        HW_LOG_DEBUG("收到数据");
        uv_write((uv_write_t*)req, stream, &req->buf, 1, _echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        HW_LOG_DEBUG("关闭成功2");
        uv_close((uv_handle_t*)stream, _OnClose);
    }

    free(buf->base);
}

Conn::Conn() {
    m_DataHandler = nullptr;
    m_nDataLen = 0;
    m_nConnData = 0;
    m_nConnID = 0;
    m_pCurRecvBuffer = nullptr;
    m_pBufPos = RecvBuf;
    m_nCheckNo = 0;
    m_IsSending = false;
    m_pSendingBuffer = nullptr;
    m_handle.data = this;
}

Conn::~Conn() {
}

uv_tcp_t* Conn::GetHandle() {
    return &m_handle;
}

bool Conn::SetDataHandler(IDataHandler* handler) {
    m_DataHandler = handler;
    return true;
}

void Conn::DoReceive() {
    uv_read_start((uv_stream_t*)&m_handle, _AllocBuffer, _OnReadData);
}

void Conn::Close() {
    if (m_DataHandler != nullptr) {
        m_DataHandler->OnCloseConnect(m_nConnID);
    }
    uv_read_stop((uv_stream_t*)&m_handle);
    m_ShutdownReq.data = (void*)this;
    uv_shutdown(&m_ShutdownReq, (uv_stream_t*)&m_handle, _OnShutdown);
    uv_close((uv_handle_t*)&m_handle, _OnClose);
    // m_nDataLen = 0;
    // m_IsSending = false;
}

ConnMgr::ConnMgr() {
    m_MaxConn = 0;
}

ConnMgr::~ConnMgr() {
}

bool ConnMgr::SetMaxConn(UINT32 maxConn) {
    IF_TRUE_LOG_ERR_RET_FALSE(maxConn == 0, "最大连接数不可为0");
    m_MaxConn = maxConn;
    return true;
}

Conn* ConnMgr::NewConn() {
    m_ConnListMutex.lock();
    if (m_ConnList.size() == m_MaxConn) {
        m_ConnListMutex.unlock();
        HW_LOG_INFO("连接数已达上限{}", m_MaxConn);
        return nullptr;
    }
    Conn* connNew = new Conn();
    m_ConnList.push_back(connNew);
    HW_LOG_DEBUG("新连接加入，当前总连接数：{}", m_ConnList.size());
    m_ConnListMutex.unlock();
    return connNew;
}

void ConnMgr::RemoveConn(Conn* conn) {
    m_ConnListMutex.lock();
    std::vector<Conn*>::iterator it = m_ConnList.begin();
    while (it != m_ConnList.end()) {
        if (*it == conn) {
            HW_LOG_DEBUG("找到连接，已从 m_ConnList 删除");
            it = m_ConnList.erase(it);
        } else {
            it++;
        }
    }
    HW_LOG_DEBUG("当前总连接数：{}", m_ConnList.size());
    m_ConnListMutex.unlock();
}

} // namespace hw