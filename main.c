#include <stdio.h>
#include <stdlib.h>
#include <uv.h>


void fire_timer(uv_timer_t *timer) {
    printf("kaboom!\n");
}
void write_cb(uv_write_t* req, int status ) {
    printf("uv_write_cb done\n");
}
void client_connected(uv_stream_t *server, int status) {
    uv_stream_t client = {0};
    uv_tcp_init(server->loop,(void *) &client);

    //accepts!!
    uv_accept(server, &client);
    printf("connected!\n");

    // writes string to a client connected on connection, nice!
    uv_write_t req ={0};
    const uv_buf_t buf = uv_buf_init("hello world\n",13);
    uv_write(&req, &client, &buf, 1,write_cb);
}

int main(void) {
    printf("version %s\n",uv_version_string());
    int err;

    uv_loop_t *loop = uv_default_loop();
    // uv_timer_t timer = {0};

    // uv_timer_init(loop, &timer);
    uv_loop_init(loop);

    // uv_timer_start(&timer,fire_timer,1000,1000);


    //initial tcp setup
    uv_tcp_t tcp = {0};
    uv_tcp_init(loop, &tcp);

    //binding address/setup
    struct sockaddr_in addr = {0};
    uv_ip4_addr("127.0.0.1", 6969, &addr);
    uv_tcp_bind(&tcp, (struct sockaddr*)&addr,UV_TCP_REUSEPORT);

    //tells which port to listen to AND what to do on connect
    uv_listen((uv_stream_t*)&tcp, 69, client_connected);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    uv_library_shutdown();
    return 0;
}