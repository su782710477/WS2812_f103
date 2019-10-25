#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"
#include "spi.h"
#define WS2812_number 256
#define Front_rest_code 3
#define Behind_rest_code 3

void WS2812_data9X8bit_data(uint8_t number, uint8_t G, uint8_t R, uint8_t B);
void WS2812_send_data();
void WS2812_Init();
#endif