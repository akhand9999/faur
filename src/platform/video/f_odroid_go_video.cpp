/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Faur, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

extern "C" {
    #include "f_odroid_go_video.v.h"
}

#include <faur.v.h>

#if F_CONFIG_SYSTEM_ODROID_GO
#include <odroid_go.h>

#define F__SCREEN_SIZE \
    (F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_SIZE_HEIGHT)

static FPixels g_pixels;
static FColorPixel* g_screenBuffer;
static FColorPixel* g_scaledBuffer;
static const FVecInt g_screenSize = {F_CONFIG_SCREEN_SIZE_WIDTH,
                                     F_CONFIG_SCREEN_SIZE_HEIGHT};

void f_platform_api__screenInit(void)
{
    g_screenBuffer =
        (FColorPixel*)f_mem_malloc(
            (F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_SIZE_HEIGHT
                * sizeof(FColorPixel)));

    g_scaledBuffer =
        (FColorPixel*)f_mem_malloc(
            (F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_ZOOM
                * F_CONFIG_SCREEN_SIZE_HEIGHT * F_CONFIG_SCREEN_ZOOM
                    * sizeof(FColorPixel)));

    f_pixels__init(
        &g_pixels, g_screenSize.x, g_screenSize.y, 1, (FPixelsFlags)0);
    f_pixels__bufferSet(
        &g_pixels, g_screenBuffer, g_screenSize.x, g_screenSize.y);
}

void f_platform_api__screenUninit(void)
{
}

void f_platform_api__screenClear(void)
{
    GO.lcd.fillScreen(f__color.pixel);
}

void f_platform_api__screenShow(void)
{
    #if F_CONFIG_DEBUG
        f_font_printf("%u", f_fps_rateDrawGetMax());
    #endif

    // Convert to little-endian
    uint16_t* pixels = g_screenBuffer;

    for(int i = F__SCREEN_SIZE; i--; ) {
        uint16_t p = *pixels;

        *pixels++ = (p << 8) | (p >> 8);
    }

    FColorPixel* dst = g_scaledBuffer;
    const FColorPixel* src = g_screenBuffer;

    for(int y = F_CONFIG_SCREEN_SIZE_HEIGHT; y--; ) {
        const FColorPixel* firstLine = dst;

        for(int x = F_CONFIG_SCREEN_SIZE_WIDTH; x--; ) {
            for(int z = F_CONFIG_SCREEN_ZOOM; z--; ) {
                *dst++ = *src;
            }

            src++;
        }

        for(int z = F_CONFIG_SCREEN_ZOOM - 1; z--; ) {
            memcpy(dst,
                   firstLine,
                   F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_ZOOM
                    * sizeof(FColorPixel));

            dst += F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_ZOOM;
        }
    }

    GO.lcd.drawBitmap(
        (F_CONFIG_SCREEN_HARDWARE_WIDTH
            - F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_ZOOM) / 2,
        (F_CONFIG_SCREEN_HARDWARE_HEIGHT
            - F_CONFIG_SCREEN_SIZE_HEIGHT * F_CONFIG_SCREEN_ZOOM) / 2,
        F_CONFIG_SCREEN_SIZE_WIDTH * F_CONFIG_SCREEN_ZOOM,
        F_CONFIG_SCREEN_SIZE_HEIGHT * F_CONFIG_SCREEN_ZOOM,
        g_scaledBuffer);
}

bool f_platform_api__screenVsyncGet(void)
{
    return false;
}

FVecInt f_platform_api__screenSizeGet(void)
{
    return g_screenSize;
}

FPixels* f_platform_api__screenPixelsGet(void)
{
    return &g_pixels;
}
#endif // F_CONFIG_SYSTEM_ODROID_GO
