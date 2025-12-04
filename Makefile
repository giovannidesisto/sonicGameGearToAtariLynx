##
 # Atari Gamer Common Lynx Library.
 # Copyright 2022 Igor Kromin (atarigamer.com)
 #
 # Licensed under the Apache License, Version 2.0 (the "License"); 
 # you may not use this file except in compliance with the License. 
 # You may obtain a copy of the License at
 #
 # http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software 
 # distributed under the License is distributed on an "AS IS" BASIS, 
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 # See the License for the specific language governing permissions and 
 # limitations under the License.
 ##

ROMNAME=MYROM
BIN_LNX = ${ROMNAME}.LNX
BIN_BLL = ${ROMNAME}.O

SOURCE_DIR=src
BUILD_DIR=build

CC65=$(CC65_HOME)
CC65_BIN=$(CC65)/bin
CC65_INC=$(CC65)/include
CC65_ASMINC=$(CC65)/asminc
CC65_TOOLS=$(CC65_BIN)

# Tool executables
CO=co65
CC=cc65
AS=ca65
AR=ar65
CL=cl65
SPRPCK=sprpck
SPRHLPR=./sprhlpr.sh
CP=cp
RM=rm -f
ECHO=echo

# Tool flags
SYS=lynx
TGI=$(SYS)-160-102-16
JOY=$(SYS)-stdjoy
SEGMENTS=--code-name CODE --rodata-name RODATA --bss-name BSS --data-name DATA
ASFLAGS=-t $(SYS) -I $(CC65_ASMINC)
CFLAGS=$(SEGMENTS) -I src/headers -t $(SYS) -Cl -O -Or
LDFLAGS_LNX=-C lynx.cfg -t $(SYS) $(SYS).lib
LDFLAGS_BLL=-C lynx_bll.cfg -t $(SYS) $(SYS).lib
# default bit depth is 1-bit per pixel
SPRFLAGS=-t6 -p2 -s4

.SUFFIXES:
.PHONY: clean mkbuild all
VPATH = src/c src/images src/asm

# == Assembly sources to assembly objects
ASM_SOURCES = $(wildcard $(SOURCE_DIR)/asm/*.s)
ASM_SOURCES_NOHDR1 = $(subst $(SOURCE_DIR)/asm/lnxhdr.s,,${ASM_SOURCES})
ASM_SOURCES_NOHDR = $(subst $(SOURCE_DIR)/asm/newbootldr.s,,${ASM_SOURCES_NOHDR1})
ASM_OBJECTS = $(patsubst $(SOURCE_DIR)/asm/%.s, $(BUILD_DIR)/%.o, $(ASM_SOURCES))
$(BUILD_DIR)/%.o: %.s
	@$(ECHO) == Assembling $< =\> $@
	@$(AS) $(ASFLAGS) $< -o $@

# == C sources to assembly intermediate files to C objects
C_SOURCES = $(wildcard $(SOURCE_DIR)/c/*.c)
C_OBJECTS = $(patsubst $(SOURCE_DIR)/c/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
$(BUILD_DIR)/%.o: %.c
	@$(ECHO) == Compiling $< =\> $@
	@$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@

# == Bitmap files to intermediate assembly to bitmap objects
BMP_SOURCES = $(wildcard $(SOURCE_DIR)/images/*.bmp)
BMP_OBJECTS = $(patsubst $(SOURCE_DIR)/images/%.bmp, $(BUILD_DIR)/%.o, $(BMP_SOURCES))
$(BUILD_DIR)/%.o: %.bmp
	@$(ECHO) == Converting bitmap $< to $@
	@$(SPRPCK) $(SPRFLAGS) $< $@ > /dev/null
	@$(ECHO) .global _$* > $(BUILD_DIR)/$*.s
	@$(ECHO) .segment \"RODATA\" >> $(BUILD_DIR)/$*.s
	@$(ECHO) _$*: .incbin \"$*.spr\" >> $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@



# force font to be multi-sprite
$(BUILD_DIR)/img_ag_font5x5.o : img_ag_font5x5.bmp
	@$(ECHO) == Converting font $< to $@
	@$(SPRPCK) $(SPRFLAGS) -u -s1 -S005005 -r065001 $< $@ > /dev/null
	@$(SPRHLPR) "$@"
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/img_ag_font5x5.s -o $@

# force sprites to use different bits per colour
# 4 bits per pixel for img_player
#$(BUILD_DIR)/img_player.o : SPRFLAGS=-t6 -p2 -s4 -S012016 -r008001 

#$(BUILD_DIR)/img_player.o : img_player.bmp
#	@$(ECHO) == Converting player $< to $@
#	@$(SPRPCK) $(SPRFLAGS)  -s4 -t6 -p2  -S012016 -r008001  $< $@ > /dev/null
#	@$(SPRHLPR) "$@"
#	@$(AS) $(ASFLAGS) $(BUILD_DIR)/img_player.s -o $@





# == Link all objects to final binary - LNX
$(BIN_LNX): $(BMP_OBJECTS) $(ASM_OBJECTS) $(C_OBJECTS)
	@$(ECHO) == Linking binary executable to $@
	@$(CL) $(LDFLAGS_LNX) $^ -o $@

# == Link all objects to final binary - O
$(BIN_BLL): $(BMP_OBJECTS) $(ASM_SOURCES_NOHDR) $(C_OBJECTS)
	@$(ECHO) == Linking binary executable to $@
	@$(CL) $(LDFLAGS_BLL) $^ -o $@

mkbuild:
	@mkdir -p build
	
all: mkbuild $(BIN_LNX) $(BIN_BLL)

clean:
	@$(ECHO) == Cleaning up
	@$(RM) -rf $(BUILD_DIR) $(BIN_LNX) $(BIN_BLL)
