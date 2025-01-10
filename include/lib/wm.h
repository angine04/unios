/* ******************wm*********************** */
#define MAX_CONTENTS 20
#define CURSOR_Z_INDEX 65535
#define DESKTOP_Z_INDEX 1

#define HIT_TOP_WINDOW 0
#define HIT_OTHER_WINDOW 1
#define HIT_DESKTOP 2


// Forward declarations to handle circular dependencies
struct wm_window_t;
struct wm_content_t;
struct wm_windowNode;

// Forward declare content struct
typedef struct wm_content_t wm_content_t;
typedef struct wm_window_t wm_window_t;
typedef struct wm_windowNode wm_windowNode;

struct wm_content_t {
    int x;
    int y;
    int width;
    int height;
    int z_index;
    int layer_index;
    bool callbackEnable;
    void (*bandFunction)(wm_window_t*);
    wm_window_t* belongWindow;
    bool dynamicSize;
};//窗口带有的组件，x,y以窗口左上角位置为原点


struct wm_window_t {
    int id;
    int pid;
    int x;
    int y;
    int width;
    int height;
    wm_content_t contents[MAX_CONTENTS];
    int layer_count;
    int w_z_index;
    int button_count;

    bool is_full_screen;
    int old_width;
    int old_height;
};//窗口，x,y表示窗口左上角位置



struct wm_windowNode {
    wm_windowNode* pre_wmN;
    wm_windowNode* next_wmN;
    wm_window_t* window;
};


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
int wm_updateTopWindow(wm_ctx_t *ctx, int cursor_x, int cursor_y);


/*!
 * @brief Resize the Top USER window and components that belong it
 */
void wm_full_screen(wm_ctx_t* ctx);

layer_ctx_t* get_layer_ctx(wm_ctx_t* ctx);

wm_window_t* get_window_ctx();

void wm_move_top_window(wm_ctx_t* ctx);






/* ******************GUI*********************** */
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MIN_WIDGET_WIDTH 10
#define MIN_WIDGET_HEIGHT 10

void GUI_init(wm_ctx_t* ctx);
/*************基础控件****************** */
/*!
 * @brief Create a window to the window manager.
 * @param x The x coordinate of the window
 * @param y The y coordinate of the window
 * @param width The width of the window
 * @param height The height of the window
 * @param window The window return to user
 * @return is success
 */
wm_window_t* ui_create_widget(int x, int y, int width, int height);
/*!
 * @brief Create a button to the window manager.
 * @param x The x position (accrording to window) of the button
 * @param y The y position (accrording to window) of the button
 * @param width The width of the button
 * @param height The height of the button
 * @param z_index The z_index of the button
 * @param text The text of the button
 * @param callback The callback function of the button
 * @param window The window to add the button
 * @return button id
 */
int ui_create_button(int x, int y, int width, int height, int z_index, char *text, void (*callback)(wm_window_t*), wm_window_t* window);
/*!
 * @brief Create a label to the window manager.
 * @param x The x position (accrording to window) of the label
 * @param y The y position (accrording to window) of the label
 * @param width The width of the label
 * @param height The height of the label
 * @param text The text of the label
 * @param window The window to add the label
 * @return label id
 */
int ui_create_label(int x, int y, int width, int height, int z_index, char *text, wm_window_t* window);
/*!
 * @brief Create a image to the window manager.
 * @param x The x coordinate of the image
 * @param y The y coordinate of the image
 * @param width The width of the image
 * @param height The height of the image
 * @param image_path The path of the image
 * @param window The window to add the image
 * @return image id
 */
int ui_create_image(int x, int y, int width, int height, int z_index, char *image_path, wm_window_t* window);
/*!
 * @brief Create a textbox to the window manager.
 * @param x The x coordinate of the textbox
 * @param y The y coordinate of the textbox
 * @param width The width of the textbox
 * @param height The height of the textbox
 * @param text The text of the textbox
 * @param window The window to add the textbox
 * @return textbox id
 */
int ui_create_textbox(int x, int y, int width, int height, char *text, wm_window_t* window);

/*************事件处理****************** */




void ui_show(wm_window_t* window);
void ui_hide(wm_window_t* window);
void ui_full_screen(wm_window_t* window);
void ui_refresh(wm_window_t* window);
void ui_close(wm_window_t* window);
void ui_move_window(wm_window_t* window);













































/* ******************USER PROC*********************** */
