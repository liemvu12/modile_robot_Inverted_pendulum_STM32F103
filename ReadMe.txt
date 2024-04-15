1.
// Hàm khởi tạo cấu hình clock cho vi điều khiển STM32
void Stm32_Clock_Init(u8 PLL) {
    unsigned char temp = 0;
    MYRCC_DeInit();	// Đặt lại tất cả cấu hình clock
    RCC->CR |= 0x00010000;	// Kích hoạt bộ dao động ngoại (HSE)
    while(!(RCC->CR >> 17)); 	// Chờ cho đến khi HSE sẵn sàng
    RCC->CFGR = 0X00000400;	// Cấu hình chia tần số cho APB1, APB2, và AHB
    PLL -= 2;	    		// Điều chỉnh giá trị của PLL
    RCC->CFGR |= PLL << 18;
    RCC->CFGR |= 1 << 16;	// Bật nguồn PLL
    FLASH->ACR |= 0x32;		// Cấu hình bộ nhớ Flash cho tốc độ tối ưu
    RCC->CR |= 0x01000000;	// Kích hoạt PLL
    while(!(RCC->CR >> 25)); 	// Chờ cho đến khi PLL sẵn sàng
    RCC->CFGR |= 0x00000002;	// Đặt PLL làm nguồn clock cho hệ thống
    while(temp != 0x02) {   	// Chờ cho đến khi PLL làm nguồn clock cho hệ thống sẵn sàng
        temp = RCC->CFGR >> 2;
        temp &= 0x03;
    }
}

Hàm Stm32_Clock_Init() có vai trò thiết lập cấu hình clock cho vi điều khiển STM32. 

- HSE (External High-Speed Clock) là một nguồn clock được cung cấp từ bên ngoài vi điều khiển. Vai trò của HSE là cung cấp một tín hiệu clock ổn định và có tần số cao hơn so với các nguồn clock nội bộ khác.
  
- PLL (Phase-Locked Loop) là một khối chức năng trong vi điều khiển, nhận tín hiệu từ nguồn clock cơ bản (như HSE) và tăng tần số của nó lên một bội số cố định để tạo ra một tín hiệu clock với tần số cao hơn. Vai trò của PLL là cải thiện hiệu suất xử lý và độ chính xác của hệ thống bằng cách cung cấp một tín hiệu clock với tần số cao hơn.

2.
// Hàm cấu hình chế độ JTAG trên vi điều khiển STM32
void JTAG_Set(u8 mode) {
    u32 temp; 		// Biến tạm thời lưu giá trị mode
    temp = mode; 	// Gán giá trị 0x02 cho biến tạm thời temp
    temp <<= 25; 	// Dịch trái giá trị của biến temp đi 25 bit
    RCC->APB2ENR |= 1 << 0;// Bật clock cho bộ điều khiển AFIO
    AFIO->MAPR &= 0xF8FFFFFF;// Xóa các bit ở vị trí [26:24] trong thanh ghi MAPR của bộ điều khiển AFIO
    AFIO->MAPR |= temp; // Thiết lập chế độ JTAG bằng cách ghi giá trị của biến temp vào các bit [26:24] trong thanh ghi MAPR của bộ điều khiển AFIO
}
	Chế độ JTAG (Joint Test Action Group) là một trong những chế độ debug có sẵn trên vi điều khiển STM32F10x. Chế độ này cho phép sử dụng giao diện JTAG để giao tiếp với vi điều khiển và thực hiện các hoạt động như nạp chương trình, đọc và ghi dữ liệu từ bộ nhớ, và kiểm tra các tín hiệu trên vi điều khiển. Đối với vi điều khiển STM32F10x, các chân GPIO có thể được cấu hình để hoạt động trong chế độ JTAG.
	Đặt mode = 0x02 khi gọi hàm JTAG_Set, thì JTAG sẽ chạy ở chế độ SW-DP (Serial Wire Debug Port). Điều này có nghĩa là các chức năng debug trên vi điều khiển STM32F10x sẽ sử dụng giao diện Serial Wire để giao tiếp thay vì giao diện JTAG truyền thống. Chế độ này thường được sử dụng khi không có đủ chân GPIO để cấu hình giao diện JTAG hoặc khi cần tiết kiệm chân GPIO cho các mục đích khác.