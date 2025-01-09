#include <graphics.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <compositor.h>
#include <malloc.h>
#include <math.h>
#define USE_ASSETS_DATA

#include <assets_data.h>

static pixel_t screen_vram[DISPLAY_WIDTH * DISPLAY_HEIGHT] = {0};

// Sort layers by z-index and return sorted indices
static int sort_layers_by_z(layer_ctx_t *ctx) {
    int sorted_count = 0;

    // Collect indices of layers in use
    for (int i = 0; i < LAYER_MAX_NUM; i++) {
        if (ctx->layers[i].in_use) { ctx->sorted_indices[sorted_count++] = i; }
    }

    // Sort indices by z-index using insertion sort
    for (int i = 1; i < sorted_count; i++) {
        int key   = ctx->sorted_indices[i];
        int key_z = ctx->layers[key].z_index;
        int j     = i - 1;

        while (j >= 0 && ctx->layers[ctx->sorted_indices[j]].z_index > key_z) {
            ctx->sorted_indices[j + 1] = ctx->sorted_indices[j];
            j--;
        }
        ctx->sorted_indices[j + 1] = key;
    }

    return sorted_count;
}

int compositor_init(layer_ctx_t *ctx) {
    ctx->layer_num = 0;
    for (int i = 0; i < LAYER_MAX_NUM; i++) {
        ctx->layers[i].in_use    = false;
        ctx->layers[i].visible   = false;
        ctx->layers[i].pos_x     = 0;
        ctx->layers[i].pos_y     = 0;
        ctx->layers[i].width     = 0;
        ctx->layers[i].height    = 0;
        ctx->layers[i].buf       = NULL;
        ctx->layers[i].z_index   = 0;
        ctx->layers[i].use_alpha = false;
    }

    int background_layer_index =
        create_layer(ctx, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
    fill(ctx, background_layer_index, 0x705E56);
    int pid = get_pid();
    render(ctx, pid);

    return 0;
}

int render(layer_ctx_t *ctx, int pid) {
    composite(ctx);
    putframe(
        screen_vram, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(pixel_t), pid);
    return 0;
}

int composite(layer_ctx_t *ctx) {
    // Create array of layer indices sorted by z-index
    int sorted_count = sort_layers_by_z(ctx);

    ctx->top_z_index =
        ctx->layers[ctx->sorted_indices[sorted_count - 1]].z_index;

    // Clear screen buffer
    // memset(screen_vram, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(pixel_t));

    // composite layers in z-index order
    for (int i = 0; i < sorted_count; i++) {
        layer_t *layer = &ctx->layers[ctx->sorted_indices[i]];
        if (!layer->visible) continue;

        for (int y = 0; y < layer->height; y++) {
            for (int x = 0; x < layer->width; x++) {
                int screen_x = layer->pos_x + x;
                int screen_y = layer->pos_y + y;

                if (screen_x >= 0 && screen_x < DISPLAY_WIDTH && screen_y >= 0
                    && screen_y < DISPLAY_HEIGHT) {
                    pixel_t src_pixel = layer->buf[y * layer->width + x];
                    pixel_t dst_pixel =
                        screen_vram[screen_y * DISPLAY_WIDTH + screen_x];

                    // Get alpha value from highest 8 bits
                    uint8_t alpha = (src_pixel >> 24) & 0xFF;

                    if (alpha == 0x00) {
                        // Fully opaque
                        screen_vram[screen_y * DISPLAY_WIDTH + screen_x] =
                            src_pixel;
                    } else if (alpha != 0xFF) {
                        // Partially transparent - blend colors
                        float   transparency = alpha / 255.0f;
                        pixel_t blended =
                            blend(src_pixel, dst_pixel, 1.0f - transparency);
                        uint8_t r = (blended >> 16) & 0xFF;
                        uint8_t g = (blended >> 8) & 0xFF;
                        uint8_t b = blended & 0xFF;

                        screen_vram[screen_y * DISPLAY_WIDTH + screen_x] =
                            (r << 16) | (g << 8) | b;
                    }
                }
            }
        }
    }

    return 0;
}

int create_layer(
    layer_ctx_t *ctx,
    int          pos_x,
    int          pos_y,
    int          width,
    int          height,
    int          z_index) {
    int layer_index = -1;

    if (ctx->layer_num >= LAYER_MAX_NUM) {
        // layer num is full
        return -1;
    }

    for (int i = 0; i < LAYER_MAX_NUM; i++) {
        if (!ctx->layers[i].in_use) {
            layer_index           = i;
            ctx->layers[i].in_use = true;
            break;
        }
    }

    if (layer_index == -1) {
        // no free layer found
        return -1;
    }

    pixel_t *layer_buf = malloc(width * height * 4);
    if (!layer_buf) {
        // malloc failed
        return -1;
    }

    // clip position and dimensions to display bounds
    int clipped_x = pos_x < 0 ? 0 : pos_x;
    int clipped_y = pos_y < 0 ? 0 : pos_y;
    int clipped_width =
        clipped_x + width > DISPLAY_WIDTH ? DISPLAY_WIDTH - clipped_x : width;
    int clipped_height = clipped_y + height > DISPLAY_HEIGHT
                           ? DISPLAY_HEIGHT - clipped_y
                           : height;

    ctx->layers[layer_index].visible = true;
    ctx->layers[layer_index].pos_x   = clipped_x;
    ctx->layers[layer_index].pos_y   = clipped_y;
    ctx->layers[layer_index].width   = clipped_width;
    ctx->layers[layer_index].height  = clipped_height;
    ctx->layers[layer_index].buf     = layer_buf;
    ctx->layers[layer_index].z_index = z_index;
    ctx->layer_num++;

    sort_layers_by_z(ctx);
    return layer_index;
}

int release_layer(layer_ctx_t *ctx, int index) {
    ctx->layers[index].in_use  = false;
    ctx->layers[index].visible = false;
    ctx->layers[index].pos_x   = 0;
    ctx->layers[index].pos_y   = 0;
    ctx->layers[index].width   = 0;
    ctx->layers[index].height  = 0;
    free(ctx->layers[index].buf);
    ctx->layers[index].buf     = NULL;
    ctx->layers[index].z_index = 0;
    ctx->layer_num--;
    sort_layers_by_z(ctx);
    return 0;
}

int fill(layer_ctx_t *ctx, int layer_index, pixel_t color) {
    layer_t *layer = &ctx->layers[layer_index];
    for (int i = 0; i < layer->width * layer->height; i++) {
        layer->buf[i] = color;
    }
    return 0;
}

int rect(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x,
    int          y,
    int          width,
    int          height,
    pixel_t      color) {
    layer_t *layer = &ctx->layers[layer_index];
    // clip the rect to layer bounds
    int x1 = x < 0 ? 0 : x;
    int y1 = y < 0 ? 0 : y;
    int x2 = x + width > layer->width ? layer->width : x + width;
    int y2 = y + height > layer->height ? layer->height : y + height;

    // draw the clipped rect
    for (int row = y1; row < y2; row++) {
        for (int col = x1; col < x2; col++) {
            layer->buf[row * layer->width + col] = color;
        }
    }
    return 0;
}

int move(layer_ctx_t *ctx, int layer_index, int x, int y) {
    ctx->layers[layer_index].pos_x = x;
    ctx->layers[layer_index].pos_y = y;
    return 0;
}

float distance(int x1, int y1, int x2, int y2) {
    return sqrtf((float)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Blend two colors in gamma-correct linear space with alpha
pixel_t blend(pixel_t color1, pixel_t color2, float alpha) {
    // Convert to linear space
    float r1 = powf(((color1 >> 16) & 0xFF) / 255.0f, 2.2f);
    float g1 = powf(((color1 >> 8) & 0xFF) / 255.0f, 2.2f);
    float b1 = powf((color1 & 0xFF) / 255.0f, 2.2f);

    float r2 = powf(((color2 >> 16) & 0xFF) / 255.0f, 2.2f);
    float g2 = powf(((color2 >> 8) & 0xFF) / 255.0f, 2.2f);
    float b2 = powf((color2 & 0xFF) / 255.0f, 2.2f);

    // Blend in linear space
    float r = r1 * alpha + r2 * (1.0f - alpha);
    float g = g1 * alpha + g2 * (1.0f - alpha);
    float b = b1 * alpha + b2 * (1.0f - alpha);

    // Convert back to sRGB space
    uint8_t r_out = (uint8_t)(powf(r, 1.0f / 2.2f) * 255.0f);
    uint8_t g_out = (uint8_t)(powf(g, 1.0f / 2.2f) * 255.0f);
    uint8_t b_out = (uint8_t)(powf(b, 1.0f / 2.2f) * 255.0f);

    return (r_out << 16) | (g_out << 8) | b_out;
}

// Blend two colors with alpha using simple blending
pixel_t blend_simple(pixel_t color1, pixel_t color2, float alpha) {
    // Extract RGB components
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;

    // Simple linear blend
    uint8_t r = (uint8_t)(r1 * alpha + r2 * (1.0f - alpha));
    uint8_t g = (uint8_t)(g1 * alpha + g2 * (1.0f - alpha));
    uint8_t b = (uint8_t)(b1 * alpha + b2 * (1.0f - alpha));

    return (r << 16) | (g << 8) | b;
}

int circle(
    layer_ctx_t *ctx,
    int          layer_index,
    int          center_x,
    int          center_y,
    int          radius,
    pixel_t      color) {
    layer_t *layer = &ctx->layers[layer_index];
    for (int y = 0; y < layer->height; y++) {
        for (int x = 0; x < layer->width; x++) {
            float dist = distance(x, y, center_x, center_y);
            if (dist <= radius) {
                // Calculate alpha based on distance from edge
                float alpha = 1.0f;
                if (dist > radius - 1.0f) { alpha = radius - dist; }

                layer->buf[y * layer->width + x] =
                    blend(color, layer->buf[y * layer->width + x], alpha);
            }
        }
    }
    return 0;
}

int triangle(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x1,
    int          y1,
    int          x2,
    int          y2,
    int          x3,
    int          y3,
    pixel_t      color) {
    layer_t *layer = &ctx->layers[layer_index];

    // Find bounding box
    int min_x = min(min(x1, x2), x3);
    int max_x = max(max(x1, x2), x3);
    int min_y = min(min(y1, y2), y3);
    int max_y = max(max(y1, y2), y3);

    // Clip to layer bounds
    min_x = max(0, min_x);
    max_x = min(layer->width - 1, max_x);
    min_y = max(0, min_y);
    max_y = min(layer->height - 1, max_y);

    // Edge vectors
    int e1_x = x2 - x1;
    int e1_y = y2 - y1;
    int e2_x = x3 - x2;
    int e2_y = y3 - y2;
    int e3_x = x1 - x3;
    int e3_y = y1 - y3;

    // MSAA sample offsets (4x MSAA)
    const float sample_offsets[4][2] = {
        {0.25f, 0.25f},
        {0.75f, 0.25f},
        {0.25f, 0.75f},
        {0.75f, 0.75f}
    };

    // For each pixel in bounding box
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int samples_inside = 0;

            // Test each sample point
            for (int s = 0; s < 4; s++) {
                float sample_x = x + sample_offsets[s][0];
                float sample_y = y + sample_offsets[s][1];

                // Test if sample point is inside triangle using edge function
                float d1 = (sample_x - x1) * e1_y - (sample_y - y1) * e1_x;
                float d2 = (sample_x - x2) * e2_y - (sample_y - y2) * e2_x;
                float d3 = (sample_x - x3) * e3_y - (sample_y - y3) * e3_x;

                bool inside = (d1 >= 0 && d2 >= 0 && d3 >= 0)
                           || (d1 <= 0 && d2 <= 0 && d3 <= 0);
                if (inside) { samples_inside++; }
            }

            if (samples_inside > 0) {
                float alpha = samples_inside / 4.0f;
                layer->buf[y * layer->width + x] =
                    blend(color, layer->buf[y * layer->width + x], alpha);
            }
        }
    }

    return 0;
}

int top(layer_ctx_t *ctx, int layer_index) {
    ctx->layers[layer_index].z_index = ctx->top_z_index + 1;
    ctx->top_z_index++;
    sort_layers_by_z(ctx);
    return 0;
}

int get_top_z_index(layer_ctx_t *ctx) {
    return ctx->top_z_index;
}

int use_resource(
    layer_ctx_t *ctx, int layer_index, int resource_index, int stretch) {
    layer_t *layer = &ctx->layers[layer_index];
    if (layer->width < resources[resource_index].width
        || layer->height < resources[resource_index].height) {
        return -1;
    }
    for (int y = 0; y < resources[resource_index].height; y++) {
        for (int x = 0; x < resources[resource_index].width; x++) {
            int offset = y * resources[resource_index].width + x;
            if (stretch >= 2) {
                rect(
                    ctx,
                    layer_index,
                    x * stretch,
                    y * stretch,
                    stretch,
                    stretch,
                    resources[resource_index].buf[offset]);
            } else {
                layer->buf[offset] = resources[resource_index].buf[offset];
            }
        }
    }
    return 0;
}

void sort_layer(layer_ctx_t *ctx) {
    sort_layers_by_z(ctx);
}

int resize(layer_ctx_t *ctx, int layer_index, int width, int height) {
    layer_t *layer = &ctx->layers[layer_index];
    pixel_t *old_buf = layer->buf;
    layer->buf = malloc(width * height * 4);
    if (!layer->buf) {
        return -1;
    }
    layer->width = width;
    layer->height = height;
    memset(layer->buf, 0, width * height * 4);
    free(old_buf);
    return 0;
}

int clear(layer_ctx_t *ctx, int layer_index) {
    fill(ctx, layer_index, COLOR_TRANSPARENT);
    return 0;
}

int show(layer_ctx_t *ctx, int layer_index) {
    ctx->layers[layer_index].visible = true;
    return 0;
}

int hide(layer_ctx_t *ctx, int layer_index) {
    ctx->layers[layer_index].visible = false;
    return 0;
}
