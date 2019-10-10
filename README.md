# ![a2x](https://github.com/alxm/a2x/raw/master/media/a2x-logo.png "a2x") [![Build Status](https://travis-ci.org/alxm/a2x.svg?branch=master)](https://travis-ci.org/alxm/a2x)

a2x is a C framework I wrote for my video games. It runs natively on Linux and FreeBSD, and can be cross-compiled for Windows, GP2X, Wiz, Caanoo, Open Pandora, and HTML5.

## Dependencies & Path Setup

```sh
# Basics
sudo apt install build-essential python3 git

# Core libraries
sudo apt install libpng-dev libsdl2-dev libsdl2-mixer-dev

# Optional extras
sudo apt install libsdl1.2-dev libsdl-mixer1.2-dev mingw-w64

# Clone repo to ~/a2x
cd ~
git clone git://github.com/alxm/a2x.git

# Set environment vars
export A2X_PATH="$HOME/a2x"
export PATH="$PATH:$A2X_PATH/bin"
```

## *Hello, World* Project

```sh
a2x_new hello
cd hello/build/make/
make -j run
```

![Starter Project Screenshot](https://github.com/alxm/a2x/raw/master/media/hello.gif "Starter Project Screenshot")

Move the square with the arrow keys or gamepad. Here are the generated `main.c` and `Makefile`:

#### main.c

```C
#include <a2x.h>

AStateHandler drawBox;

void a_main(void)
{
    a_state_push(drawBox);
}

void drawBox(void)
{
    static struct {
        int x, y;
    } context;

    A_STATE_INIT
    {
        context.x = a_screen_sizeGetWidth() / 2;
        context.y = a_screen_sizeGetHeight() / 2;
    }

    A_STATE_TICK
    {
        if(a_input_readKey(A_KEY_UP) || a_input_readButton(A_BUTTON_UP)) {
            context.y--;
        }

        if(a_input_readKey(A_KEY_DOWN) || a_input_readButton(A_BUTTON_DOWN)) {
            context.y++;
        }

        if(a_input_readKey(A_KEY_LEFT) || a_input_readButton(A_BUTTON_LEFT)) {
            context.x--;
        }

        if(a_input_readKey(A_KEY_RIGHT) || a_input_readButton(A_BUTTON_RIGHT)) {
            context.x++;
        }
    }

    A_STATE_DRAW
    {
        a_color_baseSetHex(0xaaff88);
        a_draw_fill();

        a_color_baseSetHex(0xffaa44);
        a_draw_rectangle(context.x - 40, context.y - 40, 80, 80);
    }

    A_STATE_FREE
    {
        a_out_info("Bye!");
    }
}
```

#### Makefile

```make
A_CONFIG_APP_AUTHOR := you
A_CONFIG_APP_NAME := hello

A_CONFIG_APP_VERSION_MAJOR := 1
A_CONFIG_APP_VERSION_MINOR := 0
A_CONFIG_APP_VERSION_MICRO := 0

include $(A2X_PATH)/make/dev.mk
```

## Cross-Compile for Other Platforms

I originally wrote a2x to make games for the Linux-based [GP2X handheld](https://www.alxm.org/games/gamepark.html). The following targets are also supported:

* Emscripten (HTML5)
* GPH SDK (Caanoo)
* MinGW (Windows)
* Open2x SDK (GP2X, Wiz)
* Pandora SDK (Open Pandora)

Edit `~/.config/a2x/sdk.config` with your SDK paths, then build a default project to sync your changes.

## License

Copyright 2010-2019 Alex Margarit (alex@alxm.org)

* Source code licensed under [GNU GPL 3](https://www.gnu.org/licenses/gpl.html) (file `LICENSE`)
* Other content licensed under [CC BY-NC-ND 4.0](https://creativecommons.org/licenses/by-nc-nd/4.0/) (file `media/CC-BY-NC-ND`)

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. I am writing a2x to support my other software; to keep it focused and manageable, it is a solo endeavor.