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

void GUI_init();
/*************基础控件****************** */
int ui_create_widget(int x, int y, int width, int height, wm_window_t* window);
int ui_create_button(int x, int y, int width, int height, char *text, void (*callback)(void*), wm_window_t* window);
int ui_create_label(int x, int y, int width, int height, char *text, wm_window_t* window);
int ui_create_image(int x, int y, int width, int height, char *image_path, wm_window_t* window);
int ui_create_textbox(int x, int y, int width, int height, char *text, wm_window_t* window);

/*************事件处理****************** */
// 事件处理回调函数类型
typedef void (*event_handler_t)(event_t* event, void* user_data);

// 事件处理器注册函数
int ui_add_event_handler(int widget_id, event_type_t event_type, event_handler_t handler, void* user_data);
int ui_remove_event_handler(int widget_id, event_type_t event_type, event_handler_t handler);

// 常用事件快捷处理函数
int ui_on_click(int widget_id, void (*callback)(void* sender));
int ui_on_value_changed(int widget_id, void (*callback)(void* sender, void* new_value));
int ui_on_key(int widget_id, void (*callback)(void* sender, int keycode));

// 事件队列操作
event_queue_t* event_queue_create();
void event_queue_destroy(event_queue_t* queue);
int event_queue_push(event_queue_t* queue, event_t* event);
event_t* event_queue_pop(event_queue_t* queue);

// 事件分发
void event_dispatch(event_t* event);



void ui_show(wm_window_t* window);
void ui_hide(wm_window_t* window);
void ui_full_screen(wm_window_t* window);
void ui_refresh(wm_window_t* window);
void ui_close(wm_window_t* window);
