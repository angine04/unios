#pragma once
#include <stdint.h>
#include <graphics.h>

#define LAYER_MAX_NUM 100

/*!
 * @brief Layer structure
 */
typedef struct {
    bool     visible;
    bool     in_use;
    int      pos_x;
    int      pos_y;
    int      width;
    int      height;
    pixel_t *buf;
    int      z_index;
    float    opacity;
} layer_t;

/*!
 * @brief Layer context
 */
typedef struct {
    layer_t layers[LAYER_MAX_NUM];
    int     layer_num;
    int     top_z_index;
    int     sorted_indices[LAYER_MAX_NUM];
} layer_ctx_t;

/*!
 * @brief Initialize the compositor
 * @param [out] ctx The compositor context
 * @return 0 on success, -1 on failure
 */
int compositor_init(layer_ctx_t *ctx);

/*!
 * @brief Render the compositor
 * @param ctx The compositor context
 * @param pid The PID of the process to render
 * @return 0 on success, -1 on failure
 */
int render(layer_ctx_t *ctx, int pid);

/*!
 * @brief bake the layers into single frame
 * @param ctx The compositor context
 * @return 0 on success, -1 on failure
 */
int compose(layer_ctx_t *ctx);

/*!
 * @brief Create a new layer
 * @param ctx The compositor context
 * @param pos_x The x position of the layer
 * @param pos_y The y position of the layer
 * @param width The width of the layer
 * @param height The height of the layer
 * @param opacity The opacity of the layer
 * @param z_index The z index of the layer
 * @return index of the layer, -1 on failure
 */
int create_layer(
    layer_ctx_t *ctx,
    int          pos_x,
    int          pos_y,
    int          width,
    int          height,
    int          z_index);

/*!
 * @brief Release a layer
 * @param ctx The compositor context
 * @param index The index of the layer to release
 * @return 0 on success, -1 on failure
 */
int release_layer(layer_ctx_t *ctx, int index);

/*!
 * @brief Fill a layer with a color
 * @param ctx The compositor context
 * @param index The index of the layer to fill
 * @param color The color to fill the layer with
 * @return 0 on success, -1 on failure
 */
int fill(layer_ctx_t *ctx, int layer_index, pixel_t color);

/*!
 * @brief Draw a rectangle on a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer to draw on
 * @param x The x position of the rectangle
 * @param y The y position of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 * @return 0 on success, -1 on failure
 */
int rect(layer_ctx_t *ctx, int layer_index, int x, int y, int width, int height, pixel_t color);

/*!
 * @brief Move a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer to move
 * @param x The x position of the layer
 * @param y The y position of the layer
 * @return 0 on success, -1 on failure
 */
int move(layer_ctx_t *ctx, int layer_index, int x, int y);

/*!
 * @brief Draw a circle on a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer to draw on
 * @param x The x position of the circle
 * @param y The y position of the circle
 * @param radius The radius of the circle
 * @param color The color of the circle
 * @return 0 on success, -1 on failure
 */
int circle(layer_ctx_t *ctx, int layer_index, int center_x, int center_y, int radius, pixel_t color);

/*!
 * @brief Blend two colors in gamma-correct linear space with alpha
 * @param color1 The first color
 * @param color2 The second color
 * @param alpha The alpha value
 * @return The blended color
 */
pixel_t blend(pixel_t color1, pixel_t color2, float alpha);

/*!
 * @brief Draw a triangle on a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer to draw on
 * @param x1 The x position of the first vertex
 * @param y1 The y position of the first vertex
 * @param x2 The x position of the second vertex
 * @param y2 The y position of the second vertex
 * @param x3 The x position of the third vertex
 * @param y3 The y position of the third vertex
 * @param color The color of the triangle
 * @return 0 on success, -1 on failure
 */
int triangle(layer_ctx_t *ctx, int layer_index, int x1, int y1, int x2, int y2, int x3, int y3, pixel_t color);

/*!
 * @brief put a layer on top
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @return The z index of the layer
 */
int top(layer_ctx_t *ctx, int layer_index);

/*!
 * @brief get the top z index
 * @param ctx The compositor context
 * @return The top z index
 */
int get_top_z_index(layer_ctx_t *ctx);

int use_icon_32(layer_ctx_t *ctx, int layer_index, int icon_index);
