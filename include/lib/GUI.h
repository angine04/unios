#pragma once
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

void GUI_init();
int ui_create_widget(int x, int y, int width, int height, wm_window_t* window);
int ui_create_button(int x, int y, int width, int height, char *text, void (*callback)(void*), wm_window_t* window);
int ui_create_label(int x, int y, int width, int height, char *text, wm_window_t* window);
int ui_create_image(int x, int y, int width, int height, char *image_path, wm_window_t* window);
int ui_create_textbox(int x, int y, int width, int height, char *text, wm_window_t* window);
