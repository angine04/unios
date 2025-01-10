#pragma once

#ifndef USE_ASSETS_DATA
#error "assets_data.h can only be used once by compositor.c"
#endif

#include <lib/assets/icon_unios.res.h>
#include <lib/assets/image_wallpaper.res.h>
#include <lib/assets/test.res.h>
#include <lib/assets/icon_cursor.res.h>
#include <lib/assets/image_letters.res.h>
#include <lib/assets/icon_calc.res.h>
const image_t images[] = {
    {(pixel_t *)icon_unios, 64, 64, true},
    {(pixel_t *)image_wallpaper, 256, 192, false},
    {(pixel_t *)test, 64, 64, true},
    {(pixel_t *)icon_cursor, 15, 24, true},
    {(pixel_t *)image_letters, 692, 32, false},
    {(pixel_t *)icon_calc, 64, 64, true},
};
