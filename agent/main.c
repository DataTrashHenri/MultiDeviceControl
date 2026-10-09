#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>


struct sockaddr_in *destination;
uv_tcp_t *client_socket;
uv_connect_t* connection;
uv_loop_t *loop;

bool ISCONNECTED = false;
bool ISCONNECTING = false;

void on_disconnect(uv_handle_t * handle) {
    printf("disconnected, will retry\n\n");
    ISCONNECTING = false;
    ISCONNECTED = false;
}

void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    if (nread > 0) {
        printf("read %.*s bytes\n",(int)nread, buf->base);
    }
    else if (nread < 0) {
        printf("connection lost %s\n",uv_strerror((int)nread));
        uv_close((uv_handle_t*)stream, on_disconnect);
    }
    else if (nread == 0) {
        printf("connection closed\n");
        ISCONNECTED = false;
    }

}

void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}
void on_connect(uv_connect_t* req,const int status)
{
    if (status != 0) {
        printf("couldn't connect: %s (%d)\n", uv_err_name(status), status);
        uv_close((uv_handle_t *)client_socket, on_disconnect);
        return;
    }
    printf("Connected successfully to host server\n\n");
    ISCONNECTED = true;
    int r = uv_read_start((uv_stream_t *)client_socket, alloc_cb, read_cb);
    if (r < 0) {
        printf("read_start failed: %s\n", uv_strerror(r));
        uv_close((uv_handle_t *)client_socket, on_disconnect);
    }
}

void auto_connect(uv_timer_t* handle) // every second called regardless
{
    if (ISCONNECTED || ISCONNECTING) return;
    ISCONNECTING = true;
    printf("auto_connect\n");

    //re-init tcp needed before new connection attempt
    uv_tcp_init(loop,client_socket);
    if (uv_tcp_connect(connection, client_socket, (const struct sockaddr*)destination, on_connect)<0)
    {
        printf("connection failed right away.\n");
        uv_close((uv_handle_t*)client_socket, on_disconnect);
    }
}


int main(int argc, char* argv[])
{
    loop = uv_default_loop();
    uv_loop_init(loop);

    connection = malloc(sizeof(uv_connect_t));
    client_socket = malloc(sizeof(uv_tcp_t));

    destination = malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr("127.0.0.1", 1415, destination);

    uv_timer_t *auto_login_timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop,auto_login_timer);
    uv_timer_start(auto_login_timer,auto_connect,1000,1000);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    free(client_socket);
    free(connection);
    free(destination);
    return 0;
}
