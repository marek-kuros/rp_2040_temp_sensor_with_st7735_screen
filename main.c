#include <stdio.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "lib/GUI/GUI_Paint.h"
#include "lib/LCD/LCD_1in8.h"

int LCD_1in8_GUI_template(void);
char Measure_temp(void);

void LCD_init(unsigned char Position, UWORD Colour){
    DEV_Module_Init();          //initialise SPI and other pins to drive screen
    LCD_1IN8_Init(Position);    //must have
    LCD_1IN8_Clear(Colour);     //must have
}

int main() {
    LCD_init(HORIZONTAL, MAGENTA);
    while(true) {
        LCD_1in8_GUI_template();
    }
    return 0;
}

int LCD_1in8_GUI_template(void){

    //initialise screen
    // DEV_Module_Init();          //initialise SPI and other pins to drive screen
    // LCD_1IN8_Init(Position);    //must have
    // LCD_1IN8_Clear(Colour);     //must have   

    // allocating memory for picture (kinda strange but this is how it works)
    UDOUBLE Imagesize = LCD_1IN8_HEIGHT*LCD_1IN8_WIDTH*2;
    UWORD *BlackImage;
    BlackImage = (UWORD *)malloc(Imagesize);

    //setting picture parameters
    Paint_NewImage((UBYTE *)BlackImage,LCD_1IN8.WIDTH,LCD_1IN8.HEIGHT, 0, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(WHITE);

    // make some drawing
///////////////////////////////////////////////////////////////////////////////////////////////////

    Paint_DrawString_EN(1, 1, "Pico-LCD-1.8_with_temp_sensor", &Font12, WHITE, BLUE);
    Paint_DrawNum(50, 53, Measure_temp(), &Font24, 0, BLUE, WHITE);

///////////////////////////////////////////////////////////////////////////////////////////////////

    //printing imagine
  	LCD_1IN8_Display(BlackImage);
    DEV_Delay_ms(5000);    

    /* Module Exit */
    //release memmory
    free(BlackImage);
    BlackImage = NULL;
    
    DEV_Module_Exit();
    return 0;
}

char Measure_temp(void){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    char c_tempC = (char)tempC;

    return c_tempC;
}
