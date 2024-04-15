#include "usart.h"
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 
	while((USART3->SR&0X40)==0);//Flag_Show!=0  使用串口1   
	USART3->DR = (u8) ch;    
	return ch;
}
#endif

u8 Usart3_Receive;

//end
//////////////////////////////////////////////////////////////////
/**************************实现函数**********************************************
*功    能:		usart1发送一个字节
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction;
	
  GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	USART_InitTypeDef USART_InitStructure; //定义一个串口初始化的结构体
	NVIC_InitTypeDef NVIC_InitStructure;  //定义一个嵌套中断控制器初始化的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟，GPIOA挂载在APB2时钟下，在STM32中使用IO口前都要使能对应时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //引脚9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //引脚输入输出模式为浮空输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure); //根据上面设置好的GPIO_InitStructure参数，初始化引脚GPIOA_PIN9
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; //引脚10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //引脚输入输出模式为复用推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //引脚输出速度为50MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure); //根据上面设置好的GPIO_InitStructure参数，初始化引脚GPIOA_PIN10
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE); //复位串口1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE); //停止复位
	
	USART_InitStructure.USART_BaudRate = mantissa;  //波特率设置
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;  //无校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //开启发送接受模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //不使用硬件流控
	USART_Init(USART1, &USART_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //设置抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //设置响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能中断控制器
	NVIC_Init(&NVIC_InitStructure);
	   	   
	USART_ITConfig(USART1, USART_IT_PE, ENABLE);  //PE中断使能
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //接收缓冲区非空中断使能
	
	USART_Cmd(USART1, ENABLE);  //使能串口1
}


/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//接收到数据
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
函数功能：串口3初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	USART_InitTypeDef USART_InitStructure; //定义一个串口初始化的结构体
	NVIC_InitTypeDef NVIC_InitStructure;  //定义一个嵌套中断控制器初始化的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟，GPIOB挂载在APB2时钟下，在STM32中使用IO口前都要使能对应时钟	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11; //引脚11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //引脚输入输出模式为浮空输入模式
	GPIO_Init(GPIOB, &GPIO_InitStructure); //根据上面设置好的GPIO_InitStructure参数，初始化引脚GPIOB_PIN11
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; //引脚10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //引脚输入输出模式为复用推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //引脚输出速度为50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //根据上面设置好的GPIO_InitStructure参数，初始化引脚GPIOB_PIN10
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE); //复位串口3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE); //停止复位
	
	USART_InitStructure.USART_BaudRate = mantissa;  //波特率设置
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;  //无校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //开启发送接受模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //不使用硬件流控
	USART_Init(USART3, &USART_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //设置抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //设置响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能中断控制器
	NVIC_Init(&NVIC_InitStructure);
	   	   
	USART_ITConfig(USART3, USART_IT_PE, ENABLE);  //PE中断使能
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //接收缓冲区非空中断使能
	
	USART_Cmd(USART3, ENABLE);  //使能串口1
}


/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
		if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)//接收到数据
	{	  
	 static	int uart_receive=0;//蓝牙接收相关变量
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
  	 uart_receive=USART3->DR; 
		Usart3_Receive=uart_receive;
	
		if(uart_receive==0x59)  Flag_Velocity=2;  //低速挡（默认值）
		if(uart_receive==0x58)  Flag_Velocity=1;  //高速档
		
		
			if(uart_receive==0x5A)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
			else if(uart_receive==0x41)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
			else if(uart_receive==0x42)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=1;//////////////后
			else if(uart_receive==0x43)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;//////////////后
			else if(uart_receive==0x44)	Flag_Qian=0,Flag_Hou=1,Flag_Left=1,Flag_Right=0;//////////////后
		  else if(uart_receive==0x45)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
		  else if(uart_receive==0x46)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=1;//////////////后
			else if(uart_receive==0x47)	Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;//////////////后
			else if(uart_receive==0x48)	Flag_Qian=1,Flag_Hou=0,Flag_Left=1,Flag_Right=0;//////////////后
			else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车

		if(Usart3_Receive==0x7B) Flag_PID=1;   //APP参数指令起始位
		if(Usart3_Receive==0x7D) Flag_PID=2;   //APP参数指令停止位

		 if(Flag_PID==1)  //采集数据
		 {
			Receive[i]=Usart3_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //分析数据
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
							 case 0x34:  break; //预留
							 case 0x35:  break; //预留
							 case 0x36:  break; //预留
							 case 0x37:  break; //预留
							 case 0x38:  break; //预留
						 }
					 }				 
					 Flag_PID=0;
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//数组清零
		 } 
	}  											 
} 
