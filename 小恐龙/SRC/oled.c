//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   P1^0��SCL��
//              D1   P1^1��SDA��
//              RES  ��P12
//              DC   ��P13
//              CS   ��P14               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/��
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  
#include "bmp.h"	 
//#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊ��������ģʽ
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!OLED_SDA)				//�ж��Ƿ���յ�OLED Ӧ���ź�
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊͨ���������ģʽ
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//��������

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  //if(y1%8==0) y=y1/8;      
  //else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//���ƿհ� 
void oled_drawbmp_block_clear(int bx, int by, int clear_size)
{
	unsigned int i;
	OLED_Set_Pos(bx, by);

	for (i = 0; i < clear_size; i++)
	{
		if (bx + i>128) break;
		OLED_WR_Byte(0x00,OLED_DATA);
	}
}

//���Ƶ��� 
void oled_draw_ground()
{
	static unsigned int pos = 0;
	unsigned char speed = 5;
	unsigned int ground_length = sizeof(GROUND);
	unsigned int j = 0;
	unsigned char x;

	
	OLED_Set_Pos(0,7);

	for (x = 0; x < 128; x++)
	{
		OLED_WR_Byte(GROUND[(x+pos)%ground_length],OLED_DATA);
	}

	pos = pos + speed;

}

// ����С����
void oled_draw_dino()
{
	
	unsigned int j=0;
	unsigned char x, y;
    static bit dino_dir=0;
	dino_dir=~dino_dir;

	for(y=0; y<2; y++) 
	{
		OLED_Set_Pos(16,6+y);
	
		for (x = 0; x < 16; x++) 
		{
			j = y*16 + x; 
			OLED_WR_Byte(DINO[dino_dir][j],OLED_DATA);
		}
	
	}
}

//�����ƶ� 

void oled_draw_cloud()
{
	static int pos = 128;
	static char height=0;
	char speed = 3;
	unsigned int i=0;
	int x;
	int start_x = 0;
	int length = sizeof(CLOUD);
	unsigned char byte;

	if (pos + length <= -speed)
	{
		pos = 128;
		height = rand()%3;
	}
		
	if(pos < 0)
	{
		start_x = -pos;
		OLED_Set_Pos(0, 1+height);
	} 
	else
	{
	    start_x = 0;
		OLED_Set_Pos(pos, 1+height);
    }
	
	for (x = start_x; x < length + speed; x++)
	{
		if (pos + x > 127)
		    break;
		if (x < length) 
		   byte = CLOUD[x];
		else 
		   byte = 0x0;		
		OLED_WR_Byte(byte,OLED_DATA);
	}
	
	pos = pos - speed;	
}


// �����������ϰ���
void oled_draw_cactus()
{

	char speed = 5;	
	static int pos = 128;
	int start_x = 0;
	int length = sizeof(CACTUS_2)/2;

	unsigned int j=0;
	unsigned char x, y;
	unsigned char byte;

	if (pos + length <= 0)
	{
	    oled_drawbmp_block_clear(0, 6, speed);
		pos = 128;
	}
		

for(y=0;y<2;y++)
 {
   	if(pos < 0)
	{
		start_x = -pos;
		OLED_Set_Pos(0, 6+y);

    }
	else
	{
	    start_x = 0;
		OLED_Set_Pos(pos, 6+y);
	}
			
	for (x = start_x; x < length; x++)
	{
		if (pos + x > 127)
		    break;
		byte = CACTUS_2[x+length*y];
		OLED_WR_Byte(byte,OLED_DATA);
	}
 }
    oled_drawbmp_block_clear(pos + length, 6, speed); // �����Ӱ
	pos = pos - speed;	
}





// ����������ֵ��������ϰ���

int oled_draw_cactus_random(unsigned char ver, unsigned char reset)
{

	char speed = 5;	
	static int pos = 128;
	int start_x = 0;
	int length = 0;

	unsigned int i=0, j=0;
	unsigned char x, y;
	unsigned char byte;

	if (reset == 1)
	{
		pos = 128;
		oled_drawbmp_block_clear(0, 6, speed);
		return 128;
	}

   if (ver == 0) length = 8; //sizeof(CACTUS_1) / 2;
	else if (ver == 1) length = 16; //sizeof(CACTUS_2) / 2;
	else if (ver == 2 || ver == 3) length = 24; 
	
	

for(y=0;y<2;y++)
 {
   	if(pos < 0)
	{
		start_x = -pos;
	    OLED_Set_Pos(0, 6+y);
    }
	else
	{
	    start_x = 0;
        OLED_Set_Pos(pos, 6+y);
	}
			
	for (x = start_x; x < length ; x++)
	{
		if (pos + x > 127)
		    break;
		j = y*length + x; 
	
	    if (ver == 0) byte = CACTUS_1[j];
		else if (ver == 1) byte = CACTUS_2[j];
		else if(ver == 2) byte = CACTUS_3[j];
		else byte = CACTUS_4[j];		  
	
		OLED_WR_Byte(byte,OLED_DATA);
	}
 }
    oled_drawbmp_block_clear(pos + length, 6, speed);	
	pos = pos - speed;	
	return pos + speed;
}



// ������ԾС����
int oled_draw_dino_jump(char reset)
{
	char speed_arr[] = {1, 1, 3, 3, 4, 4, 5, 6, 7};
	static char speed_idx = sizeof(speed_arr)-1;
	static char height = 0;
	static char dir = 0;
	//char speed = 4; 

	unsigned int i=0, j=0;
	unsigned char x, y;
	char offset = 0;
	unsigned char byte;
	if(reset == 1)
	{
		height = 0;
		dir = 0;
		speed_idx = sizeof(speed_arr)-1;
		return 0;
	}
	if (dir==0)
	{		
		height += speed_arr[speed_idx];
		speed_idx --;
		if (speed_idx<0) speed_idx = 0;
	}
	if (dir==1) 
	{
		height -= speed_arr[speed_idx];
		speed_idx ++;
		if (speed_idx>sizeof(speed_arr)-1) speed_idx = sizeof(speed_arr)-1;
	}
	if(height >= 31)
	{
		dir = 1;
		height = 31;
	}
	if(height <= 0)
	{		
		dir = 0;
		height = 0;
	}
	if(height <= 7) offset = 0;
	else if(height <= 15) offset = 1;
	else if(height <= 23) offset = 2;
	else if(height <= 31) offset = 3;
	else offset = 4;

	for(y=0; y<3; y++) // 4
	{
		OLED_Set_Pos(16, 5- offset + y);
	
		for (x = 0; x < 16; x++) // 32
		{
			j = y*16 + x; // 32
			byte = DINO_JUMP[height%8][j];
			OLED_WR_Byte(byte,OLED_DATA);
		}
	}
	if (dir == 0) oled_drawbmp_block_clear(16, 8- offset, 16);
	if (dir == 1) oled_drawbmp_block_clear(16, 4- offset, 16);
	return height;
}


// ��������
void draw_restart()
{
	unsigned int i=0, j = 0;
	unsigned char x, y;
	unsigned char byte;

	for (y = 2; y < 5; y++)
	{
		OLED_Set_Pos(52, y);

		for (x = 0; x < 24; x++)
		{
			byte = RESTART[j++];
			OLED_WR_Byte(byte,OLED_DATA);
		}
	
	}
	OLED_ShowString(10, 3, "GAME", 16);
	OLED_ShowString(86, 3, "OVER", 16);
}




//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  





























