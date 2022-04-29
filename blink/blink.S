#define __SFR_OFFSET 0
  
#include "avr/io.h"

.global start
.global on
.global off
.global isr

start:
 sbi DDRB, 7 ; Set PB7 as output
 ;sbi DDRJ, 1 ; Set PJ1 as output
 ret

on:
 sbi PORTB, 7;
 ret

off: 
 cbi PORTB, 7;
 ret

isr: ;not fully working
 sbis PORTB, 7
 call on;
 
 call off;
 reti
