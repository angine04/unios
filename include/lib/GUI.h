#pragma once

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MIN_WIDGET_WIDTH 10
#define MIN_WIDGET_HEIGHT 10
// 事件类型枚举
typedef enum {
    EVENT_MOUSE_CLICK,     // 鼠标点击
    EVENT_MOUSE_PRESS,     // 鼠标按下
    EVENT_MOUSE_RELEASE,   // 鼠标释放
    EVENT_MOUSE_MOVE,      // 鼠标移动
    EVENT_MOUSE_ENTER,     // 鼠标进入
    EVENT_MOUSE_LEAVE,     // 鼠标离开
    EVENT_KEY_PRESS,       // 键盘按下
    EVENT_KEY_RELEASE,     // 键盘释放
    EVENT_FOCUS_IN,        // 获得焦点
    EVENT_FOCUS_OUT,       // 失去焦点
    EVENT_VALUE_CHANGED,   // 值改变
    EVENT_RESIZE,          // 大小改变
    EVENT_CUSTOM           // 自定义事件
} event_type_t;

// 事件数据结构
typedef struct {
    event_type_t type;     // 事件类型
    void* sender;          // 事件发送者
    void* target;          // 事件目标

    union {
        // 鼠标事件数据
        struct {
            int x;         // 鼠标 X 坐标
            int y;         // 鼠标 Y 坐标
            int button;    // 按钮编号
            int clicks;    // 点击次数
        } mouse;

        // 键盘事件数据
        struct {
            int keycode;   // 键码
            int modifiers; // 修饰键状态
            char text[32]; // 输入文本
        } key;

        // 值改变事件数据
        struct {
            void* old_value;
            void* new_value;
        } value;

        // 自定义事件数据
        void* custom_data;
    } data;
} event_t;

// 事件队列管理
typedef struct event_queue_t event_queue_t;

void GUI_init(int pid);
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
int ui_create_widget(int x, int y, int width, int height, wm_window_t* window);
/*!
 * @brief Create a button to the window manager.
 * @param x The x coordinate of the button
 * @param y The y coordinate of the button
 * @param width The width of the button
 * @param height The height of the button
 * @param text The text of the button
 * @param callback The callback function of the button
 * @param window The window to add the button
 * @return button id
 */
int ui_create_button(int x, int y, int width, int height, char *text, void (*callback)(void*), wm_window_t* window);
/*!
 * @brief Create a label to the window manager.
 * @param x The x coordinate of the label
 * @param y The y coordinate of the label
 * @param width The width of the label
 * @param height The height of the label
 * @param text The text of the label
 * @param window The window to add the label
 * @return label id
 */
int ui_create_label(int x, int y, int width, int height, char *text, wm_window_t* window);
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
int ui_create_image(int x, int y, int width, int height, char *image_path, wm_window_t* window);
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
