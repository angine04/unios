#pragma once
#include <stdint.h>
#include <stddef.h>

#define COLOR_BLACK 			0x000000
#define COLOR_RED				0xff0000
#define COLOR_GREEN				0x00ff00
#define COLOR_YELLOW			0xffff00
#define COLOR_BLUE				0x0000ff
#define COLOR_PURPLE			0xff00ff
#define COLOR_LIGHTBLUE		    0x00ffff
#define COLOR_WHITE				0xffffff
#define COLOR_LIGHTGREY		    0xc0c0c0
#define COLOR_DARKRED			0x840000
#define COLOR_DARKGREEN		    0x008400
#define COLOR_DARKYELLOW	    0x848400
#define COLOR_DARKBLUE		    0x000084
#define COLOR_DARKPURPLE	    0x840084
#define COLOR_GREY		        0x848484
#define COLOR_TEAL			    0x008080
#define COLOR_SILVER		    0x808080
#define COLOR_TRANSPARENT	    0xffffffff

#define DISPLAY_WIDTH  1024
#define DISPLAY_HEIGHT 768

typedef uint32_t pixel_t;

/*!
 * \brief put frame to video memory
 *
 * \param frame frame buffer
 * \param size size of frame buffer
 * \param pid pid of process
 *
 * \return 0 if success, -1 if fail
 */
void putframe(void *frame, size_t size, int pid);
