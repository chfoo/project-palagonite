###############################################################################
# Makefile for the project 
###############################################################################

## General Flags
GAME= game
MCU = atmega644
TARGET = $(GAME).elf
CC = avr-gcc
INFO=../gameinfo.properties
SRC_DIR=../src
GAME_SOURCES=$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp $(SRC_DIR)/*/*/*.cpp $(SRC_DIR)/*/*/*/*.cpp)
PYTHON=python3

## Kernel settings
KERNEL_DIR = $(UZEBOX_KERNEL_DIR)
KERNEL_OPTIONS  = -DVIDEO_MODE=3 -DINTRO_LOGO=0 -DMAX_SPRITES=20 -DRAM_TILES_COUNT=24 -DSCROLLING=1 -DSOUND_MIXER=1


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -std=gnu99 -DF_CPU=28636360UL -fsigned-char -ffunction-sections -fno-toplevel-reorder
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 
CFLAGS += $(KERNEL_OPTIONS)
CFLAGS += -fno-exceptions -std=gnu++11

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Debugging / Optimization
CFLAGS += -O2
#ASMFLAGS += -g3 -Wa,-gdwarf2
#CFLAGS += -g3 -gdwarf-2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=$(GAME).map 
LDFLAGS += -Wl,-gc-sections 
LDFLAGS += -Wl,--section-start,.noinit=0x800100 -Wl,--section-start,.data=0x800500  # Mode 3 scrolling

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Objects that must be built in order to link
GAME_OBJECTS = $(subst $(SRC_DIR)/,,$(GAME_SOURCES:.cpp=.o))
OBJECTS = uzeboxVideoEngineCore.o uzeboxCore.o uzeboxSoundEngine.o uzeboxSoundEngineCore.o uzeboxVideoEngine.o $(GAME_OBJECTS)

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Include Directories
INCLUDES = -I"$(KERNEL_DIR)" -I./

.PHONY: clean py_tilesheet py_menu

## Build
all: $(TARGET) $(GAME).hex $(GAME).eep $(GAME).lss $(GAME).uze size

showvar:
	echo Game sources: $(GAME_SOURCES)
	echo Game objects: $(GAME_OBJECTS)
	echo Objects: $(OBJECTS)

## Compile Kernel files
uzeboxVideoEngineCore.o: $(KERNEL_DIR)/uzeboxVideoEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxSoundEngineCore.o: $(KERNEL_DIR)/uzeboxSoundEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxCore.o: $(KERNEL_DIR)/uzeboxCore.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxSoundEngine.o: $(KERNEL_DIR)/uzeboxSoundEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxVideoEngine.o: $(KERNEL_DIR)/uzeboxVideoEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

## Compile game sources
PY_TARGETS=py_tilesheet py_menu

%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
lib/%.o: $(SRC_DIR)/lib/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $< -o $@

lib/gfx/%.o: $(SRC_DIR)/lib/gfx/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $< -o $@

lib/gfx/model/%.o: $(SRC_DIR)/lib/gfx/model/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $< -o $@

lib/gfx/widget/%.o: $(SRC_DIR)/lib/gfx/widget/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $< -o $@

py_tilesheet:
	$(PYTHON) ../data/tilesheet.py ../data/font/ ../data/tiles/ > tilesheet.inc

py_menu:
	$(PYTHON) ../data/menu/menu.py > menu.inc

##Link
$(TARGET): $(PY_TARGETS) $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

%.uze: $(TARGET)
	-$(UZEBIN_DIR)packrom $(GAME).hex $@ $(INFO)

UNAME := $(shell sh -c 'uname -s 2>/dev/null || echo not')
AVRSIZEFLAGS := -A ${TARGET}
ifneq (,$(findstring MINGW,$(UNAME)))
AVRSIZEFLAGS := -C --mcu=${MCU} ${TARGET}
endif

size: ${TARGET}
	@echo
	@avr-size ${AVRSIZEFLAGS}

## Clean target
clean:
	-rm -rf $(OBJECTS) $(GAME).* dep/* *.uze tilesheet.inc menu.inc


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)
