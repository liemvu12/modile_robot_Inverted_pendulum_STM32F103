#include "adc.h"

/**************************************************************************
@brief Khởi tạo ADC để đo điện áp
@param Không có
@return Không có
@Tác giả Không rõ
**************************************************************************/
void Adc_Init_(void)
{    
  // Khởi tạo các cổng IO
	GPIO_InitTypeDef GPIO_InitStructure; // Khai báo cấu trúc GPIO
	ADC_InitTypeDef ADC_InitStructure;   // Khai báo cấu trúc ADC
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);     // Bật clock cho PORTC và ADC1
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // Thiết lập tỉ lệ chia clock ADC thành 6, tạo clock ADC 12MHz
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1; // Chân 1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; // Thiết lập chế độ chân là analog input
	GPIO_Init(GPIOC, &GPIO_InitStructure); // Khởi tạo chân GPIOC_PIN1 là analog input
	
	ADC_DeInit(ADC1);  // Reset ADC1
	ADC_ClearFlag(ADC1, ADC_Mode_Independent); // Xóa các cờ chế độ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // Thiết lập chế độ ADC là chế độ độc lập
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;     // Vô hiệu hóa chế độ quét
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // Vô hiệu hóa chế độ chuyển đổi liên tục
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Vô hiệu hóa bắt đầu chuyển đổi từ trigger ngoại vi
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // Căn dữ liệu về bên phải
	ADC_InitStructure.ADC_NbrOfChannel = 1;    // Chọn số kênh cần chuyển đổi
	ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Cmd(ADC1, ENABLE);  // Kích hoạt ADC1
	
	
  ADC_ResetCalibration(ADC1);	// Reset hiệu chuẩn của ADC1	 
	while(ADC_GetResetCalibrationStatus(ADC1));	// Chờ cho việc reset hiệu chuẩn hoàn thành	
	ADC_StartCalibration(ADC1);	 // Bắt đầu hiệu chuẩn ADC
	while(ADC_GetCalibrationStatus(ADC1));	 // Chờ cho việc hiệu chuẩn hoàn thành	
}		

/**************************************************************************
@brief Thực hiện chuyển đổi ADC
@param ch Kênh ADC
@return Giá trị ADC đã chuyển đổi
@Tác giả Không rõ
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	if(ch == 11)ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);  // Cấu hình kênh điện áp để đo chính xác, chọn thời gian lấy mẫu
	else if(ch == 5)ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_1Cycles5);  // Cấu hình kênh dòng để đo tốc độ nhanh, chọn thời gian lấy mẫu tối thiểu	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);   // Bắt đầu chuyển đổi       
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));  // Chờ cho việc chuyển đổi hoàn thành	 	   
	return ADC_GetConversionValue(ADC1);		// Trả về giá trị ADC	
}

/**************************************************************************
@brief Lấy điện áp của pin điện
@param Không có
@return Điện áp của pin điện
@Tác giả Không rõ
**************************************************************************/
int Get_battery_volt(void)   
{  
	int Volt; // Điện áp của pin điện
	Volt=Get_Adc(Battery_Ch)*3.3*11*100/4096;	// Tính toán giá trị điện áp sử dụng ADC, chuyển đổi sang mV	
	return Volt;
}

/**************************************************************************
@brief Khởi tạo ADC cho góc
@param Không có
@return Không có
@Tác giả Không rõ
**************************************************************************/
void  Angle_Adc_Init(void)
{    
// Khởi tạo các cổng IO
	GPIO_InitTypeDef GPIO_InitStructure; // Khai báo cấu trúc GPIO
	ADC_InitTypeDef ADC_InitStructure;   // Khai báo cấu trúc ADC
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);     // Bật clock cho PORTA và ADC1
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // Thiết lập tỉ lệ chia clock ADC thành 6, tạo clock ADC 12MHz
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5; // Chân 5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; // Thiết lập chế độ chân là analog input
	GPIO_Init(GPIOA, &GPIO_InitStructure); // Khởi tạo chân GPIOA_PIN5 là analog input
	
	ADC_DeInit(ADC1);  // Reset ADC1
	ADC_ClearFlag(ADC1, ADC_Mode_Independent); // Xóa các cờ chế độ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // Thiết lập chế độ ADC là chế độ độc lập
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;     // Vô hiệu hóa chế độ quét
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // Vô hiệu hóa chế độ chuyển đổi liên tục
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Vô hiệu hóa bắt đầu chuyển đổi từ trigger ngoại vi
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // Căn dữ liệu về bên phải
	ADC_InitStructure.ADC_NbrOfChannel = 1;    // Chọn số kênh cần chuyển đổi
	ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Cmd(ADC1, ENABLE);  // Kích hoạt ADC1
	
	
  ADC_ResetCalibration(ADC1);	// Reset hiệu chuẩn của ADC1	 
	while(ADC_GetResetCalibrationStatus(ADC1));	// Chờ cho việc reset hiệu chuẩn hoàn thành	
	ADC_StartCalibration(ADC1);	 // Bắt đầu hiệu chuẩn ADC
	while(ADC_GetCalibrationStatus(ADC1));	 // Chờ cho việc hiệu chuẩn hoàn thành	
	delay_ms(50);
}		

// Lấy giá trị trung bình của các chuyển đổi ADC cho kênh ch, cho times lần, sau đó trung bình
// ch: Số kênh ADC
// times: Số lần chuyển đổi
// return: Giá trị trung bình của times lần chuyển đổi cho kênh ch
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_us(200);
	}
	return temp_val/times;
}
