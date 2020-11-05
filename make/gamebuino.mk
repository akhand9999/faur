include $(FAUR_PATH)/make/global/defs.mk

F_CONFIG_BUILD_ARDUINO_BOARD := gamebuino:samd:gamebuino_meta_native
F_CONFIG_BUILD_ARDUINO_PORT ?= /dev/ttyACM0
F_CONFIG_BUILD_GEN_CODE := 0
F_CONFIG_DEBUG_FATAL_SPIN := 1
F_CONFIG_DEBUG_NO_CONSOLE := 1
F_CONFIG_FILES_EMBED_ENABLED := 0
F_CONFIG_FILES_STANDARD := 0
F_CONFIG_FPS_CONSTANT_CREDIT := 1
F_CONFIG_SCREEN_FORMAT := F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_FULLSCREEN := 1
F_CONFIG_SCREEN_HARDWARE_WIDTH := 80
F_CONFIG_SCREEN_HARDWARE_HEIGHT := 64
F_CONFIG_SOUND_ENABLED_MUSIC := 0
F_CONFIG_SOUND_VOLUME_ADJUSTABLE := 0
F_CONFIG_SYSTEM_ARDUINO := 1
F_CONFIG_SYSTEM_GAMEBUINO := 1
F_CONFIG_TRAIT_CUSTOM_MAIN := 1
F_CONFIG_TRAIT_LOW_MEM := 1
F_CONFIG_TRAIT_NO_SEEDING := 1

include $(FAUR_PATH)/make/global/config.mk
include $(FAUR_PATH)/make/global/rules-arduino.mk
