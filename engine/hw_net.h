#ifndef HW_NET_H
#define HW_NET_H

#include <basetsd.h>
#include <uv.h>

namespace hw {

namespace net {

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

extern uv_loop_t* loop;
void OnConnection(uv_stream_t* server, int status);
void OnClose(uv_handle_t* handle);
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void echo_write(uv_write_t* req, int status);
void echo_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
void free_write_req(uv_write_t* req);
bool Net_Init(const char* sListenIP, UINT16 nPort, UINT16 nMaxConn);

} // namespace net

} // namespace hw

#endif // HW_NET_H