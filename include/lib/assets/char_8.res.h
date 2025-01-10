#pragma once
#include <stdint.h>
#include <graphics.h>

pixel_t char_8[32][16] = {
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xbb000000, 0x77000000, 0x44000000, 0x44000000, 0x44000000, 0x88000000, 0xcc000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xdd000000, 0x44000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x55000000, 0xee000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xcc000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x33000000, 0xee000000, 0xff000000, 0xff000000},
    {0xff000000, 0x33000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x33000000, 0xff000000, 0xff000000},
    {0x88000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x44000000, 0xaa000000, 0xbb000000, 0x88000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x99000000, 0xff000000},
    {0x33000000, 0x00000000, 0x00000000, 0x00000000, 0x44000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xaa000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x55000000, 0xff000000},
    {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x99000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x22000000, 0x00000000, 0x00000000, 0x00000000, 0x11000000, 0xff000000},
    {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xbb000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x44000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000},
    {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x88000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xee000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x22000000, 0xff000000},
    {0x22000000, 0x00000000, 0x00000000, 0x00000000, 0x11000000, 0xcc000000, 0xff000000, 0xff000000, 0xff000000, 0x66000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x44000000, 0xff000000},
    {0x88000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x44000000, 0x66000000, 0x22000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x88000000, 0xff000000},
    {0xee000000, 0x22000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xdd000000, 0xff000000},
    {0xff000000, 0xcc000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x44000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xdd000000, 0x55000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x99000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xee000000, 0xbb000000, 0xbb000000, 0xdd000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x22000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x88000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x99000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xee000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x22000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x88000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x99000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xee000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x22000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x88000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x99000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x22000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
    {0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000},
};