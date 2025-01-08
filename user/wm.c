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


static int window_id = 2;

static int isHitByCursor(int x, int y, int width, int height, int cursor_x, int cursor_y){
    if((cursor_x > x) && (cursor_x < x+width) && (cursor_y > y) && (cursor_y < y+height)){
        return 1;
    }
    return 0;
}


void test_layers(layer_ctx_t *layer_ctx, int pid) {
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
    use_icon_32(layer_ctx, icon_layer_index, 0);
    render(layer_ctx, pid);

    while (1) {
        int x = layer_ctx->layers[layer_index].pos_x;
        int y = layer_ctx->layers[layer_index].pos_y;
        // Check if window would go out of screen bounds
        if (x + 1 + layer_ctx->layers[layer_index].width > DISPLAY_WIDTH ||
            y + 1 + layer_ctx->layers[layer_index].height > DISPLAY_HEIGHT) {
            // Reset position to top left
            move(layer_ctx, layer_index, 0, 0);
            top(layer_ctx, layer_index);
            layer_ctx->layers[layer_index2].opacity -= 0.02f;
            if (layer_ctx->layers[layer_index2].opacity < 0) {
                layer_ctx->layers[layer_index2].opacity = 0;
            }
        } else {
            move(layer_ctx, layer_index, x + 1, y + 1);
        }
        render(layer_ctx, pid);
    }
}

void test_window(wm_ctx_t *ctx, int pid){
    wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
    w->x = 100;
    w->y = 100;
    w->width = 100;
    w->height = 100;
    w->contents[0].x = 100;
    w->contents[0].y = 100;
    w->contents[0].width = 100;
    w->contents[0].height = 100;
    w->contents[0].z_index = 10;
    w->contents[0].layer_index = create_layer(ctx->layer_ctx, w->contents[0].x, w->contents[0].y, w->contents[0].width, w->contents[0].height, 10);
    fill(ctx->layer_ctx, w->contents[0].layer_index, COLOR_SILVER);
    rect(ctx->layer_ctx, w->contents[0].layer_index, 5, 5, 19, 19, COLOR_LIGHTGREY);
    circle(ctx->layer_ctx, w->contents[0].layer_index, 50, 50, 20, COLOR_BLUE);
    circle(ctx->layer_ctx, w->contents[0].layer_index, 60, 40, 20, COLOR_RED);
    triangle(ctx->layer_ctx, w->contents[0].layer_index, 50, 50, 70, 100, 90, 90, COLOR_GREEN);
    w->contents[0].callbackEnable = false;
    w->contents[0].bandFunction = NULL;
    w->contents[0].belongWindow = w;

    w->contents[1].x = 50;
    w->contents[1].y = 50;
    w->contents[1].width = 32;
    w->contents[1].height = 32;
    w->contents[1].z_index = 40;
    w->contents[1].layer_index = create_layer(ctx->layer_ctx, w->contents[1].x, w->contents[1].y, w->contents[1].width, w->contents[1].height, 20);
    use_icon_32(ctx->layer_ctx, w->contents[1].layer_index, 0);
    w->contents[1].callbackEnable = false;
    w->contents[1].bandFunction = NULL;
    w->contents[1].belongWindow = w;
    w->layer_count = 2;
    wm_add_window(ctx, w);

    // Create second window
    wm_window_t* w2 = (wm_window_t*)malloc(sizeof(wm_window_t));
    w2->x = 400;
    w2->y = 300;
    w2->width = 300;
    w2->height = 250;
    w2->contents[0].x = 15;
    w2->contents[0].y = 15;
    w2->contents[0].width = 150;
    w2->contents[0].height = 150;
    w2->contents[0].z_index = 30;
    w2->contents[0].layer_index = create_layer(ctx->layer_ctx, w2->contents[0].x, w2->contents[0].y, w2->contents[0].width, w2->contents[0].height, 30);
    fill(ctx->layer_ctx, w2->contents[0].layer_index, COLOR_PURPLE);
    rect(ctx->layer_ctx, w2->contents[0].layer_index, 10, 10, 130, 130, COLOR_BLUE);
    circle(ctx->layer_ctx, w2->contents[0].layer_index, 75, 75, 50, COLOR_GREEN);
    w2->contents[0].callbackEnable = false;
    w2->contents[0].bandFunction = NULL;
    w2->contents[0].belongWindow = w2;

    w2->contents[1].x = 50;
    w2->contents[1].y = 50;
    w2->contents[1].width = 32;
    w2->contents[1].height = 32;
    w2->contents[1].z_index = 40;
    w2->contents[1].layer_index = create_layer(ctx->layer_ctx, w2->contents[1].x, w2->contents[1].y, w2->contents[1].width, w2->contents[1].height, 40);
    use_icon_32(ctx->layer_ctx, w2->contents[1].layer_index, 0);
    w2->contents[1].callbackEnable = false;
    w2->contents[1].bandFunction = NULL;
    w2->contents[1].belongWindow = w2;
    w2->layer_count = 2;
    wm_add_window(ctx, w2);

    // Create third window that overlaps with first window
    wm_window_t* w3 = (wm_window_t*)malloc(sizeof(wm_window_t));
    w3->x = 250;
    w3->y = 150;
    w3->width = 200;
    w3->height = 200;
    w3->contents[0].x = 10;
    w3->contents[0].y = 10;
    w3->contents[0].width = 180;
    w3->contents[0].height = 180;
    w3->contents[0].z_index = 50;
    w3->contents[0].layer_index = create_layer(ctx->layer_ctx, w3->contents[0].x, w3->contents[0].y, w3->contents[0].width, w3->contents[0].height, 50);
    fill(ctx->layer_ctx, w3->contents[0].layer_index, COLOR_YELLOW);
    rect(ctx->layer_ctx, w3->contents[0].layer_index, 5, 5, 170, 170, COLOR_RED);
    w3->contents[0].callbackEnable = false;
    w3->contents[0].bandFunction = NULL;
    w3->contents[0].belongWindow = w3;
    w3->layer_count = 1;
    wm_add_window(ctx, w3);

    // Create fourth window that overlaps with second window
    wm_window_t* w4 = (wm_window_t*)malloc(sizeof(wm_window_t));
    w4->x = 450;
    w4->y = 350;
    w4->width = 200;
    w4->height = 200;
    w4->contents[0].x = 10;
    w4->contents[0].y = 10;
    w4->contents[0].width = 180;
    w4->contents[0].height = 180;
    w4->contents[0].z_index = 60;
    w4->contents[0].layer_index = create_layer(ctx->layer_ctx, w4->contents[0].x, w4->contents[0].y, w4->contents[0].width, w4->contents[0].height, 60);
    fill(ctx->layer_ctx, w4->contents[0].layer_index, COLOR_GREEN);
    rect(ctx->layer_ctx, w4->contents[0].layer_index, 5, 5, 170, 170, COLOR_BLUE);
    w4->contents[0].callbackEnable = false;
    w4->contents[0].bandFunction = NULL;
    w4->contents[0].belongWindow = w4;
    w4->layer_count = 1;
    wm_add_window(ctx, w4);

    // Test window switching with simulated clicks
    while(1){
        render(ctx->layer_ctx, pid);
        sleep(200);

        // Click on first window
        wm_updateTopWindow(ctx, 120, 120);
        render(ctx->layer_ctx, pid);
        sleep(200);

        // Click on third window
        wm_updateTopWindow(ctx, 300, 200);
        render(ctx->layer_ctx, pid);
        sleep(200);

        // Click on second window
        wm_updateTopWindow(ctx, 420, 320);
        render(ctx->layer_ctx, pid);
        sleep(200);

        // Click on fourth window
        wm_updateTopWindow(ctx, 500, 400);
        render(ctx->layer_ctx, pid);
    }


}

int main() {
    int pid = get_pid();

    wm_ctx_t* wm_ctx = malloc(sizeof(wm_ctx_t));
    wm_init(wm_ctx);

    layer_ctx_t* layer_ctx = wm_ctx->layer_ctx;

    init_desktop(wm_ctx);
    init_cursor(wm_ctx);


    // test_layers(layer_ctx, pid);
    test_window(wm_ctx, pid);
    while (1) {
        render(layer_ctx, pid);
        sleep(1000);
    }

    unreachable();
    return 0;
}

void wm_init(wm_ctx_t *ctx) {
    ctx->window_count = 2;
    ctx->layer_ctx = malloc(sizeof(layer_ctx_t));

    ctx->topWindow = (wm_windowNode*)malloc(sizeof(wm_windowNode));
    ctx->bottomWindow = (wm_windowNode*)malloc(sizeof(wm_windowNode));
    ctx->topWindow->pre_wmN = NULL;
    ctx->topWindow->next_wmN = ctx->bottomWindow;
    ctx->bottomWindow->pre_wmN = ctx->topWindow;
    ctx->bottomWindow->next_wmN = NULL;
    compositor_init(ctx->layer_ctx);
}

void init_desktop(wm_ctx_t *ctx) {
    wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
    w->w_z_index = DESKTOP_Z_INDEX;
    w->id = 0;
    ctx->bottomWindow->window = w;//bottom forever
}
void init_cursor(wm_ctx_t *ctx) {
    wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
    w->w_z_index = CURSOR_Z_INDEX;
    w->id = 1;
    ctx->bottomWindow->window = w;//bottom forever
}


int wm_add_window(wm_ctx_t *ctx, wm_window_t* window){
    //新增窗口为之前顶层用户窗口 w z index + 1
    if(ctx->window_count == 2){window->w_z_index = DESKTOP_Z_INDEX + 1;}
    else{
        window->w_z_index = ctx->topWindow->next_wmN->window->w_z_index + 1;
    }
    window->id = window_id++;
    //插入新窗口
    wm_windowNode* wmN = (wm_windowNode*)malloc(sizeof(wm_windowNode));
    wmN->window = window;
    wmN->pre_wmN = ctx->topWindow;
    wmN->next_wmN = ctx->topWindow->next_wmN;

    ctx->topWindow->next_wmN->pre_wmN = wmN;
    ctx->topWindow->next_wmN = wmN;

    ctx->window_count++;
    //排序组件
    for(int i  = 0; i < MAX_CONTENTS-1; i++){
        for(int j = 0; j < MAX_CONTENTS-1-i; j++){
            if(window->contents[j].z_index > window->contents[j+1].z_index){
                wm_content_t temp = window->contents[j];
                window->contents[j] = window->contents[j+1];
                window->contents[j+1] = temp;
            }
        }
    }
    //计算并重新设定组件图层layer z index, x, y
    for(int i = 0; i < MAX_CONTENTS; i++){
        if(window->contents[i].layer_index <= 0){
            continue;
        }else{
            ctx->layer_ctx->layers[window->contents[i].layer_index].z_index = window->w_z_index * MAX_CONTENTS + 1 + i;
            ctx->layer_ctx->layers[window->contents[i].layer_index].pos_x = window->contents[i].x + window->x;
            ctx->layer_ctx->layers[window->contents[i].layer_index].pos_y = window->contents[i].y + window->y;
        }

    }
    sort_layer(ctx->layer_ctx);
    return 0;
}
int wm_remove_window(wm_ctx_t *ctx, int window_id){

}
int wm_remove_top_window(wm_ctx_t *ctx){
    if(ctx->window_count == 2){
        return -1;
    }else{
        wm_windowNode* p = ctx->topWindow->next_wmN;
        ctx->topWindow->next_wmN = p->next_wmN;
        p->next_wmN->pre_wmN = ctx->topWindow;

        free(p->window);
        p->pre_wmN = NULL;
        p->next_wmN = NULL;
        free(p);
        ctx->window_count--;
    }
    return 0;
}
void wm_updateTopWindow(wm_ctx_t *ctx, int cursor_x, int cursor_y){
    //检索鼠标点击处触发了哪个非用户TOP WINDOW的窗口
    wm_windowNode* p = ctx->topWindow->next_wmN->next_wmN;
    int t = 0;
    while(p != ctx->bottomWindow){
        if(isHitByCursor(p->window->x, p->window->y, p->window->width, p->window->height, cursor_x, cursor_y)){
            t = 1;
            break;
        }
        p = p->next_wmN;
    }
    if(t == 0){//点击了桌面
        return;
    }
    //将触发的窗口提升到顶层，w z index为前顶层窗口的数值,重新计算并设定组件图层layer z index
    wm_windowNode* p1 = ctx->topWindow->next_wmN;
    wm_windowNode* p2 = p->pre_wmN;
    wm_windowNode* p3 = p->next_wmN;

    p2->next_wmN = p3;
    p3->pre_wmN = p2;

    p->pre_wmN = ctx->topWindow;
    p->next_wmN = p1;

    ctx->topWindow->next_wmN = p;
    p1->pre_wmN = p;
    p->window->w_z_index = p1->window->w_z_index;

    for(int i = 0; i < MAX_CONTENTS; i++){
        ctx->layer_ctx->layers[p->window->contents[i].layer_index].z_index = p->window->w_z_index * MAX_CONTENTS + 1 + i;
    }
    //将 前顶层窗口 至 被提升的窗口之前的窗口 这些窗口w z index数值-1,重新计算并设定组件图层layer z index
    wm_windowNode* pp = p1;
    while(pp != p3){
        pp->window->w_z_index -= 1;
        for(int i = 0; i < MAX_CONTENTS; i++){
            ctx->layer_ctx->layers[pp->window->contents[i].layer_index].z_index = pp->window->w_z_index * MAX_CONTENTS + 1 + i;
        }
        pp = pp->next_wmN;
    }
    sort_layer(ctx->layer_ctx);
}
void wm_resizeWindows(wm_window_t* window, int newWidth, int newHeight){

}
