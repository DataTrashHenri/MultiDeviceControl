#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>


void fire_timer(uv_timer_t *timer) {
    printf("kaboom!\n");
}
void write_cb(uv_write_t* req, int status ) {
    printf("uv_write_cb done\n");
}
uv_stream_t *setup_client(uv_stream_t *server) {
    printf("setup_client\n");
    uv_stream_t *client = malloc(sizeof(uv_stream_t));
    uv_tcp_init(server->loop,(void *)client);

    uv_accept(server, client);
    printf("connected to new client\n");
    return client;
}
void write_to_client(uv_stream_t *client, char* data) {
    printf("write_to_client\n");
    uv_write_t *req = malloc(sizeof(uv_write_t));
    const uv_buf_t buf = uv_buf_init(data,strlen(data));
    uv_write(req, client, &buf, 1,write_cb);
}
void client_connected(uv_stream_t *server, int status) {

    uv_stream_t *client = setup_client(server);

    // writes string to a client connected on connection, nice!

    char msg[] = "Host accepted your connection";
    write_to_client(client, msg);

    //uv_close((uv_handle_t *) client, nullptr);
}
uv_tcp_t *init_tcp(uv_loop_t *loop) {
    uv_tcp_t *tcp = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, tcp);
    return tcp;
}
void create_and_bin_addr(uv_tcp_t *tcp, const char *ip, const uint16_t port) {
    struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr(ip, port, addr);
    uv_tcp_bind(tcp, (struct sockaddr*)addr,UV_TCP_REUSEPORT);
}
int main(void) {
    printf("version %s\n",uv_version_string());

    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    //initial tcp setup
    uv_tcp_t *tcp = init_tcp(loop);

    //binding address/setup
    create_and_bin_addr(tcp,"127.0.0.1",6969);

    //tells which port to listen to AND what to do on connect
    uv_listen((uv_stream_t*)tcp, 69, client_connected);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    uv_library_shutdown();
    return 0;
}