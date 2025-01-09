#pragma once

#ifndef USE_ASSETS_DATA
#error "assets_data.h can only be used once by compositor.c"
#endif

#include <lib/assets/icon_unios.res.h>
#include <lib/assets/test.res.h>

const resource_t resources[] = {
    {(pixel_t *)icon_unios, 64, 64, true},
    {(pixel_t *)test, 64, 64, true},
};
