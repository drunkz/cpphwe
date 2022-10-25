#ifndef HW_CONN_MGR_H
#define HW_CONN_MGR_H

#include "include/hw_interface.h"
#include "include/hw_internals.h"
#include "include/hw_singleton.h"
#include <mutex>
#include <uv.h>
#include <vector>

namespace hw {

class Conn {
  public:
    Conn();
    virtual ~Conn();

  public:
    uv_tcp_t* GetHandle();
    void Close();
    bool SetDataHandler(IDataHandler* handler);
    void DoReceive();

  public:
    ULONG IpAddr; // 客户端IP地址
    INT32 m_nDataLen;
    char RecvBuf[RECV_BUF_SIZE];

  private:
    uv_tcp_t m_handle;
    uv_shutdown_t m_ShutdownReq;
    IDataHandler* m_DataHandler;

    unsigned long long m_nConnData;
    unsigned int m_nConnID;
    IDataBuffer* m_pCurRecvBuffer;
    char* m_pBufPos;
    INT32 m_nCheckNo;
    volatile bool m_IsSending;
    IDataBuffer* m_pSendingBuffer;
};

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

class ConnMgr : public Singleton<ConnMgr> {
    friend class Singleton<ConnMgr>;

  public:
    bool SetMaxConn(unsigned int maxConn);
    Conn* NewConn();
    void RemoveConn(Conn* conn);

  private:
    ConnMgr();
    virtual ~ConnMgr();

  private:
    UINT32 m_MaxConn;
    std::vector<Conn*> m_ConnList;
    std::mutex m_ConnListMutex;
};

} // namespace hw

#endif // HW_CONN_MGR_H