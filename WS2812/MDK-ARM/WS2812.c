#include "WS2812.h"


uint32_t WS2812_color[3] = {0x00};//8bit color mapping 24bit color
																	//1 = 110;	0 = 100;
uint8_t WS2812_data[WS2812_number + Front_rest_code + Behind_rest_code][9] = {0xff}; //array initialization 0x00
																										//2 dimensional array; 3 row is rest code, 50us low level 
																										//9 column is 72bit(24bit color mapping)
uint8_t WS2812_data_1D[(WS2812_number + Front_rest_code + Behind_rest_code) * 9] = {0xff};


uint32_t WS2812_ledcolor_2_SPIbit(uint8_t color){
	uint32_t data = 0;
	for(int i = 0; i < 8; i++){
		if((color >> i) & 0x01){
			data = data | (0x06 << (3 * i));
		}
		else{
			data = data | (0x04 << (3 * i));
		}
	}
	return data;
}

void WS2812_Init(){
	WS2812_color[0] = WS2812_ledcolor_2_SPIbit(0);
	WS2812_color[1] = WS2812_ledcolor_2_SPIbit(0);
	WS2812_color[2] = WS2812_ledcolor_2_SPIbit(0);
	printf("\r\nws2812 init");
	uint8_t WS2812_SPIbit24X3 = 0;
//	printf("\r\nsend data");
	for(int r = 0; r < WS2812_number + Front_rest_code + Behind_rest_code; r++){
		for(int c = 0; c < 9; c++) 
		{
			if(((c % 3) == 0)&&(c != 0)){
				WS2812_SPIbit24X3++;
			}
			WS2812_data[r][c] = (WS2812_color[c % 3] >> ((2 -(c % 3)) * 8));
//			printf("--->0x%02x", WS2812_data[r][c]);
		}
	}
}
/**
*color format	8bit -> 24bit
*						R:0x** -> 0x00(abandon) 0x** 0x** 0x**
*						G:0x** -> 0x00(abandon) 0x** 0x** 0x**
*						B:0x** -> 0x00(abandon) 0x** 0x** 0x**
*								total:72bit(9byte)
*
*/

void WS2812_data9X8bit_data(uint8_t number, uint8_t G, uint8_t R, uint8_t B){
	WS2812_color[0] = WS2812_ledcolor_2_SPIbit(G);
	WS2812_color[1] = WS2812_ledcolor_2_SPIbit(R);
	WS2812_color[2] = WS2812_ledcolor_2_SPIbit(B);
	
	uint8_t WS2812_SPIbit24X3 = 0;
//	printf("\r\nsend data");
	for(int i = 0; i < 9; i++) 
	{
		if(((i % 3) == 0)&&(i != 0)){
			WS2812_SPIbit24X3++;
		}
		WS2812_data[number][i] = (WS2812_color[WS2812_SPIbit24X3] >> ((2 -(i % 3)) * 8));
//		printf("9X8bit->0x%02x", WS2812_data[number][i]);
	}
}

void arry_2Dto1D(){

	for(int i = 0; i < Front_rest_code; i++){	//	row
		for(int j = 0; j<9; j++){		//column
			WS2812_data_1D[i * 9 + j] = 0x00;
		}
	}
	for(int i = Front_rest_code; i<WS2812_number + Front_rest_code; i++){	//	row
		for(int j = 0; j<9; j++){		//column
//			printf("\r\n0x%02x 2Dto1D 0x%02x", WS2812_data[i-Front_rest_code][j], WS2812_data_1D[i * 9 + j]);
			WS2812_data_1D[i * 9 + j] = WS2812_data[i-Front_rest_code][j];
		}
	}
	for(int i = WS2812_number + Front_rest_code; i < WS2812_number + Front_rest_code + Behind_rest_code; i++){	//	row
		for(int j = 0; j<9; j++){		//column
			WS2812_data_1D[i*9 + j] = 0x00;
		}
	}
	
}
void WS2812_send_data(){

	arry_2Dto1D();
//	for(int i = 0; i< 100; i++){
//		printf("--0x%02x", WS2812_data_1D[i]);
//	}
//	printf("\r\nsend data");
//	for(int i = 0; i < (WS2812_number + Front_rest_code + Behind_rest_code) * 9; i++){
//		printf(" %03d->0x%02x", i, WS2812_data_1D[i]);
//		if((i+1) % 9 == 0){
//		printf("\r\n-------------------------------------\r\n");
//		}
//	}
	HAL_SPI_Transmit(&hspi1,WS2812_data_1D, (WS2812_number + Front_rest_code + Behind_rest_code) * 9, 0xffff);
}



