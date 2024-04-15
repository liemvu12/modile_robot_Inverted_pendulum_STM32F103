#include "sys.h"

// Wait for Interrupt
void WFI_SET(void)
{
    __ASM volatile("wfi");		  
}

// Disable Interrupt
void INTX_DISABLE(void)
{		  
    __ASM volatile("cpsid i");
}

// Enable Interrupt
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");		  
}

// Set Main Stack Pointer (MSP)
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			// Set Main Stack value
    BX r14
}

// Set NVIC Vector Table
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
{ 	   	 
    SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80);
}

// Configure NVIC Priority Group
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
    u32 temp, temp1;	  
    temp1 = (~NVIC_Group) & 0x07;
    temp1 <<= 8;
    temp = SCB->AIRCR;  
    temp &= 0x0000F8FF; 
    temp |= 0x05FA0000; 
    temp |= temp1;	   
    SCB->AIRCR = temp;   	  				   
}

// Initialize NVIC
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group)	 
{ 
    u32 temp;	
    MY_NVIC_PriorityGroupConfig(NVIC_Group);
    temp = NVIC_PreemptionPriority << (4 - NVIC_Group);	  
    temp |= NVIC_SubPriority & (0x0f >> NVIC_Group);
    temp &= 0xf;  
    NVIC->ISER[NVIC_Channel / 32] |= (1 << (NVIC_Channel % 32));
    NVIC->IP[NVIC_Channel] |= temp << 4;  	    	  				   
} 
    
// Configure External Interrupt
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM) 
{
    u8 EXTADDR, EXTOFFSET;
    EXTADDR = BITx / 4;
    EXTOFFSET = (BITx % 4) * 4; 
    RCC->APB2ENR |= 0x01;		 
    AFIO->EXTICR[EXTADDR] &= ~(0x000F << EXTOFFSET);
    AFIO->EXTICR[EXTADDR] |= GPIOx << EXTOFFSET;
    EXTI->IMR |= 1 << BITx;
    if(TRIM & 0x01) EXTI->FTSR |= 1 << BITx;
    if(TRIM & 0x02) EXTI->RTSR |= 1 << BITx;
} 	  

// Reset RCC
void MYRCC_DeInit(void)
{	
    RCC->APB1RSTR = 0x00000000;		 
    RCC->APB2RSTR = 0x00000000; 
    RCC->AHBENR = 0x00000014;  
    RCC->APB2ENR = 0x00000000; 		   
    RCC->APB1ENR = 0x00000000;   
    RCC->CR |= 0x00000001;     														 
    RCC->CFGR &= 0xF8FF0000;   			 
    RCC->CR &= 0xFEF6FFFF;     
    RCC->CR &= 0xFFFBFFFF;       	  
    RCC->CFGR &= 0xFF80FFFF;    
    RCC->CIR = 0x00000000;     	 
	
#ifdef  VECT_TAB_RAM
    MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
    MY_NVIC_SetVectorTable(0x08000000, 0x0);
#endif
}
  
// Enter Standby Mode
void Sys_Standby(void)
{
    SCB->SCR |= 1 << 2;  
    RCC->APB1ENR |= 1 << 28;     	    
    PWR->CSR |= 1 << 8;          
    PWR->CR |= 1 << 2;           
    PWR->CR |= 1 << 1;           	  
    WFI_SET();				 		 
}	     
   
// System Software Reset
void Sys_Soft_Reset(void)
{   
    SCB->AIRCR = 0x05FA0000 | (u32)0x04;	  
} 		 
   
// Set JTAG Mode
void JTAG_Set(u8 mode)
{
    u32 temp;
    temp = mode;
    temp <<= 25;
    RCC->APB2ENR |= 1 << 0;     	   
    AFIO->MAPR &= 0xF8FFFFFF; 
    AFIO->MAPR |= temp;      
}

// Initialize System Clock
void Stm32_Clock_Init(u8 PLL)
{
    unsigned char temp = 0;   
    MYRCC_DeInit();		  
    RCC->CR |= 0x00010000;  
    while(!(RCC->CR >> 17));
    RCC->CFGR = 0X00000400; 
    PLL -= 2;
    RCC->CFGR |= PLL << 18;  
    RCC->CFGR |= 1 << 16;	  
    FLASH->ACR |= 0x32;	  

    RCC->CR |= 0x01000000;  
    while(!(RCC->CR >> 25))
    RCC->CFGR |= 0x00000002;
    while(temp != 0x02)     
    {   
        temp = RCC->CFGR >> 2;
        temp &= 0x03;
    }    
}	
