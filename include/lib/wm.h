#define MAX_LAYERS 10
#define MAX_WINDOWS 5
#define MAX_BUTTONS 10

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int z_index;
    int layer_index;
    int affliated_window;
} wm_button_t;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int layer_indices[MAX_LAYERS];
    int layer_count;
    int z_index;
    int button_count;
    wm_button_t buttons[MAX_BUTTONS];
} wm_window_t;

typedef struct {
    wm_window_t windows[MAX_WINDOWS];
    int window_count;
    layer_ctx_t* layer_ctx;
} wm_ctx_t;



/*!
 * @brief Initialize the window manager
 * @param ctx The window manager context
 */
void wm_init(wm_ctx_t *ctx);

/*!
 * @brief Add a window to the window manager
 * @param ctx The window manager context
 * @param window The window to add
 */
void wm_add_window(wm_ctx_t *ctx, wm_window_t *window);

/*!
 * @brief Remove a window from the window manager
 * @param ctx The window manager context
 * @param window_index The index of the window to remove
 */
void wm_remove_window(wm_ctx_t *ctx, int window_index);

/*!
 * @brief Update display
 * @param ctx The window manager context
 */
void wm_update(wm_ctx_t *ctx);

/*!
 * @brief Initialize the desktop
 * @param ctx The window manager context
 */
void init_desktop(wm_ctx_t *ctx);
