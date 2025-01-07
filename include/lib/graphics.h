#pragma once

#define COLOR_BLACK 			0x000000
#define COLOR_RED				0xff0000
#define COLOR_GREEN				0x00ff00
#define COLOR_YELLOW			0xffff00
#define COLOR_BLUE				0x0000ff
#define COLOR_PURPLE			0xff00ff
#define COLOR_LIGHTBLUE		    0x00ffff
#define COLOR_WHITE				0xffffff
#define COLOR_LIGHTGREY		    0xc6c6c6
#define COLOR_DARKRED			0x840000
#define COLOR_DARKGREEN		    0x008400
#define COLOR_DARKYELLOW	    0x848400
#define COLOR_DARKBLUE		    0x000084
#define COLOR_DARKPURPLE	    0x840084
#define COLOR_GREY		        0x848484

#define PAGE_SIZE 4096

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

/*!
 * \brief copy buffer to kernel space
 *
 * \param dst destination
 * \param src source
 * \param size size of buffer
 * \param pid pid of process
 *
 * \return 0 if success, -1 if fail
 */
int copy_to_kernel(uint32_t *dst, uint32_t *src, int size, int pid);
