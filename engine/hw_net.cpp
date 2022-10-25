#include "hw_net.h"
#include "include/hw_log.h"

namespace hw {

namespace net {

uv_loop_t* loop;
uv_tcp_t server;

void OnClose(uv_handle_t* handle) {
    HW_LOG_DEBUG("断开连接");
    free(handle);
}

void echo_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
        write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        HW_LOG_DEBUG("收到数据");
        uv_write((uv_write_t*)req, client, &req->buf, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        HW_LOG_DEBUG("关闭成功2");
        uv_close((uv_handle_t*)client, OnClose);
    }

    free(buf->base);
}

void echo_write(uv_write_t* req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
}

void free_write_req(uv_write_t* req) {
    write_req_t* wr = (write_req_t*)req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void OnConnection(uv_stream_t* server, int status) {
    if (status < 0) {
        HW_LOG_ERROR("New connection error  {}", uv_strerror(status));
        return;
    }
    uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    // dkz:观察client释放情况
    uv_tcp_init(loop, client);
    // HW_LOG_DEBUG("连接成功");
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
    } else {
        HW_LOG_DEBUG("关闭成功1");
        uv_close((uv_handle_t*)client, OnClose);
    }
}

bool Net_Init(const char* sListenIP, UINT16 nPort, UINT16 nMaxConn) {
    hw::net::loop = uv_default_loop();
    uv_tcp_init(hw::net::loop, &server);
    struct sockaddr_in addr;
    uv_ip4_addr(sListenIP, nPort, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 1000, hw::net::OnConnection);
    if (r) {
        HW_LOG_ERROR("Listen error {}", uv_strerror(r));
        return false;
    }
    return true;
}

} // namespace net

} // namespace hw