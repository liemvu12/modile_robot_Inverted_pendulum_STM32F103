#include "sys.h"	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x;
	return 1;
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 
	while((USART3->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���1   
	USART3->DR = (u8) ch;    
	return ch;
}
#endif

u8 Usart3_Receive;

//end
//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ֽ�
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

void uart1_init(u32 pclk2, u32 bound)
{ 
	
	
  GPIO_InitTypeDef GPIO_InitStructure;    //����һ�����ų�ʼ���Ľṹ��
	USART_InitTypeDef USART_InitStructure;   //����һ�����ڳ�ʼ���Ľṹ��
	NVIC_InitTypeDef NVIC_InitStructure;      //����һ��Ƕ���жϿ�������ʼ���Ľṹ��
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}


/**************************************************************************
�������ܣ�����1�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//���յ�����
	{	      
				u8 temp;
				static u8 count,last_data,last_last_data;
				temp=USART1->DR;
				   if(Usart_Flag==0)
						{	
						if(last_data==0xfe&&last_last_data==0xff) 
						Usart_Flag=1,count=0;	
						}
					 if(Usart_Flag==1)
						{	
							Urxbuf[count]=temp;     
							count++;                
							if(count==4)Usart_Flag=0;
						}
						last_last_data=last_data;
						last_data=temp;
   }
return 0;	
}

/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	 	
	
	
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��
	USART_InitTypeDef USART_InitStructure; //����һ�����ڳ�ʼ���Ľṹ��
	NVIC_InitTypeDef NVIC_InitStructure;  //����һ��Ƕ���жϿ�������ʼ���Ľṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ�ӣ�GPIOB������APB2ʱ���£���STM32��ʹ��IO��ǰ��Ҫʹ�ܶ�Ӧʱ��	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3ʱ��
	
	//TXD-RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11; //����11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //�����������ģʽΪ��������ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�����������úõ�GPIO_InitStructure��������ʼ������GPIOB_PIN11
	
	//RXD-TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; //����10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������ģʽΪ�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //��������ٶ�Ϊ50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�����������úõ�GPIO_InitStructure��������ʼ������GPIOB_PIN10
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //������ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //������Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //ʹ���жϿ�����
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = bound;  //����������
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;  //��У��λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8λ����λ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�������ͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��ʹ��Ӳ������
	USART_Init(USART3, &USART_InitStructure);
	
	
	   	   
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 

}


/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{	
		if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)//���յ�����
	{	  
	 static	int uart_receive=0;//����������ر���
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
  	 uart_receive=USART3->DR; 
		Usart3_Receive=uart_receive;
	
		if(uart_receive==0x59)  Flag_Velocity=2;  //���ٵ���Ĭ��ֵ��
		if(uart_receive==0x58)  Flag_Velocity=1;  //���ٵ�
		
		
			if(uart_receive==0x5A)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
			else if(uart_receive==0x41)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ǰ
			else if(uart_receive==0x42)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=1;//////////////��
			else if(uart_receive==0x43)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;//////////////��
			else if(uart_receive==0x44)	Flag_Qian=0,Flag_Hou=1,Flag_Left=1,Flag_Right=0;//////////////��
		  else if(uart_receive==0x45)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////��
		  else if(uart_receive==0x46)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=1;//////////////��
			else if(uart_receive==0x47)	Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;//////////////��
			else if(uart_receive==0x48)	Flag_Qian=1,Flag_Hou=0,Flag_Left=1,Flag_Right=0;//////////////��
			else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��

		if(Usart3_Receive==0x7B) Flag_PID=1;   //APP����ָ����ʼλ
		if(Usart3_Receive==0x7D) Flag_PID=2;   //APP����ָ��ֹͣλ

		 if(Flag_PID==1)  //�ɼ�����
		 {
			Receive[i]=Usart3_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //��������
		 {
	
			     if(Receive[3]==0x50) 	 PID_Send=1;
				//	 else  if(Receive[3]==0x57) 	 Flash_Send=1;
					 else  if(Receive[1]!=0x23) 
					 {								
						for(j=i;j>=4;j--)
						{
						  Data+=(Receive[j-1]-48)*pow(10,i-j);
						}
						switch(Receive[1])
						 {
							 case 0x30:  Balance_KP=Data;break;
							 case 0x31:  Balance_KD=Data;break;
							 case 0x32:  Velocity_Kp=Data;break;
							 case 0x33:  Velocity_Ki=Data;break;
							 case 0x34:  break; //Ԥ��
							 case 0x35:  break; //Ԥ��
							 case 0x36:  break; //Ԥ��
							 case 0x37:  break; //Ԥ��
							 case 0x38:  break; //Ԥ��
						 }
					 }				 
					 Flag_PID=0;
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//��������
		 } 
	}  											 
} 
