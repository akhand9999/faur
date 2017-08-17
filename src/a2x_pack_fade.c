/*
    Copyright 2010, 2016, 2017 Alex Margarit

    This file is part of a2x-framework.

    a2x-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    a2x-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with a2x-framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "a2x_pack_fade.v.h"

static bool g_fadePending;
static unsigned g_frames;
static APixel g_savedColor;
static AScreen* g_capturedScreen;
static AScreen* g_oldCapturedScreen;

static A_STATE(a_fade__toColor);
static A_STATE(a_fade__fromColor);
static A_STATE(a_fade__screens);

static void allocateScreenBuffers(bool CaptureCurrentScreen)
{
    if(g_capturedScreen == NULL
        || !a_screen__sameSize(&a__screen, g_capturedScreen)) {

        if(g_capturedScreen != NULL) {
            a_screen_free(g_capturedScreen);
        }

        if(g_oldCapturedScreen != NULL) {
            a_screen_free(g_oldCapturedScreen);
            g_oldCapturedScreen = NULL;
        }

        g_capturedScreen = a_screen_new(a__screen.width, a__screen.height);
    }

    if(CaptureCurrentScreen) {
        if(g_oldCapturedScreen == NULL) {
            g_oldCapturedScreen = a_screen_new(a__screen.width,
                                               a__screen.height);
        }

        // Capture the screen before the caller will draw something new
        a_screen_copy(g_oldCapturedScreen, &a__screen);
    }
}

static void updateCapturedScreenBuffer(void)
{
    if(!a_screen__sameSize(&a__screen, g_capturedScreen)) {
        a_out__fatal("Screen size changed before fading");
    }

    a_screen_copy(g_capturedScreen, &a__screen);
}

void a_fade__init(void)
{
    g_fadePending = false;
    g_frames = 0;

    g_savedColor = 0;
    g_capturedScreen = NULL;
    g_oldCapturedScreen = NULL;

    a_state_new("a__fadeToColor", a_fade__toColor, "", "");
    a_state_new("a__fadeFromColor", a_fade__fromColor, "", "");
    a_state_new("a__fadeScreens", a_fade__screens, "", "");
}

void a_fade__uninit(void)
{
    if(g_capturedScreen != NULL) {
        a_screen_free(g_capturedScreen);
    }

    if(g_oldCapturedScreen != NULL) {
        a_screen_free(g_oldCapturedScreen);
    }
}

void a_fade_toColor(unsigned FramesDuration)
{
    if(g_fadePending) {
        a_out__warning("a_fade_toColor: fade pending, ignoring");
        return;
    }

    g_frames = FramesDuration;
    g_savedColor = a_pixel__state.pixel;
    allocateScreenBuffers(false);

    a_state_push("a__fadeToColor");
    g_fadePending = true;
}

void a_fade_fromColor(unsigned FramesDuration)
{
    if(g_fadePending) {
        a_out__warning("a_fade_fromColor: fade pending, ignoring");
        return;
    }

    g_frames = FramesDuration;
    g_savedColor = a_pixel__state.pixel;
    allocateScreenBuffers(false);

    a_state_push("a__fadeFromColor");
    g_fadePending = true;
}

void a_fade_screens(unsigned FramesDuration)
{
    if(g_fadePending) {
        a_out__warning("a_fade_screens: fade pending, ignoring");
        return;
    }

    g_frames = FramesDuration;
    allocateScreenBuffers(true);

    a_state_push("a__fadeScreens");
    g_fadePending = true;
}

static A_STATE(a_fade__toColor)
{
    A_STATE_BODY
    {
        updateCapturedScreenBuffer();

        AFix alpha = 0;
        AFix alpha_inc = a_fix_itofix(A_PIXEL_ALPHA_MAX) / (int)g_frames;

        a_pixel_push();
        a_pixel_setPixel(g_savedColor);

        A_STATE_LOOP
        {
            A_STATE_LOOP_DRAW
            {
                a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
                a_screen_blit(g_capturedScreen);

                a_pixel_setBlend(A_PIXEL_BLEND_RGBA);
                a_pixel_setAlpha(a_fix_fixtoi(alpha));
                a_draw_fill();
            }

            alpha += alpha_inc;

            if(alpha > a_fix_itofix(A_PIXEL_ALPHA_MAX)) {
                a_state_pop();
            }
        }

        a_pixel_pop();

        g_fadePending = false;
    }
}

static A_STATE(a_fade__fromColor)
{
    A_STATE_BODY
    {
        updateCapturedScreenBuffer();

        a_pixel_push();
        a_pixel_setPixel(g_savedColor);

        // For the first frame, before the LOOP body runs
        a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
        a_draw_fill();

        AFix alpha = a_fix_itofix(A_PIXEL_ALPHA_MAX);
        AFix alpha_inc = a_fix_itofix(A_PIXEL_ALPHA_MAX) / (int)g_frames;

        A_STATE_LOOP
        {
            A_STATE_LOOP_DRAW
            {
                a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
                a_screen_blit(g_capturedScreen);

                a_pixel_setBlend(A_PIXEL_BLEND_RGBA);
                a_pixel_setAlpha(a_fix_fixtoi(alpha));
                a_draw_fill();
            }

            alpha -= alpha_inc;

            if(alpha < 0) {
                a_state_pop();
            }
        }

        a_pixel_pop();

        g_fadePending = false;
    }
}

static A_STATE(a_fade__screens)
{
    A_STATE_BODY
    {
        updateCapturedScreenBuffer();

        AFix alpha = a_fix_itofix(A_PIXEL_ALPHA_MAX);
        AFix alpha_inc = a_fix_itofix(A_PIXEL_ALPHA_MAX) / (int)g_frames;

        a_pixel_push();

        // For the first frame, before the LOOP body runs
        a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
        a_screen_blit(g_oldCapturedScreen);

        A_STATE_LOOP
        {
            A_STATE_LOOP_DRAW
            {
                a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
                a_screen_blit(g_capturedScreen);

                a_pixel_setBlend(A_PIXEL_BLEND_RGBA);
                a_pixel_setAlpha(a_fix_fixtoi(alpha));
                a_screen_blit(g_oldCapturedScreen);
            }

            alpha -= alpha_inc;

            if(alpha < 0) {
                a_state_pop();
            }
        }

        a_pixel_pop();

        g_fadePending = false;
    }
}
