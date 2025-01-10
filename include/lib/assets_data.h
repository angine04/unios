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

#include <lib/assets/char_a.res.h>
#include <lib/assets/char_b.res.h>
#include <lib/assets/char_c.res.h>
#include <lib/assets/char_d.res.h>
#include <lib/assets/char_e.res.h>
#include <lib/assets/char_f.res.h>
#include <lib/assets/char_g.res.h>
#include <lib/assets/char_h.res.h>
#include <lib/assets/char_i.res.h>
#include <lib/assets/char_j.res.h>
#include <lib/assets/char_k.res.h>
#include <lib/assets/char_l.res.h>
#include <lib/assets/char_m.res.h>
#include <lib/assets/char_n.res.h>
#include <lib/assets/char_o.res.h>
#include <lib/assets/char_p.res.h>
#include <lib/assets/char_q.res.h>
#include <lib/assets/char_r.res.h>
#include <lib/assets/char_s.res.h>
#include <lib/assets/char_t.res.h>
#include <lib/assets/char_u.res.h>
#include <lib/assets/char_v.res.h>
#include <lib/assets/char_w.res.h>
#include <lib/assets/char_x.res.h>
#include <lib/assets/char_y.res.h>
#include <lib/assets/char_z.res.h>



const image_t images[] = {
    {(pixel_t *)icon_unios, 64, 64, true},
    {(pixel_t *)image_wallpaper, 256, 192, false},
    {(pixel_t *)test, 64, 64, true},
    {(pixel_t *)icon_cursor, 15, 24, true},
    {(pixel_t *)image_letters, 692, 32, false},
    {(pixel_t *)icon_calc, 64, 64, true},
    {(pixel_t *)char_a, 18, 32, true},
    {(pixel_t *)char_b, 18, 32, true},
    {(pixel_t *)char_c, 16, 32, true},
    {(pixel_t *)char_d, 18, 32, true},
    {(pixel_t *)char_e, 16, 32, true},
    {(pixel_t *)char_f, 12, 32, true},
    {(pixel_t *)char_g, 18, 32, true},
    {(pixel_t *)char_h, 16, 32, true},
    {(pixel_t *)char_i, 6, 32, true},
    {(pixel_t *)char_j, 10, 32, true},
    {(pixel_t *)char_k, 16, 32, true},
    {(pixel_t *)char_l, 6, 32, true},
    {(pixel_t *)char_m, 24, 32, true},
    {(pixel_t *)char_n, 16, 32, true},
    {(pixel_t *)char_o, 18, 32, true},
    {(pixel_t *)char_p, 16, 32, true},
    {(pixel_t *)char_q, 16, 32, true},
    {(pixel_t *)char_r, 12, 32, true},
    {(pixel_t *)char_s, 14, 32, true},
    {(pixel_t *)char_t, 12, 32, true},
    {(pixel_t *)char_u, 16, 32, true},
    {(pixel_t *)char_v, 18, 32, true},
    {(pixel_t *)char_w, 24, 32, true},
    {(pixel_t *)char_x, 16, 32, true},
    {(pixel_t *)char_y, 18, 32, true},
    {(pixel_t *)char_z, 14, 32, true},
};
