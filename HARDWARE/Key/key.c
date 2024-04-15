#include "key.h"

// H√†m kh·ªüi t·∫°o n√∫t nh·∫•n
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // Khai b√°o c·∫•u tr√∫c ƒë·ªÉ kh·ªüi t·∫°o GPIO

    // B·∫≠t clock cho GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

    // Thi·∫øt l·∫≠p ch√¢n GPIOA_PIN0 l√† ƒë·∫ßu v√†o, ch·∫ø ƒë·ªô k√©o l√™n n·ªôi tr·ªü
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // √Åp d·ª•ng c·∫•u h√¨nh v√†o ch√¢n GPIOA_PIN0
}

u8 click_N_Double (u8 time)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(KEY==0)  Forever_count++;   //≥§∞¥±Í÷æŒªŒ¥÷√1
     else        Forever_count=0;
		if(0==KEY&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2;//À´ª˜÷¥––µƒ÷∏¡Ó
				}
		}
		if(1==KEY)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//µ•ª˜÷¥––µƒ÷∏¡Ó
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}
/**************************************************************************
∫Ø ˝π¶ƒ‹£∫∞¥º¸…®√Ë
»Îø⁄≤Œ ˝£∫Œﬁ
∑µªÿ  ÷µ£∫∞¥º¸◊¥Ã¨ 0£∫Œﬁ∂Ø◊˜ 1£∫µ•ª˜ 
**************************************************************************/
u8 click(void)
{
			static u8 flag_key=1;//∞¥º¸∞¥À…ø™±Í÷æ
			if(flag_key&&KEY==0)
			{
			flag_key=0;
			return 1;	// ∞¥º¸∞¥œ¬
			}
			else if(1==KEY)			flag_key=1;
			return 0;//Œﬁ∞¥º¸∞¥œ¬
}

/**************************************************************************
∫Ø ˝π¶ƒ‹£∫≥§∞¥ºÏ≤‚
»Îø⁄≤Œ ˝£∫Œﬁ
∑µªÿ  ÷µ£∫∞¥º¸◊¥Ã¨ 0£∫Œﬁ∂Ø◊˜ 1£∫≥§∞¥2s
**************************************************************************/
u8 Long_Press(void)
{
			static u16 Long_Press_count,Long_Press;
	    if(Long_Press==0&&KEY==0)  Long_Press_count++;   //≥§∞¥±Í÷æŒªŒ¥÷√1
      else                       Long_Press_count=0; 
		  if(Long_Press_count>200)		
			{
				Long_Press=1;	
				Long_Press_count=0;
				return 1;
			}				
			 if(Long_Press==1)     //≥§∞¥±Í÷æŒª÷√1
			{
				  Long_Press=0;
			}
			return 0;
}
