/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "jpeg.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* GPIO PINOUT
 * LCD VCC    -----> PB1
 * LCD VCC2    -----> PB2
 * LCD RESET  -----> PA6 HIGH
 * BOARD LED2 -----> PB4
 * BOARD LED1 -----> PA15
 */

void LCD_SPI_Transmit(uint8_t *data,int length){
    HAL_SPI_Transmit(&hspi4,data,length,1000);
}

void LCD_WriteReg(uint8_t reg){
    uint8_t regStartByte=0b01110100;  //t
    uint8_t dataBuffer[2];
    dataBuffer[0]=regStartByte;
    dataBuffer[1]=reg;
    LCD_SPI_Transmit(dataBuffer,2);
}
void LCD_WriteDat(uint8_t *data,int len){
    uint8_t datStartByte=0b01110110;  //v
    uint8_t dataBuffer[len+1];
    dataBuffer[0]=datStartByte;
    for(int index=0;index<len;index++){
        dataBuffer[index+1]=data[index];
    }
    LCD_SPI_Transmit(dataBuffer,len+1);
}

void LCD_Power_On_Set(){


    // power on set 1
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);  //VCC ON
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);  //VCC ON

    // power on set 2
    HAL_Delay(10);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);  //RESET start
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);  //RESET end

    // power on set 3
    HAL_Delay(10);
}

void LCD_Display_Initial_Set(){

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator

    uint8_t data[9]={};
    // display initial set 1-2
    LCD_WriteReg(0xc0);
    data[0]=0x01; data[1]=0x18;
    LCD_WriteDat(data,2);
    // display initial set 3
    HAL_Delay(10);
    // display initial set 4
    LCD_WriteReg(0x20);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display initial set 5
    LCD_WriteReg(0x36);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display initial set 6
    LCD_WriteReg(0x3a);
    data[0]=0x70;
    LCD_WriteDat(data,1);
    // display initial set 7-9
    LCD_WriteReg(0xb1);
    data[0]=0x12; data[1]=0x1f; data[2]=0x1c;
    LCD_WriteDat(data,3);
    // display initial set 10-11
    LCD_WriteReg(0xb2);
    data[0]=0x20; data[1]=0xc8;
    LCD_WriteDat(data,2);
    // display initial set 12
    LCD_WriteReg(0xb3);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display initial set 13
    LCD_WriteReg(0xb4);
    data[0]=0x04;
    LCD_WriteDat(data,1);
    // display initial set 14-18
    LCD_WriteReg(0xb5);
    data[0]=0x12; data[1]=0x0f; data[2]=0x0f; data[3]=0x00; data[4]=0x00;
    LCD_WriteDat(data,5);
    // display initial set 19-24
    LCD_WriteReg(0xb6);
    data[0]=0x03; data[1]=0x18; data[2]=0x02; data[3]=0x40; data[4]=0x10; data[5]=0x33;
    LCD_WriteDat(data,6);
    // display initial set 25
    HAL_Delay(10);
    // display initial set 26-30
    LCD_WriteReg(0xc3);
    data[0]=0x07; data[1]=0x05; data[2]=0x04; data[3]=0x04; data[4]=0x03;
    LCD_WriteDat(data,5);
    // display initial set 31-36
    LCD_WriteReg(0xc4);
    data[0]=0x12; data[1]=0x34; data[2]=0x13; data[3]=0x13; data[4]=0x00; data[5]=0x0c;
    LCD_WriteDat(data,6);
    // display initial set 37
    HAL_Delay(10);
    // display initial set 38
    LCD_WriteReg(0xc5);
    data[0]=0x76;
    LCD_WriteDat(data,1);
    // display initial set 39
    HAL_Delay(10);
    // display initial set 40-42
    LCD_WriteReg(0xc6);
    data[0]=0x23; data[1]=0x50; data[2]=0x00;
    LCD_WriteDat(data,3);
    // display initial set 43
    HAL_Delay(10);
    // display initial set 44-45
    LCD_WriteReg(0xc7);
    data[0]=0x00; data[1]=0xff;
    LCD_WriteDat(data,2);
    // display initial set 53-61
    LCD_WriteReg(0xd0);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x02; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 62-70
    LCD_WriteReg(0xd2);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x02; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 71-79
    LCD_WriteReg(0xd4);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x02; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 80-89
    LCD_WriteReg(0xd1);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x00; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 90-98
    LCD_WriteReg(0xd3);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x00; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 99-107
    LCD_WriteReg(0xd5);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x00; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator
}

void LCD_Sleep_Out_N_Display_On_Set(){

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator

    uint8_t data[9]={};
    // sleep out & display on set 1
    LCD_WriteReg(0x11);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // sleep out & display on set 2
    HAL_Delay(100);
    // sleep out & display on set 3
    LCD_WriteReg(0x29);
    data[0]=0x00;
    LCD_WriteDat(data,1);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator

}
void LCD_Display_Off_N_Sleep_In_Set(){

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator

    uint8_t data[9]={};
    // display off & sleep in set 1
    LCD_WriteReg(0x10);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display off & sleep in set 2
    LCD_WriteReg(0x28);
    data[0]=0x00;
    LCD_WriteDat(data,1);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator

}


#define PI 3.14159265

#define S(o, n) r[t[(int)(h[0]) / 60 * 3 + o] + o - 2] = (n + h[2] - c / 2) * 255;
void C(float h[3], int r[3]) {
    float g = 2 * h[2] - 1, c = (g < 0 ? 1 + g : 1 - g) * h[1], a = (int)(h[0]) % 120 / 60.f - 1;
    int t[] = { 2, 2, 2, 3, 1, 2, 3, 3, 0, 4, 2, 0, 4, 1, 1, 2, 3, 1 };
    S(0, c)
    S(1, c * (a < 0 ? 1 + a : 1 - a))
    S(2, 0)
}

uint32_t huecolor(float hue) {
    int normalized = (int)(hue /360.0 * 256 * 6);

    //find the region for this position
    int region = normalized / 256;

    //find the distance to the start of the closest region
    int x = normalized % 256;

    uint8_t r = 0, g = 0, b = 0;
    switch (region)
    {
        case 0: r = 255; g = 0;   b = 0;   g += x; break;
        case 1: r = 255; g = 255; b = 0;   r -= x; break;
        case 2: r = 0;   g = 255; b = 0;   b += x; break;
        case 3: r = 0;   g = 255; b = 255; g -= x; break;
        case 4: r = 0;   g = 0;   b = 255; r += x; break;
        case 5: r = 255; g = 0;   b = 255; b -= x; break;
    }
    return r + (g << 8) + (b << 16);
}
void pset32to16(int x,int y,uint32_t data){
    int r=data>>19&0b11111;
    int g=data>>10&0b111111;
    int b=data>>3&0b11111;
    int color=0;
    color|=r<<11|g<<5|b;
    displayData[x + y * 480] = color;
}

uint16_t huecolor16(float hue) {
    float r = 0, g = 0, b = 0;
    uint16_t color = 0;
    int rgb[3] = {};
    float hsl[3] = { hue, 1, .6 };
    C(hsl, rgb);
    r = rgb[0] * 0b11111 / 255;
    g = rgb[1] * 0b111111 / 255;
    b = rgb[2] * 0b11111 / 255;
    color |= (((uint16_t)(r)&0b11111) << 11);
    color |= (((uint16_t)(g)&0b111111) << 5);
    color |= ((uint16_t)(b)&0b11111);
    return color;
}


void pset24(int x,int y,uint32_t data){
    if(y>=720)return;
    displayData[x*3 + y*3 * 480] = (data&0xff); //b
    displayData[x*3+1 + y*3 * 480] = (data&0xff00)>>8; //g
    displayData[x*3+2+ y*3 * 480] = (data&0xff0000)>>16; //r
}

void pset32(int x,int y,uint32_t data){
    displayData[x + y * 480] = data;
}

volatile uint32_t color=0;
float hue=0;
int r=0;
int centerX=480/2,centerY=720/2;
const uint8_t font8x8[13][8] = {
        {0x3c, 0x42, 0x46, 0x4a, 0x52, 0x62, 0x3c, 0x00},  // 0030 (zero)
        {0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7c, 0x00},  // 0031 (one)
        {0x3c, 0x42, 0x02, 0x0c, 0x30, 0x42, 0x7e, 0x00},  // 0032 (two)
        {0x3c, 0x42, 0x02, 0x1c, 0x02, 0x42, 0x3c, 0x00},  // 0033 (three)
        {0x08, 0x18, 0x28, 0x48, 0xfe, 0x08, 0x1c, 0x00},  // 0034 (four)
        {0x7e, 0x40, 0x7c, 0x02, 0x02, 0x42, 0x3c, 0x00},  // 0035 (five)
        {0x1c, 0x20, 0x40, 0x7c, 0x42, 0x42, 0x3c, 0x00},  // 0036 (six)
        {0x7e, 0x42, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00},  // 0037 (seven)
        {0x3c, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x3c, 0x00},  // 0038 (eight)
        {0x3c, 0x42, 0x42, 0x3e, 0x02, 0x04, 0x38, 0x00},  // 0039 (nine)
        {0x0c, 0x12, 0x10, 0x38, 0x10, 0x10, 0x38, 0x00},  // 0066 (f)
        {0x00, 0x00, 0x6c, 0x32, 0x32, 0x2c, 0x20, 0x70},  // 0070 (p)
        {0x00, 0x00, 0x3e, 0x40, 0x3c, 0x02, 0x7c, 0x00},  // 0073 (s)
};
void printChar(int index,int cx,int cy){
    uint8_t* data=font8x8[index];
    for(int y=0;y<8;y++){
        int line=data[y];
        for(int x=0;x<8;x++){
            for(int r=0;r<2;r++){
                for( int s=0;s<2;s++){
                    if((line>>(7-x))&1==1){
                        pset24(cx+(7-y)*2+r,cy+x*2+s,0xffffff);
                    }else{
                        pset24(cx+(7-y)*2+r,cy+x*2+s,0);
                    }
                }
            }
        }
    }
}

void printFPS(int fps){
    printChar(fps/100%10,0,0);
    printChar(fps/10%10,0,15);
    printChar(fps%10,0,15*2);
    printChar(10,0,15*3);
    printChar(11,0,15*4);
    printChar(12,0,15*5);
}
int time=0;
int fps=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
    printFPS(fps);
    fps=0;
}
//uint32_t hueLUT[720]={};
float sineLUT[360]={};
float cosineLUT[360]={};
void draw() {
//    hue+=2;
//    if(hue>359)hue=0;
//    color=hueLUT[(int)hue/2];
//    for(int i=0;i<800;i++){
////        hue+=2;
////        if(hue>359)hue=0;
//        for(int j=16;j<480;j++){
//            pset16(j,i,color);
//        }
//    }

    hue += 1;
    if(hue>359)hue=0;
    for (int i = 0; i < 720; i++) {
        hue+=0.5;
        if(hue>359)hue=0;
        color=huecolor(hue);
        for(int r=100;r<200;r+=4) {
            int x1 = cosineLUT[i/2] * r + centerX;
            int y1 = sineLUT[i/2] * r + centerY;
            for(int x=-2;x<3;x++){
                for(int y=-2;y<3;y++) {
                    pset24(x1+x, y1+y, color);
                }
            }
        }
    }
    fps+=1;
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    for(int i=0;i<360;i++){
        //hueLUT[i]= huecolor((float)i);
        sineLUT[i]=sin(i*PI/180);
        cosineLUT[i]=cos(i*PI/180);
    }
  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LTDC_Init();
  MX_SPI4_Init();
  MX_TIM2_Init();
  MX_JPEG_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim2);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);  //LED VCC

    HAL_Delay(100);
    LCD_Power_On_Set();
    //HAL_Delay(100);
    LCD_Display_Initial_Set();
    //HAL_Delay(100);
    LCD_Sleep_Out_N_Display_On_Set();


    MX_LTDC_Init();
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);  //CPLD VCC ON


//    for(int i=150;i<690;i++){
//        color=huecolor(hue);
//        hue+=0.5;
//        if(hue>=359)hue=0;
//        for(int j=0;j<370;j++){
//            displayData[(i)*370+j]=color;
//        }
//    }
    for(int i=0;i<480;i++){
        for(int j=0;j<800;j++){
            pset24(i,j,0xffffff);
        }
    }
    for(int h=0;h<lgHeight;h++){
        for(int w=0;w<lgWidth;w++) {
//            displayData[w + h * 370] = lg[w + h *lgWidth];
            pset24(w,h+(720-lgHeight),lg[w + h *lgWidth]);
            //displayData[w +(370-lgWidth)+ h * 370] = lg[w + h *lgWidth];
        }
    }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    while (1)
    {
        draw();

//      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator
//      HAL_Delay(500);
//      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator
//      HAL_Delay(500);

//        r+=2;
//        if(r>690-150)r=0;
//        for(int h=0;h<lgHeight;h++){
//            for(int w=0;w<lgWidth;w++) {
//                pset(w,h+r,lg[w + h *lgWidth]);
//                //displayData[w+ (h+r) * 370] = lg[w + h *lgWidth];
//            }
//        }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 70;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);  //LED Indicator
  __disable_irq();
  while (1)
  {

  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
