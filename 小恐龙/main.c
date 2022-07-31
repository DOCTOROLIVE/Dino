//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子

//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "stc8.h"
#include "oled.h"

#include <stdlib.h>

extern const unsigned char code COVER[]	;



sbit key1 = P5^4;
sbit key2 = P5^5;

sbit buzz = P3^0;


unsigned char get_key_val()
{
	unsigned char key_num = 0;
	if (key1 == 0)
	{
		key_num = 1;
		/*
		delay1ms(10);
		if (key1 == 0)
		{
			key_num = 1;
		}*/
	}
	else if (key2 == 0)
	{
		key_num = 2;
		/*
		delay1ms(10);
		if (key2 == 0)
		{
			key_num = 2;
		}*/
	}	
	return key_num;
}

	  




void TM1_Isr() interrupt 3 
{ 
   buzz = ~buzz;
}
 int main(void)
 {  	
    unsigned char key_num = 0;
	char sound_flag = 0;
	unsigned char cactus_category = 0;
	unsigned char cactus_length = 8;
	unsigned int score = 0;
	unsigned int highest_score = 0;
	char height = 0;
	int cactus_pos = 128;	
	unsigned char cur_speed = 30;
	char failed = 0;
	char reset = 0;
		


		P3M0 = 0x00;
        P3M1 = 0x00;
		P5M0 = 0x00;
        P5M1 = 0x00;


		OLED_Init();  //初始化OLED 


	    TMOD = 0x00; 
        TL1  = 0xAE; 
        TH1  = 0xFB; 
		TF1 = 0;		//清除TF1标志
        TR1  = 1; //启动定时器
        ET1  = 0; //使能定时器中断
        EA = 1;

		
		OLED_DrawBMP(0,0,128,8,COVER); 
		while(get_key_val()==0);
		delay_ms(100);
		OLED_Clear();
	while(1) 
	{	
		if (failed == 1)
		{
			draw_restart();
			if(sound_flag == 1)
			{
				ET1 = 1;
				delay_ms(10);
				ET1 = 0;			
				delay_ms(10);
				ET1 = 1;
				delay_ms(10);
				ET1 = 0;			
				delay_ms(10);				
				sound_flag = 0;
			}
			
			key_num = get_key_val();
			if (key_num == 2)
			{
				if(score > highest_score) highest_score = score;
				score = 0;
				failed = 0;
				height = 0;
				reset = 1;
				oled_draw_dino_jump(reset);
				oled_draw_cactus_random(cactus_category, reset);
				fill_picture(0x00);
			}
			continue;
		}		
		

		score ++;
		if (height == 0) key_num = get_key_val();
		
		oled_draw_ground();
		oled_draw_cloud();
		//oled_draw_dino();
		//oled_draw_dino_jump(0);
		//oled_draw_cactus();
		
		if (height>0 || key_num > 0) height = oled_draw_dino_jump(reset);
		else oled_draw_dino();
		
		cactus_pos = oled_draw_cactus_random(cactus_category, reset);
		if(cactus_category == 0) cactus_length = 8;
		else if(cactus_category == 1) cactus_length = 16;
		else cactus_length = 24;
		
		if (cactus_pos + cactus_length < 0) 
		{
		  cactus_category = rand()%4;	//4
			oled_draw_cactus_random(cactus_category, 1);
		}		
		
		if ((height < 16) && ( (cactus_pos>=16 && cactus_pos <=32) || (cactus_pos + cactus_length>=16 && cactus_pos + cactus_length <=32)))
		{
			failed = 1;
			sound_flag = 1;
		}		
		
		OLED_ShowString(35, 0, "HI:", 12);
		OLED_ShowNum(58, 0, highest_score, 5, 12);
		OLED_ShowNum(98, 0, score, 5, 12);
		if (score % 100 == 0) ET1 = 1;
		else{ 
			ET1 = 0;
		}
	
		reset = 0;
		
		cur_speed = score/20;
		if (cur_speed > 49) cur_speed = 49;
		delay_ms(50 - cur_speed);
		//delay_ms(100);    
		
	}	  
	

}	  
	

	