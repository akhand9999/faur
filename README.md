# ![Faur](./media/faur.png "Faur")

[![Build Status](https://travis-ci.org/alxm/faur.svg?branch=master)](https://travis-ci.org/alxm/faur)

*Faur* is a personal C framework I'm writing for my video games.

Features include 2D graphics, sound, inputs, state management, ECS model, and utilities to help with data, files, math, strings, time, and more.

Faur builds natively on Linux, and can be [cross-compiled](#cross-compile-for-other-platforms) for Web, Windows, and a variety of embedded devices.

## Dependencies & Path Setup

```sh
# Required
sudo apt install build-essential git python3
sudo apt install libsdl2-dev libsdl2-mixer-dev libpng-dev

# Optional
sudo apt install python3-pil ffmpeg
sudo apt install libsdl1.2-dev libsdl-mixer1.2-dev mingw-w64

# Clone repo to ~/faur
cd ~
git clone git://github.com/alxm/faur.git

# Set FAUR_PATH environment var
export FAUR_PATH="$HOME/faur"

# Add Faur's tools to $PATH
export PATH="$PATH:$FAUR_PATH/bin"
```

## *Hello, World* Project

```sh
faur-new hello
cd hello/build/make/
make run
```

![Hello, World screenshot](./media/hello.gif "Hello, World screenshot")

Move the square with the arrow keys or a gamepad.

### Generated Files

```sh
hello/
├── assets/
├── build/
│   └── make/
│       └── Makefile
└── src/
    └── main.c
```

#### hello/build/make/Makefile

```make
F_CONFIG_APP_AUTHOR := <author>
F_CONFIG_APP_NAME := hello

include $(FAUR_PATH)/make/dev.mk
```

#### hello/src/main.c

```c
#include <faur.h>

void f_main(void)
{
    static struct {
        int x, y;
        FButton *up, *down, *left, *right;
    } context;

    F_STATE_INIT
    {
        context.x = f_screen_sizeGetWidth() / 2;
        context.y = f_screen_sizeGetHeight() / 2;

        context.up = f_button_new();
        f_button_bindKey(context.up, F_KEY_UP);
        f_button_bindButton(context.up, NULL, F_BUTTON_UP);

        context.down = f_button_new();
        f_button_bindKey(context.down, F_KEY_DOWN);
        f_button_bindButton(context.down, NULL, F_BUTTON_DOWN);

        context.left = f_button_new();
        f_button_bindKey(context.left, F_KEY_LEFT);
        f_button_bindButton(context.left, NULL, F_BUTTON_LEFT);

        context.right = f_button_new();
        f_button_bindKey(context.right, F_KEY_RIGHT);
        f_button_bindButton(context.right, NULL, F_BUTTON_RIGHT);
    }

    F_STATE_TICK
    {
        if(f_button_pressGet(context.up)) {
            context.y--;
        }

        if(f_button_pressGet(context.down)) {
            context.y++;
        }

        if(f_button_pressGet(context.left)) {
            context.x--;
        }

        if(f_button_pressGet(context.right)) {
            context.x++;
        }
    }

    F_STATE_DRAW
    {
        f_color_colorSetHex(0xaaff88);
        f_draw_fill();

        f_color_colorSetHex(0xffaa44);
        f_draw_rectangle(context.x - 40, context.y - 40, 80, 80);
    }

    F_STATE_FREE
    {
        f_button_free(context.up);
        f_button_free(context.down);
        f_button_free(context.left);
        f_button_free(context.right);
    }
}
```

## Cross-Compile for Other Platforms

I first wrote Faur (formerly *a2x*) to make games for the Linux-based [GP2X handheld](https://www.alxm.org/games/gamepark.html). These are the supported platforms now:

#### Desktop

* Linux, FreeBSD *(native)*
* Windows *(MinGW-w64)*
* Web *(Emscripten)*

#### Embedded Linux

* GP2X, Wiz, Caanoo *(Open2x SDK, GPH SDK)*
* Open Pandora *(Pandora SDK)*

#### Arduino

* Gamebuino META *(Arduino SAMD)*
* Odroid-GO *(Arduino ESP32)*

The default toolchain paths are in faur/make/global/defs.mk and they can be overridden in ~/.config/faur/sdk.mk. To build for a different target, edit the application Makefile and replace `dev.mk` with another file from faur/make/.

## License

Copyright 2010-2020 Alex Margarit (alex@alxm.org)

* Source code licensed under [GNU GPL 3.0](https://www.gnu.org/licenses/gpl.html) (file `LICENSE`)
* Other content licensed under [CC BY-NC-ND 4.0](https://creativecommons.org/licenses/by-nc-nd/4.0/) (file `media/CC-BY-NC-ND`)

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. Faur is a personal framework to support my other software; to keep it focused and manageable, it is a solo endeavor. The project is named after the old Romanian word *faur*, often used in fables to mean *wizard blacksmith*. ⚒️✨
