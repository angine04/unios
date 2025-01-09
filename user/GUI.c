#include "GUI.h"

void GUI_init(){

}

int ui_create_widget(int x, int y, int width, int height, wm_window_t* window){

}

int ui_create_button(int x, int y, int width, int height, char *text, void (*callback)(void*), wm_window_t* window){

}

int ui_create_label(int x, int y, int width, int height, char *text, wm_window_t* window){

}

int ui_create_image(int x, int y, int width, int height, char *image_path, wm_window_t* window){

}
int main(){
    GUI_init();
    return 0;
}
