#ifndef __CAR_OPS_H
#define __CAR_OPS_H 	

#include "key.h"
#include "paj7620u2.h"

u8 get_key_usart_code(key_e key, char menu, bool long_pressed_status, bool backup_ops);
u8 get_gesture_usart_code(ges_e ops);

#endif
