#ifndef __DELAY_H
#define __DELAY_H 			

typedef unsigned char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























