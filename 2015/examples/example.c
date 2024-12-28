#include <stdio.h>
#include <uv.h>

void on_timer(uv_timer_t* handle) {
    printf("Timer triggered!\n");
}

int main() {
    uv_loop_t *loop = uv_default_loop();

    uv_timer_t timer_req;
    uv_timer_init(loop, &timer_req);
    uv_timer_start(&timer_req, on_timer, 1000, 1000); // Fires every 1000ms

    printf("Starting libuv loop...\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    return 0;
}

// Compile the C program
// gcc -o example example.c -luv
// clang -o example example.c -luv

// Run the compiled program
// ./example
