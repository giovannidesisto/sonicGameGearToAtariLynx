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

; Store commonly used runtime variables in the zeropage memory segment
; for faster access.
; These are used internally by ag* functions and should not be used by
; your code.


.segment "ZEROPAGE" : zeropage

; --- Font variables -----------------------------------------------------------

__agFontStrIdx:                   .byte 0
.exportzp __agFontStrIdx

__agFontStrLen:                   .byte 0
.exportzp __agFontStrLen

__agFontStrChar:                  .byte 0
.exportzp __agFontStrChar

; --- Temporary variables ------------------------------------------------------

__agTmpIdx8_1:                   .byte 0
.exportzp __agTmpIdx8_1

__agTmpIdx8_2:                   .byte 0
.exportzp __agTmpIdx8_2

__agTmpAddr_1:                   .addr 0
.exportzp __agTmpAddr_1

__agTmpAddr_2:                   .addr 0
.exportzp __agTmpAddr_2


