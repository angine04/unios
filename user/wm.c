#include <stdio.h>
#include <stdbool.h>
#include <graphics.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <compositor.h>
#include <malloc.h>

int main() {
    int pid = get_pid();

    layer_ctx_t* layer_ctx = malloc(sizeof(layer_ctx_t));
    compositor_init(layer_ctx);

    int layer_index = create_layer(layer_ctx, 100, 100, 500, 500, 1);
    assert(layer_index != -1);
    fill(layer_ctx, layer_index, COLOR_SILVER);
    rect(layer_ctx, layer_index, 5, 5, 490, 490, COLOR_LIGHTGREY);
    circle(layer_ctx, layer_index, 250, 250, 100, COLOR_BLUE);
    circle(layer_ctx, layer_index, 250, 250, 50, COLOR_RED);
    triangle(layer_ctx, layer_index, 100, 100, 700, 150, 250, 300, COLOR_GREEN);

    while (1) {
        int x = layer_ctx->layers[layer_index].pos_x;
        int y = layer_ctx->layers[layer_index].pos_y;
        // Check if window would go out of screen bounds
        if (x + 1 + layer_ctx->layers[layer_index].width > DISPLAY_WIDTH ||
            y + 1 + layer_ctx->layers[layer_index].height > DISPLAY_HEIGHT) {
            // Reset position to top left
            move(layer_ctx, layer_index, 0, 0);
        } else {
            move(layer_ctx, layer_index, x + 1, y + 1);
        }
        render(layer_ctx, pid);
        // sleep(1000);
    }

    unreachable();
    return 0;
}
