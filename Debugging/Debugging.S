#define __SFR_OFFSET 0
  
#include "avr/io.h"

.global start
.global on
.global off
.global isr

start:

 
 sbi DDRB, 7 ; Set PB7, D13, as output
 ret

on:
 sbi PORTB, 7;
 ret

off: 
 cbi PORTB, 7;
 ret
