#include <graphics.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <compositor.h>
#include <malloc.h>
#include <math.h>

#define USE_ASSETS_DATA

#include <assets_data.h>

static pixel_t screen_vram[DISPLAY_WIDTH * DISPLAY_HEIGHT] = {0};

// for rounded rect acceleration
pixel_t corner_pattern_left_top[11][11] = {
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xee383838,
     0x95383838, 0x4c383838,
     0x1e383838, 0x06383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xf7383838,
     0x86383838, 0x10383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xdf383838, 0x2c383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xff000000, 0xe3383838,
     0x1c383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xf7383838, 0x35383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0x87383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xed383838,
     0x0f383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x94383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x4d383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x1d383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x06383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
};

pixel_t corner_pattern_right_top[11][11] = {
    {0x0c383838,
     0x28383838, 0x4c383838,
     0x95383838, 0xee383838,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x10383838,
     0x85383838, 0xf6383838,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x2c383838,
     0xdf383838, 0xff000000,
     0xff000000, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x1c383838, 0xe3383838,
     0xff000000, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x35383838,
     0xf6383838, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x86383838, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x0f383838, 0xed383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x94383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x4c383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x1d383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x06383838},
};

pixel_t corner_pattern_left_bottom[11][11] = {
    {0x0d383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x26383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x4d383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0x94383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xed383838,
     0x0f383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0x86383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xf6383838, 0x2d383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xff000000, 0xdb383838,
     0x1a383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xde383838, 0x2c383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xf6383838,
     0x85383838, 0x10383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xee383838,
     0x94383838, 0x4c383838,
     0x1e383838, 0x05383838},
};

pixel_t corner_pattern_right_bottom[11][11] = {
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x06383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x1d383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x4c383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x94383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x0f383838, 0xed383838},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x86383838, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x2c383838,
     0xf6383838, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x1b383838, 0xdb383838,
     0xff000000, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x2c383838,
     0xdf383838, 0xff000000,
     0xff000000, 0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x10383838,
     0x85383838, 0xf6383838,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000},
    {0x05383838,
     0x1e383838, 0x4c383838,
     0x94383838, 0xee383838,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000},
};

pixel_t corner_with_border_left_top[12][12] = {
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xe1808080, 0x87808080,
     0x46808080, 0x1c808080,
     0x07808080},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xe1808080, 0x59808080,
     0x037b7b7b, 0x00626262,
     0x004d4d4d, 0x00404040,
     0x003a3a3a},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xb1808080,
     0x0d7e7e7e, 0x005e5e5e,
     0x003d3d3d, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0x90808080, 0x02777777,
     0x00444444, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xff000000, 0xa8808080,
     0x01787878, 0x00404040,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xe1808080, 0x0c7e7e7e,
     0x00474747, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0x5a808080, 0x005e5e5e,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xe0808080,
     0x027b7b7b, 0x003c3c3c,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x88808080,
     0x00626262, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x47808080,
     0x004e4e4e, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x1b808080,
     0x00404040, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x07808080,
     0x003a3a3a, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
};

pixel_t corner_with_border_left_bottom[12][12] = {
    {0x0c808080,
     0x28808080, 0x47808080,
     0x87808080, 0xde808080,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000},
    {0x003b3b3b,
     0x00434343, 0x004d4d4d,
     0x00626262, 0x027b7b7b,
     0x59808080, 0xe1808080,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x003d3d3d,
     0x005e5e5e, 0x0c7d7d7d,
     0xa6808080, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00444444,
     0x01777777, 0x8e808080,
     0xff000000, 0xff000000,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00404040, 0x01787878,
     0xa8808080, 0xff000000,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00474747,
     0x0c7d7d7d, 0xe1808080,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x005e5e5e, 0x58808080,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x003c3c3c, 0x027b7b7b,
     0xe0808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00626262,
     0x88808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x004d4d4d,
     0x47808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00404040,
     0x1b808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x003a3a3a,
     0x06808080},
};

pixel_t corner_with_border_right_top[12][12] = {
    {0x07808080,
     0x003c3c3c, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x1b808080,
     0x00434343, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x46808080,
     0x004e4e4e, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0x88808080,
     0x00626262, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xdf808080,
     0x027b7b7b, 0x003c3c3c,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0x58808080, 0x005e5e5e,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xe1808080, 0x0c7d7d7d,
     0x00454545, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xff000000, 0xa7808080,
     0x01767676, 0x003f3f3f,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0x90808080, 0x02777777,
     0x00444444, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xb1808080,
     0x0d7d7d7d, 0x005e5e5e,
     0x003d3d3d, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xe1808080, 0x58808080,
     0x027b7b7b, 0x00626262,
     0x004d4d4d, 0x00404040,
     0x00393939},
    {0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xe1808080, 0x87808080,
     0x46808080, 0x1b808080,
     0x05808080},
};

pixel_t corner_with_border_right_bottom[12][12] = {
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x003a3a3a,
     0x06808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00404040,
     0x1b808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x004d4d4d,
     0x47808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00626262,
     0x88808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x003c3c3c, 0x027b7b7b,
     0xdf808080},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x005e5e5e, 0x58808080,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00444444,
     0x0c7d7d7d, 0xe1808080,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00404040, 0x01767676,
     0xa7808080, 0xff000000,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x00444444,
     0x01777777, 0x8c808080,
     0xff000000, 0xff000000,
     0xff000000},
    {0x00383838,
     0x00383838, 0x00383838,
     0x00383838, 0x003d3d3d,
     0x005e5e5e, 0x0c7d7d7d,
     0xa5808080, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000},
    {0x00393939,
     0x00404040, 0x004d4d4d,
     0x00626262, 0x027b7b7b,
     0x58808080, 0xe1808080,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000},
    {0x05808080,
     0x1b808080, 0x46808080,
     0x87808080, 0xdd808080,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000, 0xff000000,
     0xff000000},
};

// Sort layers by z-index and return sorted indices
static int sort_layers_by_z(layer_ctx_t *ctx) {
    int sorted_count = 0;

    // Collect indices of layers in use
    for (int i = 0; i < LAYER_MAX_NUM; i++) {
        if (ctx->layers[i].in_use) { ctx->sorted_indices[sorted_count++] = i; }
    }

    // Sort indices by z-index using insertion sort
    for (int i = 1; i < sorted_count; i++) {
        int key   = ctx->sorted_indices[i];
        int key_z = ctx->layers[key].z_index;
        int j     = i - 1;

        while (j >= 0 && ctx->layers[ctx->sorted_indices[j]].z_index > key_z) {
            ctx->sorted_indices[j + 1] = ctx->sorted_indices[j];
            j--;
        }
        ctx->sorted_indices[j + 1] = key;
    }

    return sorted_count;
}

int compositor_init(layer_ctx_t *ctx) {
    ctx->layer_num = 0;
    for (int i = 0; i < LAYER_MAX_NUM; i++) {
        ctx->layers[i].in_use    = false;
        ctx->layers[i].visible   = false;
        ctx->layers[i].pos_x     = 0;
        ctx->layers[i].pos_y     = 0;
        ctx->layers[i].width     = 0;
        ctx->layers[i].height    = 0;
        ctx->layers[i].buf       = NULL;
        ctx->layers[i].z_index   = 0;
        ctx->layers[i].use_alpha = false;
    }

    int splash_layer_index =
        create_layer(ctx, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
    fill(ctx, splash_layer_index, COLOR_BLACK);
    for (int i = 0; i < 5; i++) {
        rect(
            ctx,
            splash_layer_index,
            DISPLAY_WIDTH / 2 - 45 + i * 20,
            DISPLAY_HEIGHT / 2 - 15,
            10,
            30,
            COLOR_WHITE);
    }
    rect(
        ctx,
        splash_layer_index,
        DISPLAY_WIDTH / 2 - 35,
        DISPLAY_HEIGHT / 2 + 10,
        10,
        5,
        COLOR_WHITE);
    rect(
        ctx,
        splash_layer_index,
        DISPLAY_WIDTH / 2 + 5,
        DISPLAY_HEIGHT / 2 - 15,
        10,
        5,
        COLOR_WHITE);
    int pid = get_pid();
    render(ctx, pid);

    ctx->is_dirty   = false;
    ctx->dirty_rect = (dirty_rect_t){0, 0, 0, 0};
    return 0;
}

void mark_dirty(layer_ctx_t *ctx, int x1, int y1, int x2, int y2) {
    if (!ctx->is_dirty) {
        ctx->dirty_rect = (dirty_rect_t){x1, y1, x2, y2};
        ctx->is_dirty   = true;
    } else {
        // expand dirty rect
        ctx->dirty_rect.x1 = min(ctx->dirty_rect.x1, x1);
        ctx->dirty_rect.y1 = min(ctx->dirty_rect.y1, y1);
        ctx->dirty_rect.x2 = max(ctx->dirty_rect.x2, x2);
        ctx->dirty_rect.y2 = max(ctx->dirty_rect.y2, y2);
    }
}

int render(layer_ctx_t *ctx, int pid) {
    composite(ctx);
    putframe(
        screen_vram, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(pixel_t), pid);
    return 0;
}

int composite(layer_ctx_t *ctx) {
    if (!ctx->is_dirty) {
        return 0; // no dirty area, return
    }

    int sorted_count = sort_layers_by_z(ctx);
    ctx->top_z_index =
        ctx->layers[ctx->sorted_indices[sorted_count - 1]].z_index;

    // ensure dirty area in screen range
    int x1 = max(0, ctx->dirty_rect.x1);
    int y1 = max(0, ctx->dirty_rect.y1);
    int x2 = min(DISPLAY_WIDTH - 1, ctx->dirty_rect.x2);
    int y2 = min(DISPLAY_HEIGHT - 1, ctx->dirty_rect.y2);

    // update pixels in dirty area
    for (int i = 0; i < sorted_count; i++) {
        layer_t *layer = &ctx->layers[ctx->sorted_indices[i]];
        if (!layer->visible) continue;

        // calculate intersection of layer and dirty area
        int layer_x1 = max(x1, layer->pos_x);
        int layer_y1 = max(y1, layer->pos_y);
        int layer_x2 = min(x2, layer->pos_x + layer->width - 1);
        int layer_y2 = min(y2, layer->pos_y + layer->height - 1);

        if (layer_x1 > layer_x2 || layer_y1 > layer_y2) continue;

        for (int y = layer_y1; y <= layer_y2; y++) {
            for (int x = layer_x1; x <= layer_x2; x++) {
                int layer_local_x = x - layer->pos_x;
                int layer_local_y = y - layer->pos_y;

                pixel_t src_pixel =
                    layer->buf[layer_local_y * layer->width + layer_local_x];
                pixel_t dst_pixel = screen_vram[y * DISPLAY_WIDTH + x];

                // Get alpha value from highest 8 bits
                uint8_t alpha = (src_pixel >> 24) & 0xFF;

                if (alpha == 0x00) {
                    // Fully opaque
                    screen_vram[y * DISPLAY_WIDTH + x] = src_pixel;
                } else if (alpha != 0xFF) {
                    // Partially transparent - blend colors
                    float   transparency = alpha / 255.0f;
                    pixel_t blended =
                        blend(src_pixel, dst_pixel, 1.0f - transparency);
                    uint8_t r = (blended >> 16) & 0xFF;
                    uint8_t g = (blended >> 8) & 0xFF;
                    uint8_t b = blended & 0xFF;

                    screen_vram[y * DISPLAY_WIDTH + x] =
                        (r << 16) | (g << 8) | b;
                }
            }
        }
    }

    // reset dirty area
    ctx->is_dirty   = false;
    ctx->dirty_rect = (dirty_rect_t){0, 0, 0, 0};
    return 0;
}

int create_layer(
    layer_ctx_t *ctx,
    int          pos_x,
    int          pos_y,
    int          width,
    int          height,
    int          z_index) {
    int layer_index = -1;

    if (ctx->layer_num >= LAYER_MAX_NUM) {
        // layer num is full
        return -1;
    }

    for (int i = 0; i < LAYER_MAX_NUM; i++) {
        if (!ctx->layers[i].in_use) {
            layer_index           = i;
            ctx->layers[i].in_use = true;
            break;
        }
    }

    if (layer_index == -1) {
        // no free layer found
        return -1;
    }

    pixel_t *layer_buf = malloc(width * height * 4);
    if (!layer_buf) {
        // malloc failed
        return -1;
    }

    // clip position and dimensions to display bounds
    int clipped_x = pos_x < 0 ? 0 : pos_x;
    int clipped_y = pos_y < 0 ? 0 : pos_y;
    int clipped_width =
        clipped_x + width > DISPLAY_WIDTH ? DISPLAY_WIDTH - clipped_x : width;
    int clipped_height = clipped_y + height > DISPLAY_HEIGHT
                           ? DISPLAY_HEIGHT - clipped_y
                           : height;

    ctx->layers[layer_index].visible = true;
    ctx->layers[layer_index].pos_x   = clipped_x;
    ctx->layers[layer_index].pos_y   = clipped_y;
    ctx->layers[layer_index].width   = clipped_width;
    ctx->layers[layer_index].height  = clipped_height;
    ctx->layers[layer_index].buf     = layer_buf;
    ctx->layers[layer_index].z_index = z_index;
    ctx->layer_num++;

    sort_layers_by_z(ctx);
    return layer_index;
}

int release_layer(layer_ctx_t *ctx, int index) {
    ctx->layers[index].in_use  = false;
    ctx->layers[index].visible = false;
    ctx->layers[index].pos_x   = 0;
    ctx->layers[index].pos_y   = 0;
    ctx->layers[index].width   = 0;
    ctx->layers[index].height  = 0;
    free(ctx->layers[index].buf);
    ctx->layers[index].buf     = NULL;
    ctx->layers[index].z_index = 0;
    ctx->layer_num--;
    sort_layers_by_z(ctx);
    return 0;
}

int fill(layer_ctx_t *ctx, int layer_index, pixel_t color) {
    layer_t *layer = &ctx->layers[layer_index];
    for (int i = 0; i < layer->width * layer->height; i++) {
        layer->buf[i] = color;
    }
    // 标记整个图层区域为脏
    mark_dirty(
        ctx,
        layer->pos_x,
        layer->pos_y,
        layer->pos_x + layer->width - 1,
        layer->pos_y + layer->height - 1);
    return 0;
}

int rect(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x,
    int          y,
    int          width,
    int          height,
    pixel_t      color) {
    layer_t *layer = &ctx->layers[layer_index];
    // clip the rect to layer bounds
    int x1 = x < 0 ? 0 : x;
    int y1 = y < 0 ? 0 : y;
    int x2 = x + width > layer->width ? layer->width : x + width;
    int y2 = y + height > layer->height ? layer->height : y + height;

    // draw the clipped rect
    for (int row = y1; row < y2; row++) {
        for (int col = x1; col < x2; col++) {
            layer->buf[row * layer->width + col] = color;
        }
    }
    // mark affected area as dirty
    mark_dirty(
        ctx,
        layer->pos_x + x1,
        layer->pos_y + y1,
        layer->pos_x + x2 - 1,
        layer->pos_y + y2 - 1);
    return 0;
}

int move(layer_ctx_t *ctx, int layer_index, int x, int y) {
    layer_t *layer = &ctx->layers[layer_index];
    // mark old position as dirty
    mark_dirty(
        ctx,
        layer->pos_x,
        layer->pos_y,
        layer->pos_x + layer->width - 1,
        layer->pos_y + layer->height - 1);

    layer->pos_x = x;
    layer->pos_y = y;

    // mark new position as dirty
    mark_dirty(ctx, x, y, x + layer->width - 1, y + layer->height - 1);
    return 0;
}

float distance(int x1, int y1, int x2, int y2) {
    return sqrtf((float)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Blend two colors in gamma-correct linear space with alpha
pixel_t blend(pixel_t color1, pixel_t color2, float alpha) {
    // Convert to linear space
    float r1 = powf(((color1 >> 16) & 0xFF) / 255.0f, 2.2f);
    float g1 = powf(((color1 >> 8) & 0xFF) / 255.0f, 2.2f);
    float b1 = powf((color1 & 0xFF) / 255.0f, 2.2f);

    float r2 = powf(((color2 >> 16) & 0xFF) / 255.0f, 2.2f);
    float g2 = powf(((color2 >> 8) & 0xFF) / 255.0f, 2.2f);
    float b2 = powf((color2 & 0xFF) / 255.0f, 2.2f);

    // Blend in linear space
    float r = r1 * alpha + r2 * (1.0f - alpha);
    float g = g1 * alpha + g2 * (1.0f - alpha);
    float b = b1 * alpha + b2 * (1.0f - alpha);

    // Convert back to sRGB space
    uint8_t r_out = (uint8_t)(powf(r, 1.0f / 2.2f) * 255.0f);
    uint8_t g_out = (uint8_t)(powf(g, 1.0f / 2.2f) * 255.0f);
    uint8_t b_out = (uint8_t)(powf(b, 1.0f / 2.2f) * 255.0f);

    return (r_out << 16) | (g_out << 8) | b_out;
}

// Blend two colors with alpha using simple blending
pixel_t blend_simple(pixel_t color1, pixel_t color2, float alpha) {
    // Extract RGB components
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;

    // Simple linear blend
    uint8_t r = (uint8_t)(r1 * alpha + r2 * (1.0f - alpha));
    uint8_t g = (uint8_t)(g1 * alpha + g2 * (1.0f - alpha));
    uint8_t b = (uint8_t)(b1 * alpha + b2 * (1.0f - alpha));

    return (r << 16) | (g << 8) | b;
}

int circle(
    layer_ctx_t *ctx,
    int          layer_index,
    int          center_x,
    int          center_y,
    int          radius,
    pixel_t      color) {
    layer_t *layer = &ctx->layers[layer_index];
    for (int y = 0; y < layer->height; y++) {
        for (int x = 0; x < layer->width; x++) {
            float dist = distance(x, y, center_x, center_y);
            if (dist <= radius) {
                // Calculate alpha based on distance from edge
                float alpha = 1.0f;
                if (dist > radius - 1.0f) { alpha = radius - dist; }
                if (layer->buf[y * layer->width + x]
                    & 0xff000000 == 0xff000000) {
                    int transparency = (int)(255 - alpha * 255) & 0xff;
                    layer->buf[y * layer->width + x] =
                        color | (transparency << 24);
                } else {
                    layer->buf[y * layer->width + x] =
                        blend(color, layer->buf[y * layer->width + x], alpha);
                }
            }
        }
    }
    // mark circle bounding box as dirty
    mark_dirty(
        ctx,
        layer->pos_x + center_x - radius,
        layer->pos_y + center_y - radius,
        layer->pos_x + center_x + radius,
        layer->pos_y + center_y + radius);
    return 0;
}

int triangle(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x1,
    int          y1,
    int          x2,
    int          y2,
    int          x3,
    int          y3,
    pixel_t      color) {
    layer_t *layer = &ctx->layers[layer_index];

    // Find bounding box
    int min_x = min(min(x1, x2), x3);
    int max_x = max(max(x1, x2), x3);
    int min_y = min(min(y1, y2), y3);
    int max_y = max(max(y1, y2), y3);

    // Clip to layer bounds
    min_x = max(0, min_x);
    max_x = min(layer->width - 1, max_x);
    min_y = max(0, min_y);
    max_y = min(layer->height - 1, max_y);

    // Edge vectors
    int e1_x = x2 - x1;
    int e1_y = y2 - y1;
    int e2_x = x3 - x2;
    int e2_y = y3 - y2;
    int e3_x = x1 - x3;
    int e3_y = y1 - y3;

    // MSAA sample offsets (4x MSAA)
    const float sample_offsets[4][2] = {
        {0.25f, 0.25f},
        {0.75f, 0.25f},
        {0.25f, 0.75f},
        {0.75f, 0.75f}
    };

    // For each pixel in bounding box
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int samples_inside = 0;

            // Test each sample point
            for (int s = 0; s < 4; s++) {
                float sample_x = x + sample_offsets[s][0];
                float sample_y = y + sample_offsets[s][1];

                // Test if sample point is inside triangle using edge function
                float d1 = (sample_x - x1) * e1_y - (sample_y - y1) * e1_x;
                float d2 = (sample_x - x2) * e2_y - (sample_y - y2) * e2_x;
                float d3 = (sample_x - x3) * e3_y - (sample_y - y3) * e3_x;

                bool inside = (d1 >= 0 && d2 >= 0 && d3 >= 0)
                           || (d1 <= 0 && d2 <= 0 && d3 <= 0);
                if (inside) { samples_inside++; }
            }

            if (samples_inside > 0) {
                float alpha = samples_inside / 4.0f;
                layer->buf[y * layer->width + x] =
                    blend(color, layer->buf[y * layer->width + x], alpha);
            }
        }
    }

    // mark triangle bounding box as dirty
    mark_dirty(
        ctx,
        layer->pos_x + min_x,
        layer->pos_y + min_y,
        layer->pos_x + max_x,
        layer->pos_y + max_y);
    return 0;
}

int top(layer_ctx_t *ctx, int layer_index) {
    ctx->layers[layer_index].z_index = ctx->top_z_index + 1;
    ctx->top_z_index++;
    sort_layers_by_z(ctx);
    return 0;
}

int get_top_z_index(layer_ctx_t *ctx) {
    return ctx->top_z_index;
}

int use_image(layer_ctx_t *ctx, int layer_index, int image_index, float scale) {
    layer_t *layer = &ctx->layers[layer_index];

    int new_width  = images[image_index].width * scale;
    int new_height = images[image_index].height * scale;

    if (layer->width != new_width || layer->height != new_height) {
        resize(ctx, layer_index, new_width, new_height);
    }

    if (scale >= 2) {
        for (int y = 0; y < images[image_index].height; y++) {
            for (int x = 0; x < images[image_index].width; x++) {
                int offset = y * images[image_index].width + x;
                rect(
                    ctx,
                    layer_index,
                    x * scale,
                    y * scale,
                    scale,
                    scale,
                    images[image_index].buf[offset]);
            }
        }
    }

    if (scale == 1) {
        for (int y = 0; y < images[image_index].height; y++) {
            for (int x = 0; x < images[image_index].width; x++) {
                int offset         = y * images[image_index].width + x;
                layer->buf[offset] = images[image_index].buf[offset];
            }
        }
    }

    if (scale - 0.5 < 0.000001) {
        for (int y = 0; y < images[image_index].height / 2; y++) {
            for (int x = 0; x < images[image_index].width / 2; x++) {
                int src_offset =
                    (y * 2) * images[image_index].width + (x * 2) + 1;
                int dst_offset = y * (images[image_index].width / 2) + x;
                layer->buf[dst_offset] = images[image_index].buf[src_offset];
            }
        }
    }
    mark_dirty(ctx, layer->pos_x, layer->pos_y, layer->width, layer->height);
    return 0;
}

int use_char(
    layer_ctx_t *ctx, int layer_index, char ch, float scale, pixel_t color) {
    layer_t *layer = &ctx->layers[layer_index];

    assert(ch >= 'a' && ch <= 'z');

    int image_index = RESOURCE_CHAR_A + ch - 'a';

    int new_width  = images[image_index].width * scale;
    int new_height = images[image_index].height * scale;

    if (layer->width != new_width || layer->height != new_height) {
        resize(ctx, layer_index, new_width, new_height);
    }

    if (scale >= 2) {
        for (int y = 0; y < images[image_index].height; y++) {
            for (int x = 0; x < images[image_index].width; x++) {
                int offset = y * images[image_index].width + x;
                rect(
                    ctx,
                    layer_index,
                    x * scale,
                    y * scale,
                    scale,
                    scale,
                    images[image_index].buf[offset] | (0x00ffffff & color));
            }
        }
    }

    if (scale == 1) {
        for (int y = 0; y < images[image_index].height; y++) {
            for (int x = 0; x < images[image_index].width; x++) {
                int offset = y * images[image_index].width + x;
                layer->buf[offset] =
                    images[image_index].buf[offset] | (0x00ffffff & color);
            }
        }
    }

    if (scale - 0.5 < 0.000001) {
        for (int y = 0; y < images[image_index].height / 2; y++) {
            for (int x = 0; x < images[image_index].width / 2; x++) {
                int src_offset =
                    (y * 2) * images[image_index].width + (x * 2) + 1;
                int dst_offset = y * (images[image_index].width / 2) + x;
                layer->buf[dst_offset] =
                    images[image_index].buf[src_offset] | (0x00ffffff & color);
            }
        }
    }
    mark_dirty(ctx, layer->pos_x, layer->pos_y, layer->width, layer->height);
    return 0;
}

int use_text(
    layer_ctx_t *ctx, int layer_index, char *text, float scale, pixel_t color) {
    layer_t *layer       = &ctx->layers[layer_index];
    int      text_length = strlen(text);

    int width = 0;
    for (int i = 0; i < text_length; i++) {
        char ch          = text[i];
        int  image_index = 0;
        // FIXME: number map incorrect. currently remapped for workaround
        // 0123456789 -> 1234567890
        if (ch > '0' && ch <= '9') {
            image_index = RESOURCE_CHAR_0 + ch - '0' - 1;
        } else if (ch == '0') {
            image_index = RESOURCE_CHAR_9;
        } else if (ch >= 'a' && ch <= 'z') {
            image_index = RESOURCE_CHAR_A + ch - 'a';
        } else if (ch == '.') {
            image_index = RESOURCE_CHAR_PERIOD;
        } else if (ch == '+') {
            image_index = RESOURCE_CHAR_PLUS;
        } else if (ch == '-') {
            image_index = RESOURCE_CHAR_MINUS;
        } else if (ch == '*') {
            image_index = RESOURCE_CHAR_TIMES;
        } else if (ch == '/') {
            image_index = RESOURCE_CHAR_DIVIDED;
        } else if (ch == '=') {
            image_index = RESOURCE_CHAR_EQUAL;
        } else if (ch == ' ') {
            image_index = RESOURCE_CHAR_WHITESPACE;
        }
        width += images[image_index].width;
        if (i < text_length - 1) {
            width += 1; // Add 1 pixel padding between each character
        }
    }

    int height = images[RESOURCE_CHAR_A].height;

    int new_width  = width * scale;
    int new_height = height * scale;

    if (layer->width != new_width || layer->height != new_height) {
        resize(ctx, layer_index, new_width, new_height);
    }

    pixel_t *temp = malloc(width * height * 4);
    memset(temp, 0xff, width * height * 4);

    int current_left_top_x = 0;
    for (int i = 0; i < text_length; i++) {
        char ch = text[i];
        int  image_index;
        if (ch > '0' && ch <= '9') {
            image_index = RESOURCE_CHAR_0 + ch - '0' - 1;
        } else if (ch == '0') {
            image_index = RESOURCE_CHAR_9;
        } else if (ch >= 'a' && ch <= 'z') {
            image_index = RESOURCE_CHAR_A + ch - 'a';
        } else if (ch == '.') {
            image_index = RESOURCE_CHAR_PERIOD;
        } else if (ch == '+') {
            image_index = RESOURCE_CHAR_PLUS;
        } else if (ch == '-') {
            image_index = RESOURCE_CHAR_MINUS;
        } else if (ch == '*') {
            image_index = RESOURCE_CHAR_TIMES;
        } else if (ch == '/') {
            image_index = RESOURCE_CHAR_DIVIDED;
        } else if (ch == '=') {
            image_index = RESOURCE_CHAR_EQUAL;
        } else if (ch == ' ') {
            image_index = RESOURCE_CHAR_WHITESPACE;
        }
        for (int y = 0; y < images[image_index].height; y++) {
            for (int x = 0; x < images[image_index].width; x++) {
                int offset        = y * images[image_index].width + x;
                int temp_offset   = y * width + current_left_top_x + x;
                temp[temp_offset] = images[image_index].buf[offset];
            }
        }
        current_left_top_x += images[image_index].width + 1;
    }

    if (scale >= 2) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int offset = y * width + x;
                rect(
                    ctx,
                    layer_index,
                    x * scale,
                    y * scale,
                    scale,
                    scale,
                    temp[offset] | (0x00ffffff & color));
            }
        }
    }

    if (scale == 1) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int offset         = y * width + x;
                layer->buf[offset] = temp[offset] | (0x00ffffff & color);
            }
        }
    }

    if (scale - 0.5 < 0.000001) {
        for (int y = 0; y < height / 2; y++) {
            for (int x = 0; x < width / 2; x++) {
                int src_offset = (y * 2) * width + (x * 2) + 1;
                int dst_offset = y * (width / 2) + x;
                layer->buf[dst_offset] =
                    temp[src_offset] | (0x00ffffff & color);
            }
        }
    }
    mark_dirty(ctx, layer->pos_x, layer->pos_y, layer->width, layer->height);
    return 0;
}

void sort_layer(layer_ctx_t *ctx) {
    sort_layers_by_z(ctx);
}

int resize(layer_ctx_t *ctx, int layer_index, int width, int height) {
    layer_t *layer   = &ctx->layers[layer_index];
    pixel_t *old_buf = layer->buf;
    layer->buf       = malloc(width * height * 4);
    if (!layer->buf) { return -1; }
    layer->width  = width;
    layer->height = height;
    memset(layer->buf, 0, width * height * 4);
    free(old_buf);
    // mark old area as dirty
    mark_dirty(
        ctx,
        layer->pos_x,
        layer->pos_y,
        layer->pos_x + layer->width - 1,
        layer->pos_y + layer->height - 1);
    // mark new area as dirty
    mark_dirty(
        ctx,
        layer->pos_x,
        layer->pos_y,
        layer->pos_x + width - 1,
        layer->pos_y + height - 1);
    return 0;
}

int clear(layer_ctx_t *ctx, int layer_index) {
    layer_t *layer = &ctx->layers[layer_index];
    fill(ctx, layer_index, COLOR_TRANSPARENT);
    // fill() has handled dirty area marking
    return 0;
}

int show(layer_ctx_t *ctx, int layer_index) {
    layer_t *layer                   = &ctx->layers[layer_index];
    ctx->layers[layer_index].visible = true;
    // mark whole layer area as dirty
    mark_dirty(
        ctx,
        layer->pos_x,
        layer->pos_y,
        layer->pos_x + layer->width - 1,
        layer->pos_y + layer->height - 1);
    return 0;
}

int hide(layer_ctx_t *ctx, int layer_index) {
    layer_t *layer                   = &ctx->layers[layer_index];
    ctx->layers[layer_index].visible = false;
    // mark whole layer area as dirty
    mark_dirty(
        ctx,
        layer->pos_x,
        layer->pos_y,
        layer->pos_x + layer->width - 1,
        layer->pos_y + layer->height - 1);
    return 0;
}

int rounded_rect(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x,
    int          y,
    int          width,
    int          height,
    int          radius,
    pixel_t      color) {
    assert(radius <= width / 2 && radius <= height / 2);
    pixel_t *buf = ctx->layers[layer_index].buf;

    if (radius == 11 && color == 0x383838) {
        // special case for window border
        rect(ctx, layer_index, x, y, width, height, color);
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                // FIXME: not blending color, will overwrite previous color
                buf[(i + y) * (width + 2 * x) + j + x] =
                    corner_pattern_left_top[i][j];
                buf[(i + y) * (width + 2 * x) + j + x + width - 11] =
                    corner_pattern_right_top[i][j];
                buf[(i + y + height - 11) * (width + 2 * x) + j + x] =
                    corner_pattern_left_bottom[i][j];
                buf[(i + y + height - 11) * (width + 2 * x) + j + x + width
                    - 11] = corner_pattern_right_bottom[i][j];
            }
        }
    } else {
        circle(ctx, layer_index, x + radius, y + radius, radius, color);
        circle(ctx, layer_index, x + width - radius, y + radius, radius, color);
        circle(
            ctx, layer_index, x + radius, y + height - radius, radius, color);
        circle(
            ctx,
            layer_index,
            x + width - radius,
            y + height - radius,
            radius,
            color);
        rect(
            ctx,
            layer_index,
            x + radius,
            y + 1,
            width - radius * 2,
            radius,
            color);
        rect(
            ctx,
            layer_index,
            x + radius,
            y + height - radius,
            width - radius * 2,
            radius,
            color);
        rect(
            ctx,
            layer_index,
            x + 1,
            y + radius,
            width - 1,
            height - radius * 2,
            color);
    }
    return 0;
}

// FIXME: only for window border acceleration. Full functionality is not
// supported yet.
int rounded_rect_stroke(
    layer_ctx_t *ctx,
    int          layer_index,
    int          x,
    int          y,
    int          width,
    int          height,
    int          radius,
    pixel_t      color) {
    assert(radius <= width / 2 && radius <= height / 2);
    assert(color == 0x808080);
    assert(radius == 12);
    pixel_t *buf = ctx->layers[layer_index].buf;

    // special case for window border
    rect(ctx, layer_index, x, y, width, 1, color);
    rect(ctx, layer_index, x, y + height - 1, width, 1, color);
    rect(ctx, layer_index, x, y + 1, 1, height - 2, color);
    rect(ctx, layer_index, x + width - 1, y + 1, 1, height - 2, color);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            // FIXME: not blending color, will overwrite previous color
            buf[(i + y) * (width + 2 * x) + j + x] =
                corner_with_border_left_top[i][j];
            buf[(i + y) * (width + 2 * x) + j + x + width - 12] =
                corner_with_border_left_bottom[i][j];
            buf[(i + y + height - 12) * (width + 2 * x) + j + x] =
                corner_with_border_right_top[i][j];
            buf[(i + y + height - 12) * (width + 2 * x) + j + x + width - 12] =
                corner_with_border_right_bottom[i][j];
        }
    }
    return 0;
}
