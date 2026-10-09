#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

void client_connected(uv_stream_t* server, int status)
{
    printf("New client application found.\n");
    uv_stream_t *client = malloc(sizeof(uv_stream_t));
    uv_tcp_init(server->loop,(void *)client);

    uv_accept(server, client);
    printf("New client accepted.\n\n");
}

void callback_default(uv_handle_t* handle)
{
    printf("default callback\n");
}

int main(int argc, char* argv[])
{
    printf("Server starts with libuv version %s\n",uv_version_string());

    uv_loop_t *loop = uv_default_loop();
    uv_loop_init(loop);

    uv_tcp_t *tcp = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop,tcp);

    struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr("127.0.0.1", 1415, addr);
    uv_tcp_bind(tcp, (struct sockaddr*)addr,UV_TCP_REUSEPORT);

    uv_listen((uv_stream_t*)tcp, 69, client_connected);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    return 0;
}
