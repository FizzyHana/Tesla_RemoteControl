#include "get_usart_code.h"
#include <stdbool.h>

/**
  * @brief  ����жϷ������
  * @param  key : ���µİ�ť������������POWER��CENTRE��
					RIGHT��LEFT��FORWARD��BACK
			menu : ��ǰ�����˵�
			long_pressed_status���Ƿ񳤰�
			backup_ops���Ƿ�Ϊ����ָ����ƴ���������led��ʱ��Ч��
				
  * @retval usart��һλ����ָ��
  */
u8 get_key_usart_code(key_e key, char menu, bool long_pressed_status, bool backup_ops)
{
	switch(key)
	{
		case KEY_POWER:
			return 0x40;
		
		case KEY_CENTRE:
			if(menu == 1)
				return 0x29;
			else
				return 0x20;
			
		case KEY_RIGHT:
			if(menu == 1)
				return 0x04;
			else if(menu == 2 && long_pressed_status == true)
				return 0x31;
			else if(backup_ops == true)
				return 0x22;
			else
				return 0x23;
			
		case KEY_LEFT:
			if(menu == 1)
				return 0x03;
			else if(menu == 2 && long_pressed_status == true)
				return 0x32;
			else if(backup_ops == true)
				return 0x24;
			else
				return 0x25;
			
		case KEY_FORWARD:
			if(menu == 1)
				return 0x01;
			else
				return 0x21;
			
		case KEY_BACK:
			if(menu == 1)
				return 0x02;
			else
				return 0x26;
			
		default:
			return 0;
	}
}

u8 get_gesture_usart_code(ges_e ges)
{
	switch(ges)
	{
		case UP:
			return 0x01;
		
		case DOWN:
			return 0x02;
		
		case LEFT:
			return 0x03;
		
		case RIGHT:
			return 0x04;
		
		case FORWARD:
			return 0x00;
		
		case BACKWARD:
			return 0x20;
		
		case CLOCKWISE:
			return 0x09;
		
		case ANTICLOCKWISE:
			return 0x11;
		
		case WAVE:
			return 0x29;
		
		default:
			return 0;
	}
}
