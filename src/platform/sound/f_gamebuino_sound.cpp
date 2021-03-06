/*
    Copyright 2019-2020 Alex Margarit <alex@alxm.org>
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
    #include "f_gamebuino_sound.v.h"
}

#include <faur.v.h>

#if F_CONFIG_SOUND_ENABLED && F_CONFIG_SYSTEM_GAMEBUINO
#include <Arduino.h>
#include <Gamebuino-Meta.h>

bool f_platform_api__soundMuteGet(void)
{
    return false;
}

void f_platform_api__soundSampleFree(FPlatformSample* Sample)
{
    F_UNUSED(Sample);
}

void f_platform_api__soundSamplePlay(const FPlatformSample* Sample, int Channel, bool Loop)
{
    if(Sample == NULL) {
        return;
    }

    F_UNUSED(Channel);
    F_UNUSED(Loop);

    const FSample* sample = (const FSample*)Sample;

    gb.sound.stop(0);
    gb.sound.play(sample->buffer, sample->size);
}

void f_platform_api__soundSampleStop(int Channel)
{
    gb.sound.stop((int8_t)Channel);
}

bool f_platform_api__soundSampleIsPlaying(int Channel)
{
    return gb.sound.isPlaying((int8_t)Channel);
}
#endif // F_CONFIG_SOUND_ENABLED && F_CONFIG_SYSTEM_GAMEBUINO
