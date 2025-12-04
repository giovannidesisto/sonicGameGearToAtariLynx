;
; Atari Gamer Common Lynx Library.
; Copyright 2022 Igor Kromin (atarigamer.com)
;
; Licensed under the Apache License, Version 2.0 (the "License"); 
; you may not use this file except in compliance with the License. 
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software 
; distributed under the License is distributed on an "AS IS" BASIS, 
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
; See the License for the specific language governing permissions and 
; limitations under the License.
;

; This is a file which can be customised to create a 64 byte LNX ROM
; with cart name, manufacturer, etc. The header is prepended to the ROM
; data by the CC65 linker.
; Based on https://bitbucket.org/atarilynx/lynx/src/cd1c78cf3a25b8e9cb22c930d6204fbe8c6bf3c6/tools/cc65/libsrc/lynx/exehdr.s?at=master&fileviewer=file-view-default

.import __BANK0BLOCKSIZE__
.export __EXEHDRUSR__: absolute = 1

.segment "EXEHDRUSR"

; DO NOT CHANGE
.byte	'L','Y','N','X'

; ROM bank sizes, the first is the bank 0 size, the second is the bank 1 size.
; This is typically set in the lynx.cfg file.
.word	__BANK0BLOCKSIZE__
.word	0

; ROM version number.
.word	1

; Cart name - 32 bytes, use spaces to pad out (31 characters + null terminator).
;       |<--     32 byte maximum     -->|
.asciiz	"MY ROM                         "

; Manufacturer - 16 bytes, use spaces to pad out (15 characters + null terminator).
;       |<-- 16b max -->|
.asciiz	"HOMEBREW       "

; Screen rotation/orientation - 0=none, 1=left, 2=right.
.byte	0

; AUDIN used for addressing 0=no, 1=yes.
.byte   0

; EEPROM flags. The value is a bit field split as follows:
;  - Bits 0-2 encode a 3-bit number field for the EEPROM chip with the following values:
;             0: No EEPROM
;             1: 93c46 (1024 bit, 128 byte, most commonly used)
;             2: 93c56
;             3: 93c66
;             4: 93c76
;             5: 93c86
;    EEPROM size (in bits) can be calculated by size=2^(value+9)
;  - Bits 3-5 are set to 0, do not change.
;  - Bit    6 Whether a real EEPROM chip is used or LynxSD functionality is supported:
;             0: Real EEPROM
;             1: LynxSD save file
;    If both real ROM and LynxSD are supported, set to 1.
;  - Bit    7 EEPROM mode (EEPROM word size) values:
;             0: 16-bit (most commonly used)
;             1: 8-bit
.byte   0

; Spare bytes, do not use.
.byte   0,0,0