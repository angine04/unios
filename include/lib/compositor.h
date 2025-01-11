#pragma once
#include <stdint.h>
#include <graphics.h>
#include <assets.h>
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
    bool     use_alpha;
} layer_t;

typedef struct {
    int x1, y1;  // top-left corner
    int x2, y2;  // bottom-right corner
} dirty_rect_t;


/*!
 * @brief Layer context
 */
typedef struct {
    layer_t layers[LAYER_MAX_NUM];
    int     layer_num;
    int     top_z_index;
    int     sorted_indices[LAYER_MAX_NUM];
    dirty_rect_t dirty_rect;
    bool    is_dirty;
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
int composite(layer_ctx_t *ctx);

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
    layer_ctx_t *ctx, int pos_x, int pos_y, int width, int height, int z_index);

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
int rect(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x,
    int          y,
    int          width,
    int          height,
    pixel_t      color);

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
int circle(
    layer_ctx_t *ctx,
    int          layer_index,
    int          center_x,
    int          center_y,
    int          radius,
    pixel_t      color);

/*!
 * @brief Blend two colors in gamma-correct linear space with alpha
 * @param color1 The first color
 * @param color2 The second color
 * @param alpha The alpha value
 * @return The blended color
 */
pixel_t blend(pixel_t color1, pixel_t color2, float alpha);

/*!
 * @brief Blend two colors using simple blending
 *
 * @attention This function is not gamma-correct but it's faster than blend()
 *
 * @param color1 The first color
 * @param color2 The second color
 * @param alpha The alpha value
 * @return The blended color
 */
pixel_t blend_simple(pixel_t color1, pixel_t color2, float alpha);

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
int triangle(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x1,
    int          y1,
    int          x2,
    int          y2,
    int          x3,
    int          y3,
    pixel_t      color);

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

/*!
 * @brief Use a image
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @param image_index The index of the image
 * @param scale The scale factor, possible value: 0.5, 1, 2
 * @return 0 on success, -1 on failure
 */
int use_image(layer_ctx_t *ctx, int layer_index, int image_index, float scale);

/*!
 * @brief Sort the layers by z index
 * @param ctx The compositor context
 */
void sort_layer(layer_ctx_t *ctx);

/*!
 * @brief Resize a layer to a new size
 *
 * @attention This function clears the layer buffer.
 *
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @param width The new width
 * @param height The new height
 * @return 0 on success, -1 on failure
 */
int resize(layer_ctx_t *ctx, int layer_index, int width, int height);

/*!
 * @brief Clear a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @return 0 on success, -1 on failure
 */
int clear(layer_ctx_t *ctx, int layer_index);

/*!
 * @brief Show a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @return 0 on success, -1 on failure
 */
int show(layer_ctx_t *ctx, int layer_index);

/*!
 * @brief Hide a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @return 0 on success, -1 on failure
 */
int hide(layer_ctx_t *ctx, int layer_index);

/*!
 * @brief Draw a rounded rectangle on a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @param x The x position of the rectangle
 * @param y The y position of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param radius The radius of the rectangle corners
 * @param color The color of the rectangle
 * @return 0 on success, -1 on failure
 */
int rounded_rect(layer_ctx_t *ctx, int layer_index, int x, int y, int width, int height, int radius, pixel_t color);

/*!
 * @brief Mark a region as dirty
 * @param ctx The compositor context
 * @param x The x position of the dirty region
 * @param y The y position of the dirty region
 * @param width The width of the dirty region
 * @param height The height of the dirty region
 */
void mark_dirty(layer_ctx_t *ctx, int x, int y, int width, int height);

/*!
 * @brief Draw a character on a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @param ch The character
 * @param scale The scale factor, possible value: 0.5, 1, 2
 * @param color The color of the character
 * @return 0 on success, -1 on failure
 */
int use_char(layer_ctx_t *ctx, int layer_index, char ch, float scale, pixel_t color);

/*!
 * @brief Draw a text on a layer
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @param text The text to draw
 * @param scale The scale factor, possible value: 0.5, 1, 2
 * @param color The color of the text
 * @return 0 on success, -1 on failure
 */
int use_text(layer_ctx_t *ctx, int layer_index, char *text, float scale, pixel_t color);

/*!
 * @brief Draw a stroke on a window border
 *
 * @attention This function is only for window border acceleration. Full
 * functionality is not supported yet.
 *
 * @param ctx The compositor context
 * @param layer_index The index of the layer
 * @param x The x position of the stroke
 * @param y The y position of the stroke
 * @param width The width of the stroke
 * @param height The height of the stroke
 * @param radius The radius of the stroke
 * @param color The color of the stroke
 * @return 0 on success, -1 on failure
 */
int rounded_rect_stroke(layer_ctx_t *ctx, int layer_index, int x, int y, int width, int height, int radius, pixel_t color);
