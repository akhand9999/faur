//
// Generated by `faur-build-embed-gfx /alex/files/dev/open/faur/media/g_palette.png /alex/files/dev/open/faur/src/generated/media/g_palette.png.c _g_palette 0xFF00FF`
//
#include <faur.v.h>

#ifndef F__GEN_GFX_CONST
    #if F_CONFIG_RENDER_SOFTWARE
        #define F__GEN_GFX_CONST const
    #else
        #define F__GEN_GFX_CONST
    #endif
#endif

static const FColorPixel f__gfx_buffer___g_palette[10 * 2 * 1] = {
    #if F_CONFIG_SCREEN_FORMAT == F_COLOR_FORMAT_RGB_565
        0x0000,0xf81f,0x07e0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1861,0x28e3,0xacd3,0xcdd7,0x4c7b,0x5cfd,0x4df3,0x5e75,0xae67,0xbee9
    #elif F_CONFIG_SCREEN_FORMAT == F_COLOR_FORMAT_RGB_565_B
        0x0000,0x1ff8,0xe007,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x6118,0xe328,0xd3ac,0xd7cd,0x7b4c,0xfd5c,0xf34d,0x755e,0x67ae,0xe9be
    #elif F_CONFIG_SCREEN_FORMAT == F_COLOR_FORMAT_RGBA_5551
        0x0000,0xf83e,0x07c0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1842,0x28c6,0xace6,0xcdee,0x4c76,0x5cfa,0x4de6,0x5e6a,0xae4e,0xbed2
    #elif F_CONFIG_SCREEN_FORMAT == F_COLOR_FORMAT_RGBA_8888
        0x00000000,0xff00ff00,0x00ff0000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x1f0f0f00,0x2f1f1f00,0xaf989800,0xcfb8b800,0x4f8fdf00,0x5f9fef00,0x4fbf9f00,0x5fcfaf00,0xa8cf3f00,0xb8df4f00
    #elif F_CONFIG_SCREEN_FORMAT == F_COLOR_FORMAT_ARGB_8888
        0x00000000,0x00ff00ff,0x0000ff00,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x001f0f0f,0x002f1f1f,0x00af9898,0x00cfb8b8,0x004f8fdf,0x005f9fef,0x004fbf9f,0x005fcfaf,0x00a8cf3f,0x00b8df4f
    #elif F_CONFIG_SCREEN_FORMAT == F_COLOR_FORMAT_ABGR_8888
        0x00000000,0x00ff00ff,0x0000ff00,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x000f0f1f,0x001f1f2f,0x009898af,0x00b8b8cf,0x00df8f4f,0x00ef9f5f,0x009fbf4f,0x00afcf5f,0x003fcfa8,0x004fdfb8
    #else
        #error Unknown color format
    #endif
};

#if F_CONFIG_RENDER_SOFTWARE
static const FSpriteWord f__gfx_spans___g_palette__0[] = {7,1,1,8,3,10};

static const struct {
    FSpriteWord framesNum;
    FSpriteWord* spans[1];
} f__gfx_texture___g_palette = {
    1,
    {
        (FSpriteWord*)f__gfx_spans___g_palette__0,
    },
};
#endif

static F__GEN_GFX_CONST FSprite f__gfx_sprite___g_palette = {
    {
        .size = {10, 2},
        .framesNum = 1,
        .bufferLen = 10 * 2,
        .bufferSize = 10 * 2 * sizeof(FColorPixel),
        .flags = F_PIXELS__CONST,
        .buffer = (FColorPixel*)f__gfx_buffer___g_palette,
    },
    #if F_CONFIG_RENDER_SOFTWARE
        (FPlatformTexture*)&f__gfx_texture___g_palette,
    #else
        NULL,
    #endif
};

const FSprite* const f_gfx__g_palette = (const FSprite*)&f__gfx_sprite___g_palette;
