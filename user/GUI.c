#include "GUI.h"
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


wm_ctx_t* window_ctx;
layer_ctx_t* layer_ctx;

void GUI_init(){
    window_ctx = get_window_ctx(ctx);
    layer_ctx = get_layer_ctx(ctx);
}

int ui_create_widget(int x, int y, int width, int height, wm_window_t* window){
    if(x > SCREEN_WIDTH - MIN_WIDGET_WIDTH || y > SCREEN_HEIGHT - MIN_WIDGET_HEIGHT || width < MIN_WIDGET_WIDTH || height < MIN_WIDGET_HEIGHT){
        return -1;
    }else{
        wm_window_t* w = (wm_window_t*)malloc(sizeof(wm_window_t));
        window = w;

        w->x = x;
        w->y = y;
        w->width = width;
        w->height = height;

        /*****background******** */
        w->contents[0].x = 0;
        w->contents[0].y = 0;
        w->contents[0].width = width;
        w->contents[0].height = height;
        w->contents[0].z_index = 10;
        w->contents[0].layer_index = create_layer(layer_ctx, w->contents[0].x, w->contents[0].y, w->contents[0].width, w->contents[0].height, 10);
        fill(layer_ctx, w->contents[0].layer_index, COLOR_SILVER);
        rect(layer_ctx, w->contents[0].layer_index, 0, 40, width, height - 40, COLOR_LIGHTGREY);
        w->contents[0].bandFunction = NULL;
        w->contents[0].belongWindow = window;
        w->contents[0].callbackEnable = false;
        w->contents[0].dynamicSize = true;
        /*****title******** */

        /*****button******** */
        w->contents[1].x = width - 30;
        w->contents[1].y = height - 20;
        w->contents[1].width = 20;
        w->contents[1].height = 20;
        w->contents[1].z_index = 20;
        w->contents[1].layer_index = create_layer(layer_ctx, w->contents[1].x, w->contents[1].y, w->contents[1].width, w->contents[1].height, 20);
        circle(layer_ctx, w->contents[1].layer_index, 10, 10, 10, COLOR_RED);
        w->contents[1].bandFunction = ui_close;
        w->contents[1].belongWindow = window;
        w->contents[1].callbackEnable = true;
        w->contents[1].dynamicSize = false;

        w->contents[2].x = width - 60;
        w->contents[2].y = height - 20;
        w->contents[2].width = 20;
        w->contents[2].height = 20;
        w->contents[2].z_index = 21;
        w->contents[2].layer_index = create_layer(layer_ctx, w->contents[1].x, w->contents[1].y, w->contents[1].width, w->contents[1].height, 21);
        circle(layer_ctx, w->contents[2].layer_index, 10, 10, 10, COLOR_YELLOW);
        w->contents[2].bandFunction = ui_hide;
        w->contents[2].belongWindow = window;
        w->contents[2].callbackEnable = true;
        w->contents[2].dynamicSize = false;

        w->contents[3].x = width - 90;
        w->contents[3].y = height - 20;
        w->contents[3].width = 20;
        w->contents[3].height = 20;
        w->contents[3].z_index = 22;
        w->contents[3].layer_index = create_layer(layer_ctx, w->contents[3].x, w->contents[3].y, w->contents[3].width, w->contents[3].height, 22);
        circle(layer_ctx, w->contents[3].layer_index, 10, 10, 10, COLOR_GREEN);
        w->contents[3].bandFunction = ui_full_screen;
        w->contents[3].belongWindow = window;
        w->contents[3].callbackEnable = true;
        w->contents[3].dynamicSize = false;
        /*****label******** */

        /*****image******** */

        /*****textbox******** */

        return 0;
    }
}

int ui_create_button(int x, int y, int width, int height, char *text, void (*callback)(void*), wm_window_t* window){

}

int ui_create_label(int x, int y, int width, int height, char *text, wm_window_t* window){

}

int ui_create_image(int x, int y, int width, int height, char *image_path, wm_window_t* window){

}

void ui_show(wm_window_t* window){
    wm_add_window(window_ctx, window);
}
void ui_hide(wm_window_t* window){
    wm_remove_window(window_ctx, window->id);
}
void ui_full_screen(wm_window_t* window){
    wm_resizeWindows(window, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void ui_refresh(wm_window_t* window){

}
void ui_close(wm_window_t* window){
    wm_remove_window(window_ctx, window->id);
}

int main(){
    GUI_init();
    return 0;
}
