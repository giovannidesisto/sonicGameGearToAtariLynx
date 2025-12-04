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
; Code based on https://forums.nesdev.com/viewtopic.php?f=2&t=14499&start=30#p175256

.export _agRand8
.export _agInitRand8

.zeropage
        rand8:    .res 4
        randMask: .res 1

.code

_agRand8:
        sta     randMask
        clc
        lda     rand8+0
        adc     rand8+1
        sta     rand8+1
        adc     rand8+2
        sta     rand8+2
        adc     rand8+3
        sta     rand8+3
        clc
        lda     rand8+0
        adc     #$27
        sta     rand8+0
        lda     rand8+1
        adc     #$59
        sta     rand8+1
        lda     rand8+2
        adc     #$41
        sta     rand8+2
        lda     rand8+3
        adc     #$31
        sta     rand8+3
        and     randMask
        ldx     #0
        rts

_agInitRand8:
        sta     rand8+0
        stx     rand8+1
        lda     #0
        sta     rand8+2
        sta     rand8+3
        rts