/* ******************WM*********************** */
#include <stdio.h>
#include <stdbool.h>
#include <graphics.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <compositor.h>
#include <wm.h>
#include <malloc.h>
#include <lib/mouse.h>


#define USE_WALLPAPER 1

#define WALLPAPER_COLOR 0xB5D4DB

static int window_id = 2;

static int isHitByCursor(
    int x, int y, int width, int height, int cursor_x, int cursor_y) {
    if ((cursor_x > x) && (cursor_x < x + width) && (cursor_y > y)
        && (cursor_y < y + height)) {
        return 1;
    }
    return 0;
}

void test_cursor(layer_ctx_t* layer_ctx, int pid) {
    int layer_index = create_layer(layer_ctx, 100, 100, 20, 20, 65535);
    assert(layer_index != -1);
    fill(layer_ctx, layer_index, COLOR_RED);
    render(layer_ctx, pid);
    while (1) {
        mouse_t mouse  = get_mouse_status();
        int     x      = mouse.x;
        int     y      = mouse.y;
        int     button = mouse.buttons;
        move(layer_ctx, layer_index, x, y);
        if (button == 1) {
            fill(layer_ctx, layer_index, COLOR_GREEN);
        } else if (button == 2) {
            fill(layer_ctx, layer_index, COLOR_BLUE);
        }
        render(layer_ctx, pid);
    }
}

void test_layers(layer_ctx_t* layer_ctx, int pid) {
    int layer_index = create_layer(layer_ctx, 100, 100, 200, 200, 1);
    assert(layer_index != -1);
    fill(layer_ctx, layer_index, COLOR_SILVER);
    render(layer_ctx, pid);
    rect(layer_ctx, layer_index, 5, 5, 190, 190, COLOR_LIGHTGREY);
    render(layer_ctx, pid);
    circle(layer_ctx, layer_index, 100, 100, 30, COLOR_BLUE);
    render(layer_ctx, pid);
    circle(layer_ctx, layer_index, 100, 100, 10, COLOR_RED);
    render(layer_ctx, pid);
    triangle(layer_ctx, layer_index, 50, 50, 70, 130, 90, 110, COLOR_GREEN);
    render(layer_ctx, pid);
    int layer_index2 = create_layer(layer_ctx, 300, 300, 100, 100, 2);
    assert(layer_index2 != -1);
    fill(layer_ctx, layer_index2, COLOR_PURPLE);
    rect(layer_ctx, layer_index2, 5, 5, 90, 90, COLOR_TRANSPARENT);
    render(layer_ctx, pid);
    int icon_layer_index = create_layer(layer_ctx, 300, 300, 32, 32, 3);
    assert(icon_layer_index != -1);
    use_image(layer_ctx, icon_layer_index, 0, 1);
    render(layer_ctx, pid);

    while (1) {
        int x = layer_ctx->layers[layer_index].pos_x;
        int y = layer_ctx->layers[layer_index].pos_y;
        // Check if window would go out of screen bounds
        if (x + 1 + layer_ctx->layers[layer_index].width > DISPLAY_WIDTH
            || y + 1 + layer_ctx->layers[layer_index].height > DISPLAY_HEIGHT) {
            // Reset position to top left
            move(layer_ctx, layer_index, 0, 0);
            top(layer_ctx, layer_index);
        } else {
            move(layer_ctx, layer_index, x + 1, y + 1);
        }
        render(layer_ctx, pid);
    }
}

void dummy_window(wm_ctx_t* ctx, int pid) {
    // does not work without this
    // do not ask why, neither do I know
    wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
    for (int i = 0; i < MAX_CONTENTS; i++) {
        w->contents[i].x           = 0;
        w->contents[i].y           = 0;
        w->contents[i].width       = 0;
        w->contents[i].height      = 0;
        w->contents[i].z_index     = 0;
        w->contents[i].layer_index = -1;
    }
    w->x                       = 0;
    w->y                       = 0;
    w->width                   = 1;
    w->height                  = 1;
    w->contents[0].x           = 0;
    w->contents[0].y           = 0;
    w->contents[0].width       = 1;
    w->contents[0].height      = 1;
    w->contents[0].z_index     = 10;
    w->contents[0].layer_index = create_layer(
        ctx->layer_ctx,
        w->contents[0].x,
        w->contents[0].y,
        w->contents[0].width,
        w->contents[0].height,
        10);
    w->contents[0].callbackEnable = false;
    w->contents[0].bandFunction   = NULL;
    w->contents[0].belongWindow   = w;
    hide(ctx->layer_ctx, w->contents[0].layer_index);
    wm_add_window(ctx, w);
}

void test_window(wm_ctx_t* ctx, int pid) {
    wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
    for (int i = 0; i < MAX_CONTENTS; i++) {
        w->contents[i].x           = 0;
        w->contents[i].y           = 0;
        w->contents[i].width       = 0;
        w->contents[i].height      = 0;
        w->contents[i].z_index     = 0;
        w->contents[i].layer_index = -1;
    }
    w->x                       = 0;
    w->y                       = 0;
    w->width                   = 1;
    w->height                  = 1;
    w->contents[0].x           = 0;
    w->contents[0].y           = 0;
    w->contents[0].width       = 1;
    w->contents[0].height      = 1;
    w->contents[0].z_index     = 10;
    w->contents[0].layer_index = create_layer(
        ctx->layer_ctx,
        w->contents[0].x,
        w->contents[0].y,
        w->contents[0].width,
        w->contents[0].height,
        10);
    w->contents[0].callbackEnable = false;
    w->contents[0].bandFunction   = NULL;
    w->contents[0].belongWindow   = w;
    hide(ctx->layer_ctx, w->contents[0].layer_index);
    wm_add_window(ctx, w);

    // Create second window
    wm_window_t* w2 = (wm_window_t*)malloc(sizeof(wm_window_t));
    for (int i = 0; i < MAX_CONTENTS; i++) {
        w2->contents[i].x           = 0;
        w2->contents[i].y           = 0;
        w2->contents[i].width       = 0;
        w2->contents[i].height      = 0;
        w2->contents[i].z_index     = 0;
        w2->contents[i].layer_index = -1;
    }
    w2->x                       = 400;
    w2->y                       = 300;
    w2->width                   = 300;
    w2->height                  = 250;
    w2->contents[0].x           = 0;
    w2->contents[0].y           = 0;
    w2->contents[0].width       = 300;
    w2->contents[0].height      = 250;
    w2->contents[0].z_index     = 30;
    w2->contents[0].layer_index = create_layer(
        ctx->layer_ctx,
        w2->contents[0].x,
        w2->contents[0].y,
        w2->contents[0].width,
        w2->contents[0].height,
        30);
    fill(ctx->layer_ctx, w2->contents[0].layer_index, COLOR_PURPLE);
    rect(
        ctx->layer_ctx,
        w2->contents[0].layer_index,
        10,
        10,
        130,
        130,
        COLOR_BLUE);
    circle(
        ctx->layer_ctx, w2->contents[0].layer_index, 75, 75, 50, COLOR_GREEN);
    w2->contents[0].callbackEnable = false;
    w2->contents[0].bandFunction   = NULL;
    w2->contents[0].belongWindow   = w2;

    w2->contents[1].x           = 50;
    w2->contents[1].y           = 50;
    w2->contents[1].width       = 64;
    w2->contents[1].height      = 64;
    w2->contents[1].z_index     = 40;
    w2->contents[1].layer_index = create_layer(
        ctx->layer_ctx,
        w2->contents[1].x,
        w2->contents[1].y,
        w2->contents[1].width,
        w2->contents[1].height,
        40);
    use_image(
        ctx->layer_ctx, w2->contents[1].layer_index, RESOURCE_ICON_UNIOS, 1);
    w2->contents[1].callbackEnable = false;
    w2->contents[1].bandFunction   = NULL;
    w2->contents[1].belongWindow   = w2;
    w2->layer_count                = 2;
    wm_add_window(ctx, w2);

    // Create third window that overlaps with first window
    wm_window_t* w3 = (wm_window_t*)malloc(sizeof(wm_window_t));
    for (int i = 0; i < MAX_CONTENTS; i++) {
        w3->contents[i].x           = 0;
        w3->contents[i].y           = 0;
        w3->contents[i].width       = 0;
        w3->contents[i].height      = 0;
        w3->contents[i].z_index     = 0;
        w3->contents[i].layer_index = -1;
    }
    w3->x                       = 250;
    w3->y                       = 250;
    w3->width                   = 200;
    w3->height                  = 200;
    w3->contents[0].x           = 0;
    w3->contents[0].y           = 0;
    w3->contents[0].width       = 200;
    w3->contents[0].height      = 200;
    w3->contents[0].z_index     = 50;
    w3->contents[0].layer_index = create_layer(
        ctx->layer_ctx,
        w3->contents[0].x,
        w3->contents[0].y,
        w3->contents[0].width,
        w3->contents[0].height,
        50);
    fill(ctx->layer_ctx, w3->contents[0].layer_index, COLOR_YELLOW);
    rect(
        ctx->layer_ctx, w3->contents[0].layer_index, 5, 5, 170, 170, COLOR_RED);
    w3->contents[0].callbackEnable = false;
    w3->contents[0].bandFunction   = NULL;
    w3->contents[0].belongWindow   = w3;
    w3->layer_count                = 1;
    wm_add_window(ctx, w3);

    GUI_init(ctx);
    wm_window_t* w4 = NULL;
    w4              = ui_create_widget(100, 100, 400, 300);
    ui_show(w4);
}

void test_gui(wm_ctx_t* ctx, int pid) {
    GUI_init(ctx);
    wm_window_t* w = NULL;
    w              = ui_create_widget(100, 100, 400, 300);
    ui_show(w);
}

int main() {
    int pid = get_pid();

    wm_ctx_t* wm_ctx = malloc(sizeof(wm_ctx_t));
    wm_init(wm_ctx);
    render(wm_ctx->layer_ctx, pid);

    layer_ctx_t* layer_ctx = wm_ctx->layer_ctx;

    init_desktop(wm_ctx);
    init_cursor(wm_ctx);

    dummy_window(wm_ctx, pid);
    // test_layers(layer_ctx, pid);
    // test_window(wm_ctx, pid);
    // test_gui(wm_ctx, pid);
    // test_cursor(layer_ctx, pid);

    GUI_init(wm_ctx);
    wm_window_t* w1 = NULL;
    w1              = ui_create_widget(400, 100, 400, 300);
    ui_show(w1);
    GUI_init(wm_ctx);
    wm_window_t* w2 = NULL;
    w2              = ui_create_widget(200, 200, 400, 500);
    ui_show(w2);

    while (1) {
        mouse_t mouse  = get_mouse_status();
        int     x      = mouse.x;
        int     y      = mouse.y;
        int     button = mouse.buttons;
        int     cursor_layer_index =
            wm_ctx->topWindow->window->contents[0].layer_index;
        move(layer_ctx, cursor_layer_index, x, y);
        while (!mouse_event_empty()) {
            mouse_event_t event = pop_mouse_event();
            if (event.event == MOUSE_LEFT_DOWN) {
                int mouse_hit = -1;
                mouse_hit = wm_updateTopWindow(wm_ctx, mouse.x, mouse.y);
                if (mouse_hit == HIT_TOP_WINDOW) {
                    for(int i = 0; i < MAX_CONTENTS; i++) {
                        if(wm_ctx->topWindow->next_wmN->window->contents[i].layer_index != -1 && wm_ctx->topWindow->next_wmN->window->contents[i].callbackEnable == true){
                            if(isHitByCursor(wm_ctx->topWindow->next_wmN->window->contents[i].x + wm_ctx->topWindow->next_wmN->window->x, wm_ctx->topWindow->next_wmN->window->contents[i].y + wm_ctx->topWindow->next_wmN->window->y, wm_ctx->topWindow->next_wmN->window->contents[i].width, wm_ctx->topWindow->next_wmN->window->contents[i].height,x, y) == 1){
                                wm_ctx->topWindow->next_wmN->window->contents[i].bandFunction(wm_ctx->topWindow->next_wmN->window);
                            }
                        }
                    }
                } else if (mouse_hit == HIT_DESKTOP) {

                }
            }
        }
        render(layer_ctx, pid);
        // sleep(1000);
    }

    unreachable();
    return 0;
}

void wm_init(wm_ctx_t* ctx) {
    ctx->window_count = 2;
    ctx->layer_ctx    = malloc(sizeof(layer_ctx_t));

    ctx->topWindow              = (wm_windowNode*)malloc(sizeof(wm_windowNode));
    ctx->bottomWindow           = (wm_windowNode*)malloc(sizeof(wm_windowNode));
    ctx->topWindow->pre_wmN     = NULL;
    ctx->topWindow->next_wmN    = ctx->bottomWindow;
    ctx->bottomWindow->pre_wmN  = ctx->topWindow;
    ctx->bottomWindow->next_wmN = NULL;
    compositor_init(ctx->layer_ctx);
}

void init_desktop(wm_ctx_t* ctx) {
    wm_window_t* w             = (wm_window_t*)malloc(sizeof(wm_window_t));
    w->w_z_index               = DESKTOP_Z_INDEX;
    w->id                      = 0;
    w->x                       = 0;
    w->y                       = 0;
    w->contents[0].x           = 0;
    w->contents[0].y           = 0;
    w->contents[0].width       = DISPLAY_WIDTH;
    w->contents[0].height      = DISPLAY_HEIGHT;
    w->contents[0].z_index     = 0;
    w->contents[1].x           = 64;
    w->contents[1].y           = 32;
    w->contents[1].width       = DISPLAY_WIDTH;
    w->contents[1].height      = DISPLAY_HEIGHT;
    w->contents[1].z_index     = 0;
    w->contents[2].x           = 64;
    w->contents[2].y           = 128;
    w->contents[2].width       = DISPLAY_WIDTH;
    w->contents[2].height      = DISPLAY_HEIGHT;
    w->contents[2].z_index     = 0;
    w->contents[0].layer_index = create_layer(
        ctx->layer_ctx,
        w->contents[0].x,
        w->contents[0].y,
        w->contents[0].width,
        w->contents[0].height,
        0);

    if (USE_WALLPAPER) {
        use_image(
            ctx->layer_ctx,
            w->contents[0].layer_index,
            RESOURCE_IMAGE_WALLPAPER,
            4);
    } else {
        fill(ctx->layer_ctx, w->contents[0].layer_index, WALLPAPER_COLOR);
    }
    w->contents[1].layer_index = create_layer(
        ctx->layer_ctx, w->contents[1].x, w->contents[1].y, 64, 64, 1);
    clear(ctx->layer_ctx, w->contents[1].layer_index);
    use_image(
        ctx->layer_ctx, w->contents[1].layer_index, RESOURCE_ICON_UNIOS, 1);
    w->contents[2].layer_index = create_layer(
        ctx->layer_ctx, w->contents[2].x, w->contents[2].y, 64, 64, 2);
    clear(ctx->layer_ctx, w->contents[2].layer_index);
    use_image(
        ctx->layer_ctx, w->contents[2].layer_index, RESOURCE_ICON_CALC, 1);
    w->layer_count = 3;
}

void init_cursor(wm_ctx_t* ctx) {
    wm_window_t* w             = (wm_window_t*)malloc(sizeof(wm_window_t));
    w->w_z_index               = CURSOR_Z_INDEX;
    w->id                      = 1;
    w->x                       = 1;
    w->y                       = 1;
    w->contents[0].x           = 1;
    w->contents[0].y           = 1;
    w->contents[0].width       = 15;
    w->contents[0].height      = 24;
    w->contents[0].layer_index = create_layer(
        ctx->layer_ctx,
        w->contents[0].x,
        w->contents[0].y,
        w->contents[0].width,
        w->contents[0].height,
        65535);
    ctx->topWindow->window = w; // top forever
    int cursor_layer_index = w->contents[0].layer_index;
    use_image(ctx->layer_ctx, cursor_layer_index, RESOURCE_ICON_CURSOR, 1);
}

int wm_add_window(wm_ctx_t* ctx, wm_window_t* window) {
    // 新增窗口为之前顶层用户窗口 w z index + 1
    if (ctx->window_count == 2) {
        window->w_z_index = DESKTOP_Z_INDEX + 1;
    } else {
        window->w_z_index = ctx->topWindow->next_wmN->window->w_z_index + 1;
    }
    window->id = window_id++;
    // 插入新窗口
    wm_windowNode* wmN = (wm_windowNode*)malloc(sizeof(wm_windowNode));
    wmN->window        = window;
    wmN->pre_wmN       = ctx->topWindow;
    wmN->next_wmN      = ctx->topWindow->next_wmN;

    ctx->topWindow->next_wmN->pre_wmN = wmN;
    ctx->topWindow->next_wmN          = wmN;

    ctx->window_count++;
    // 排序组件
    for (int i = 0; i < MAX_CONTENTS - 1; i++) {
        for (int j = 0; j < MAX_CONTENTS - 1 - i; j++) {
            if (window->contents[j].z_index > window->contents[j + 1].z_index) {
                wm_content_t temp       = window->contents[j];
                window->contents[j]     = window->contents[j + 1];
                window->contents[j + 1] = temp;
            }
        }
    }
    // 计算并重新设定组件图层layer z index, x, y
    for (int i = 0; i < MAX_CONTENTS; i++) {
        if (window->contents[i].layer_index <= 0) {
            continue;
        } else {
            ctx->layer_ctx->layers[window->contents[i].layer_index].z_index =
                window->w_z_index * MAX_CONTENTS + 1 + i;
            ctx->layer_ctx->layers[window->contents[i].layer_index].pos_x =
                window->contents[i].x + window->x;
            ctx->layer_ctx->layers[window->contents[i].layer_index].pos_y =
                window->contents[i].y + window->y;
        }
    }
    sort_layer(ctx->layer_ctx);
    return 0;
}

int wm_remove_window(wm_ctx_t* ctx, int window_id) {}

int wm_remove_top_window(wm_ctx_t* ctx) {
    if (ctx->window_count == 2) {
        return -1;
    } else {
        wm_windowNode* p         = ctx->topWindow->next_wmN;
        ctx->topWindow->next_wmN = p->next_wmN;
        p->next_wmN->pre_wmN     = ctx->topWindow;

        for(int i = 0; i < MAX_CONTENTS; i++){
            if(p->window->contents[i].layer_index != -1){
                //TODO: 精确标记dirty area
                mark_dirty(ctx->layer_ctx, 0, 0, 1024, 768);
                release_layer(ctx->layer_ctx, p->window->contents[i].layer_index);
            }
        }
        free(p->window);
        p->pre_wmN  = NULL;
        p->next_wmN = NULL;
        free(p);
        ctx->window_count--;

    }
    return 0;
}

int wm_updateTopWindow(wm_ctx_t* ctx, int cursor_x, int cursor_y) {
    // 检索鼠标点击处触发了哪个非用户TOP WINDOW的窗口
    wm_windowNode* p = ctx->topWindow->next_wmN;
    if (isHitByCursor(
            p->window->x,
            p->window->y,
            p->window->width,
            p->window->height,
            cursor_x,
            cursor_y)) {
        return HIT_TOP_WINDOW; // 点击的是顶层窗口
    }
    p     = ctx->topWindow->next_wmN->next_wmN;
    int t = 0;
    while (p != ctx->bottomWindow) {
        if (isHitByCursor(
                p->window->x,
                p->window->y,
                p->window->width,
                p->window->height,
                cursor_x,
                cursor_y)) {
            t = 1;
            break;
        }
        p = p->next_wmN;
    }
    if (t == 0) { // 点击了桌面
        return HIT_DESKTOP;
    }
    // 将触发的窗口提升到顶层，w z
    // index为前顶层窗口的数值,重新计算并设定组件图层layer z index
    wm_windowNode* p1 = ctx->topWindow->next_wmN;
    wm_windowNode* p2 = p->pre_wmN;
    wm_windowNode* p3 = p->next_wmN;

    p2->next_wmN = p3;
    p3->pre_wmN  = p2;

    p->pre_wmN  = ctx->topWindow;
    p->next_wmN = p1;

    ctx->topWindow->next_wmN = p;
    p1->pre_wmN              = p;
    p->window->w_z_index     = p1->window->w_z_index;

    for (int i = 0; i < MAX_CONTENTS; i++) {
        if (p->window->contents[i].layer_index == -1) {
            continue;
        } else {
            ctx->layer_ctx->layers[p->window->contents[i].layer_index].z_index =
                p->window->w_z_index * MAX_CONTENTS + 1 + i;
        }
    }
    // 将 前顶层窗口 至 被提升的窗口之前的窗口 这些窗口w z
    // index数值-1,重新计算并设定组件图层layer z index
    wm_windowNode* pp = p1;
    while (pp != p3) {
        pp->window->w_z_index -= 1;
        for (int i = 0; i < MAX_CONTENTS; i++) {
            if (pp->window->contents[i].layer_index == -1) {
                continue;
            } else {
                ctx->layer_ctx->layers[pp->window->contents[i].layer_index]
                    .z_index = pp->window->w_z_index * MAX_CONTENTS + 1 + i;
            }
        }
        mark_dirty(ctx->layer_ctx,
            pp->window->x,
            pp->window->y,
            pp->window->x + pp->window->width - 1,
            pp->window->y + pp->window->height - 1);
        pp = pp->next_wmN;
    }
    sort_layer(ctx->layer_ctx);

    // mark affected area - all windows from p1 to p3 need to be redrawn
    // also mark the window being moved to top
    mark_dirty(ctx->layer_ctx,
        p->window->x,
        p->window->y,
        p->window->x + p->window->width - 1,
        p->window->y + p->window->height - 1);
    mark_dirty(ctx->layer_ctx, 0, 0, 1024, 768);
    return HIT_OTHER_WINDOW;

}

void wm_full_screen(wm_ctx_t* ctx) {
    if(ctx->topWindow->next_wmN->window->is_full_screen){
        ctx->topWindow->next_wmN->window->is_full_screen = false;
        ctx->topWindow->next_wmN->window->width = ctx->topWindow->next_wmN->window->old_width;
        ctx->topWindow->next_wmN->window->height = ctx->topWindow->next_wmN->window->old_height;
        int background_content_index;
        for(int i = 0; i < MAX_CONTENTS; i++){
            if(ctx->topWindow->next_wmN->window->contents[i].layer_index != -1){
                background_content_index = i;
                break;
            }
        }
        ctx->topWindow->next_wmN->window->contents[background_content_index].width = ctx->topWindow->next_wmN->window->old_width;
        ctx->topWindow->next_wmN->window->contents[background_content_index].height = ctx->topWindow->next_wmN->window->old_height;
        resize(ctx->layer_ctx, ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index, ctx->topWindow->next_wmN->window->old_width, ctx->topWindow->next_wmN->window->old_height);
        clear(ctx->layer_ctx, ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index);
        rounded_rect(
                ctx->layer_ctx,
                ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index,
                0,
                0,
                ctx->topWindow->next_wmN->window->old_width,
                ctx->topWindow->next_wmN->window->old_height,
                12,
                0x808080);
            rounded_rect(
                ctx->layer_ctx,
                ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index,
                1,
                1,
                ctx->topWindow->next_wmN->window->old_width - 2,
                ctx->topWindow->next_wmN->window->old_height - 2,
                11,
                0x383838);
        mark_dirty(ctx->layer_ctx, 0, 0, 1024, 768);
    }else{
        ctx->topWindow->next_wmN->window->is_full_screen = true;
        ctx->topWindow->next_wmN->window->x = 0;
        ctx->topWindow->next_wmN->window->y = 0;
        for(int i = 0; i < MAX_CONTENTS; i++){
            if(ctx->topWindow->next_wmN->window->contents[i].layer_index != -1){
                move(ctx->layer_ctx, ctx->topWindow->next_wmN->window->contents[i].layer_index, 0 + ctx->topWindow->next_wmN->window->contents[i].x, 0 + ctx->topWindow->next_wmN->window->contents[i].y);
            }
        }
        ctx->topWindow->next_wmN->window->width = SCREEN_WIDTH;
        ctx->topWindow->next_wmN->window->height = SCREEN_HEIGHT;
        int background_content_index;
        for(int i = 0; i < MAX_CONTENTS; i++){
            if(ctx->topWindow->next_wmN->window->contents[i].layer_index != -1){
                background_content_index = i;
                break;
            }
        }
        ctx->topWindow->next_wmN->window->contents[background_content_index].width = SCREEN_WIDTH;
        ctx->topWindow->next_wmN->window->contents[background_content_index].height = SCREEN_HEIGHT;
        resize(ctx->layer_ctx, ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index, SCREEN_WIDTH, SCREEN_HEIGHT);
        clear(ctx->layer_ctx, ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index);
        rounded_rect(
                ctx->layer_ctx,
                ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index,
                0,
                0,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                12,
                0x808080);
            rounded_rect(
                ctx->layer_ctx,
                ctx->topWindow->next_wmN->window->contents[background_content_index].layer_index,
                1,
                1,
                SCREEN_WIDTH - 2,
                SCREEN_HEIGHT - 2,
                11,
                0x383838);
        mark_dirty(ctx->layer_ctx, 0, 0, 1024, 768);
    }
}

void wm_move_top_window(wm_ctx_t* ctx) {

    mouse_t mouse  = get_mouse_status();
    int     x      = mouse.x;
    int     y      = mouse.y;
    mouse_event_t event = pop_mouse_event();

    while(event.event != MOUSE_LEFT_DOWN){
        mouse = get_mouse_status();
        x = mouse.x;
        y = mouse.y;
        event = pop_mouse_event();
        int     cursor_layer_index =
            ctx->topWindow->window->contents[0].layer_index;
        move(ctx->layer_ctx, cursor_layer_index, x, y);
        ctx->topWindow->next_wmN->window->x = x;
        ctx->topWindow->next_wmN->window->y = y;
        for(int i = 0; i < MAX_CONTENTS; i++){
            if(ctx->topWindow->next_wmN->window->contents[i].layer_index != -1){
                move(ctx->layer_ctx, ctx->topWindow->next_wmN->window->contents[i].layer_index, x + ctx->topWindow->next_wmN->window->contents[i].x, y + ctx->topWindow->next_wmN->window->contents[i].y);
            }
        }
        mark_dirty(ctx->layer_ctx, x, y, x + ctx->topWindow->next_wmN->window->width, y + ctx->topWindow->next_wmN->window->height);
        render(ctx->layer_ctx, get_pid());
    }
}


layer_ctx_t* get_layer_ctx(wm_ctx_t* ctx) {
    return ctx->layer_ctx;
}

/* ******************GUI********************** */
wm_ctx_t*    window_ctx = NULL;
layer_ctx_t* layer_ctx = NULL;

void GUI_init(wm_ctx_t* ctx) {
    window_ctx = ctx;
    layer_ctx  = get_layer_ctx(window_ctx);
}

wm_window_t* ui_create_widget(int x, int y, int width, int height) {
    if (x > SCREEN_WIDTH - MIN_WIDGET_WIDTH
        || y > SCREEN_HEIGHT - MIN_WIDGET_HEIGHT || width < MIN_WIDGET_WIDTH
        || height < MIN_WIDGET_HEIGHT) {
        return NULL;
    } else {
        wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
        for (int i = 0; i < MAX_CONTENTS; i++) {
            w->contents[i].x           = 0;
            w->contents[i].y           = 0;
            w->contents[i].width       = 0;
            w->contents[i].height      = 0;
            w->contents[i].z_index     = 0;
            w->contents[i].layer_index = -1;
        }

        w->x      = x;
        w->y      = y;
        w->width  = width;
        w->height = height;
        w->is_full_screen = false;
        w->old_width = width;
        w->old_height = height;

        /*****background******** */
        w->contents[0].x           = 0;
        w->contents[0].y           = 0;
        w->contents[0].width       = width;
        w->contents[0].height      = height;
        w->contents[0].z_index     = 10;
        w->contents[0].layer_index = create_layer(
            layer_ctx,
            w->contents[0].x,
            w->contents[0].y,
            w->contents[0].width,
            w->contents[0].height,
            10);
        clear(layer_ctx, w->contents[0].layer_index);
        rounded_rect(
            layer_ctx,
            w->contents[0].layer_index,
            0,
            0,
            width,
            height,
            12,
            0x808080);
        rounded_rect(
            layer_ctx,
            w->contents[0].layer_index,
            1,
            1,
            width - 2,
            height - 2,
            11,
            0x383838);
        w->contents[0].bandFunction   = NULL;
        w->contents[0].belongWindow   = w;
        w->contents[0].callbackEnable = false;
        w->contents[0].dynamicSize    = true;
        /*****title******** */

        /*****button******** */
        w->contents[1].x           = 54;
        w->contents[1].y           = 6;
        w->contents[1].width       = 12;
        w->contents[1].height      = 12;
        w->contents[1].z_index     = 20;
        w->contents[1].layer_index = create_layer(
            layer_ctx,
            w->contents[1].x,
            w->contents[1].y,
            w->contents[1].width,
            w->contents[1].height,
            20);
        clear(layer_ctx, w->contents[1].layer_index);
        circle(layer_ctx, w->contents[1].layer_index, 6, 6, 6, 0xED6A5E);
        w->contents[1].bandFunction   = ui_close;
        w->contents[1].belongWindow   = w;
        w->contents[1].callbackEnable = true;
        w->contents[1].dynamicSize    = false;

        w->contents[2].x           = 36;
        w->contents[2].y           = 6;
        w->contents[2].width       = 12;
        w->contents[2].height      = 12;
        w->contents[2].z_index     = 21;
        w->contents[2].layer_index = create_layer(
            layer_ctx,
            w->contents[2].x,
            w->contents[2].y,
            w->contents[2].width,
            w->contents[2].height,
            21);
        clear(layer_ctx, w->contents[2].layer_index);
        circle(layer_ctx, w->contents[2].layer_index, 6, 6, 6, 0xF4BF50);
        w->contents[2].bandFunction   = ui_move_window;
        w->contents[2].belongWindow   = w;
        w->contents[2].callbackEnable = true;
        w->contents[2].dynamicSize    = false;

        w->contents[3].x           = 18;
        w->contents[3].y           = 6;
        w->contents[3].width       = 12;
        w->contents[3].height      = 12;
        w->contents[3].z_index     = 22;
        w->contents[3].layer_index = create_layer(
            layer_ctx,
            w->contents[3].x,
            w->contents[3].y,
            w->contents[3].width,
            w->contents[3].height,
            22);
        clear(layer_ctx, w->contents[3].layer_index);
        circle(layer_ctx, w->contents[3].layer_index, 6, 6, 6, 0x61C555);
        w->contents[3].bandFunction   = ui_full_screen;
        w->contents[3].belongWindow   = w;
        w->contents[3].callbackEnable = true;
        w->contents[3].dynamicSize    = false;
        /*****label******** */

        /*****image******** */

        /*****textbox******** */

        return w;
    }
}

int ui_create_button(
    int   x,
    int   y,
    int   width,
    int   height,
    int   z_index,
    char* text,
    void (*callback)(wm_window_t*),
    wm_window_t* window) { // TODO: 检测边界情况
    int free_index = -1;
    for (int i = 0; i < MAX_CONTENTS; i++) {
        if (window->contents[i].layer_index == -1) {
            free_index = i;
            break;
        }
    }
    if (free_index == -1) { return -1; }
    window->contents[free_index].x       = x;
    window->contents[free_index].y       = y;
    window->contents[free_index].width   = width;
    window->contents[free_index].height  = height;
    window->contents[free_index].z_index = z_index;
    window->contents[free_index].layer_index =
        create_layer(layer_ctx, x, y, width, height, z_index);
    fill(layer_ctx, window->contents[free_index].layer_index, COLOR_BLACK);
    rect(
        layer_ctx,
        window->contents[free_index].layer_index,
        2,
        2,
        width - 4,
        height - 4,
        COLOR_GREY);
    window->contents[free_index].bandFunction   = callback;
    window->contents[free_index].belongWindow   = window;
    window->contents[free_index].callbackEnable = true;
    window->contents[free_index].dynamicSize    = true;
    return 0;
}

int ui_create_label(
    int          x,
    int          y,
    int          width,
    int          height,
    int          z_index,
    char*        text,
    wm_window_t* window) {}

int ui_create_image(
    int          x,
    int          y,
    int          width,
    int          height,
    int          z_index,
    char*        image_path,
    wm_window_t* window) {
    int free_index = -1;
    for (int i = 0; i < MAX_CONTENTS; i++) {
        if (window->contents[i].layer_index == -1) {
            free_index = i;
            break;
        }
    }
    if (free_index == -1) { return -1; }
    window->contents[free_index].x       = x;
    window->contents[free_index].y       = y;
    window->contents[free_index].width   = width;
    window->contents[free_index].height  = height;
    window->contents[free_index].z_index = z_index;
    window->contents[free_index].layer_index =
        create_layer(layer_ctx, x, y, width, height, z_index);
    fill(layer_ctx, window->contents[free_index].layer_index, COLOR_BLACK);
    // TODO: 加载图片
    window->contents[free_index].bandFunction   = NULL;
    window->contents[free_index].belongWindow   = window;
    window->contents[free_index].callbackEnable = false;
    window->contents[free_index].dynamicSize    = true;
    return 0;
}

void ui_show(wm_window_t* window) {
    wm_add_window(window_ctx, window);
}

void ui_hide(wm_window_t* window) {
    wm_remove_window(window_ctx, window->id);
}

void ui_full_screen(wm_window_t* window) {
    wm_full_screen(window_ctx);
}

void ui_refresh(wm_window_t* window) {}

void ui_close(wm_window_t* window) {
    wm_remove_top_window(window_ctx);
}

void ui_move_window(wm_window_t* window) {
    wm_move_top_window(window_ctx);
}
