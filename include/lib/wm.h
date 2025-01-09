#define MAX_CONTENTS 20
#define CURSOR_Z_INDEX 65535
#define DESKTOP_Z_INDEX 1


typedef struct wm_content_t{
    int x;
    int y;
    int width;
    int height;
    int z_index;
    int layer_index;
    bool callbackEnable;
    void (*bandFunction)(void);
    struct wm_window_t* belongWindow;
    bool dynamicSize;
} wm_content_t;//窗口带有的组件，x,y以窗口左上角位置为原点

typedef struct wm_windowNode {
    struct wm_windowNode* pre_wmN;
    struct wm_windowNode* next_wmN;
    struct wm_window_t* window;
}wm_windowNode;

typedef struct wm_window_t{
    int id;
    int pid;
    int x;
    int y;
    int width;
    int height;
    struct wm_content_t contents[MAX_CONTENTS];
    int layer_count;
    int w_z_index;
    int button_count;
} wm_window_t;//窗口，x,y表示窗口左上角位置


typedef struct wm_ctx_t{
    wm_windowNode* topWindow;
    wm_windowNode* bottomWindow;
    int window_count;
    layer_ctx_t* layer_ctx;
} wm_ctx_t;


/* ******************init*********************** */
/*!
 * @brief Initialize the window manager
 * @param ctx The window manager context
 */
void wm_init(wm_ctx_t *ctx);

/*!
 * @brief Initialize the cursor. the topWindow will be init.
 * @param ctx The window manager context
 */
void init_cursor(wm_ctx_t *ctx);

/*!
 * @brief Initialize the desktop. the bottomWindow will be init.
 * @param ctx The window manager context
 */
void init_desktop(wm_ctx_t *ctx);

/* ***************window operate***************** */
/*!
 * @brief Add a window to the window manager.Will set the window's z_index, id, sort the window's contents, and sort the layer
 * @param ctx The window manager context
 * @param window The window to add
 */
int wm_add_window(wm_ctx_t *ctx, wm_window_t* window);

/*!
 * @brief Remove a window from the window manager by id
 * @param ctx The window manager context
 * @param window_index The index of the window to remove
 */
int wm_remove_window(wm_ctx_t *ctx, int window_id);

/*!
 * @brief Remove top window from the window manager
 * @param ctx The window manager context
 * @param window_index The index of the window to remove
 */
int wm_remove_top_window(wm_ctx_t *ctx);

/*!
 * @brief 根据鼠标点击位置，更新用户最上层窗口change the top window
 * @param ctx The window manager context
 * @param cursor_x cursor x position
 * @param cursor_y cursor y position
 */
void wm_updateTopWindow(wm_ctx_t *ctx, int cursor_x, int cursor_y);


/*!
 * @brief Resize the Top USER window and components that belong it
 */
void wm_resizeWindows(wm_window_t* window, int newWidth, int newHeight);

layer_ctx_t* get_layer_ctx(wm_ctx_t* ctx);

wm_window_t* get_window_ctx(wm_ctx_t* ctx);
