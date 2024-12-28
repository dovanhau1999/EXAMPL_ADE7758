/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define MAX_BUFFER 20

uint8_t Next_page, Select;

bool Flag_Rx;
bool Flag_display;
char Line1[70];

uint8_t rx_data[MAX_BUFFER], cop_rx_data[MAX_BUFFER];
uint8_t length, count;
int32_t ADC_Current, ADC_Voltage, ADC_Power, ADC_Reactive, ADC_Apparent;
int32_t VALUE_Current, VALUE_Voltage, VALUE_Power, VALUE_Reactive, VALUE_Apparent;

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
IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*----- DEFINE USE FUNTION PRINT -----*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* ----END-------- */

ADE7758_SPI ade7758_1;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART2)
  {
    Flag_Rx = true;
    memcpy(cop_rx_data, rx_data, MAX_BUFFER);
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_data, MAX_BUFFER);
  }
}

int stringToNumber(const char *str)
{
  return atoi(str); // Hàm atoi có sẵn trong thư viện stdlib.h
}

void numberToString(int num, char *str)
{
  sprintf(str, "%d", num); // Hàm sprintf để chuyển số thành chuỗi
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_data, MAX_BUFFER);
  ADE7758_Init(&ade7758_1, &hspi1, GPIOB, GPIO_PIN_0);

  ADE7758_Write(&ade7758_1, 0x05, SAGCYC, 1);
  ADE7758_Write(&ade7758_1, 0x30, SAGLVL, 1);
  ADE7758_Write(&ade7758_1, 0x1C0, MASK, 3);
  ADE7758_Write(&ade7758_1, 0x40, LCYCMODE, 1);
  ADE7758_Write(&ade7758_1, 0x9C, COMPMODE, 1);

  ADE7758_Write(&ade7758_1, 0xF8, AVRMSOS, 2);
  ADE7758_Write(&ade7758_1, 0x19, AIRMSOS, 2);
  ADE7758_Write(&ade7758_1, 0x04, AWG, 2);
  ADE7758_Write(&ade7758_1, 0x04, AVAG, 2);
  ADE7758_Write(&ade7758_1, 0xF6, APHCAL, 2);
  ADE7758_Write(&ade7758_1, 0xF8, AWAITOS, 2);
  ADE7758_Write(&ade7758_1, 0xF8, AVAROS, 2);

  Next_page = CASE1;
  Select = CASE1;
  Flag_Rx = false;
  Flag_display = false;
  count = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /*BEGIN 1 - TAKE OF HE SO*/
    // printf("Gia Tri Dien Ap ADC: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AIRMS, 3));
    // printf("Gia Tri Dong Dien ADC: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVRMS, 3));
    // printf("Gia Tri Cong Suat ADC: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AWATTHR, 2));
    // printf("Gia Tri Cong Suat Phan Khang ADC: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVARHR, 2));
    // printf("Gia Tri Cong Suat Toan Phan ADC: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVAHR, 2));
    /*END 1*/

    /*BEGIN 2 - READ PARAMETER*/
    // dummy[0] = (int32_t)ADE7758_Read(&ade7758_1, AIRMS, 3) * 170 / 7432;
    // dummy[1] = (int32_t)ADE7758_Read(&ade7758_1, AVRMS, 3) * 220 / 1078302;
    // dummy[2] = (int32_t)ADE7758_Read(&ade7758_1, AWATTHR, 2) * 40 / 9;
    // dummy[3] = (int32_t)ADE7758_Read(&ade7758_1, AVARHR, 2);
    // dummy[4] = (int32_t)ADE7758_Read(&ade7758_1, AVAHR, 2);
    // printf("Gia Tri Dien Ap: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AIRMS, 3) * 170 / 7432);
    // printf("Gia Tri Dong Dien: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVRMS, 3) * 220 / 1078302);
    // printf("Gia Tri Cong Suat: %d \n\r", ADE7758_Read(&ade7758_1, AWATTHR, 2) * 40 / 9);
    // printf("Gia Tri Cong Suat Phan Khang: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVARHR, 2));
    // printf("Gia Tri Cong Suat Toan Phan ADC: %d \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVAHR, 2));
    /*END 2*/

    // SEND DATA RESPONED
    //     if (flag_rx)
    //     {
    //       /* code */
    //       flag_rx = false;
    //       char tx_char[10];
    //       uint8_t num = strlen(cop_rx_data);
    //       numberToString(num, tx_char);
    //  HAL_UART_Transmit(&huart2, tx_char, strlen(tx_char), 1000);
    // //      printf("abc");
    //       memset(rx_data, 0, MAX_BUFFER);
    //       // memset(cop_rx_data, 0, MAX_BUFFER);gi
    // }

    switch (Next_page)
    {
    case CASE1:
      /* code */
      if (Flag_display == false)
      {
        /* code */
        Flag_display = true;
        strcpy(Line1, "\n\r| 1. Setting moc chuan de do luong (Press \"1\" de chon): \n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        strcpy(Line1, "| --------------------------------------------------------\n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        strcpy(Line1, "| 2. Do luong gia tri thuc te (Press \"2\" de chon): \n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        strcpy(Line1, "| --------------------------------------------------------\n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        strcpy(Line1, "| 3. Hien thi gia tri moc da set (Press \"3\" de chon): \n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        strcpy(Line1, "| --------------------------------------------------------\n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
      }
      if (Flag_Rx == true)
      {
        /* code */
        if (cop_rx_data[0] == '1')
        {
          /* code */
          Flag_Rx = false;
          memset(rx_data, 0, MAX_BUFFER);
          Next_page = CASE2;
          Flag_display = false;
        }
        if (cop_rx_data[0] == '2')
        {
          /* code */
          Flag_Rx = false;
          memset(rx_data, 0, MAX_BUFFER);
          Next_page = CASE3;
          Flag_display = false;
        }
        if (cop_rx_data[0] == '3')
        {
          /* code */
          Flag_Rx = false;
          memset(rx_data, 0, MAX_BUFFER);
          memset(cop_rx_data, 0, MAX_BUFFER);
          Next_page = CASE4;
          Flag_display = false;
        }
      }

      break;

    case CASE2:
      switch (Select)
      {
      case /* constant-expression */ CASE1:
        /* code */
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri DIEN AP (ADC): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          ADC_Voltage = stringToNumber((char *)cop_rx_data);
          // ADC_Voltage = ADC_Voltage - 100;
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE2;
          Flag_display = false;
        }
        break;

      case CASE2:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri DONG DIEN (ADC): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          ADC_Current = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE3;
          Flag_display = false;
        }
        break;

      case CASE3:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri CONG SUAT (ADC): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          ADC_Power = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE4;
          Flag_display = false;
        }
        break;

      case CASE4:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri CONG SUAT PK (ADC): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          ADC_Reactive = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE5;
          Flag_display = false;
        }
        break;

      case CASE5:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri CONG SUAT TP (ADC): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          ADC_Apparent = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE6;
          Flag_display = false;
        }
        break;

      case CASE6:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri DIEN AP (VOLT): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          VALUE_Voltage = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE7;
          Flag_display = false;
        }
        break;

      case CASE7:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri DONG DIEN (AMPE): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          VALUE_Current = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE8;
          Flag_display = false;
        }
        break;

      case CASE8:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri CONG SUAT (WAT): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          VALUE_Power = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE9;
          Flag_display = false;
        }
        break;

      case CASE9:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri CONG SUAT PK (VAR): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          VALUE_Reactive = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE10;
          Flag_display = false;
        }
        break;

      case CASE10:
        if (Flag_display == false)
        {
          /* code */
          Flag_display = true;
          strcpy(Line1, "Nhap gia tri CONG SUAT TP (VA): \n\r");

          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        }
        if (Flag_Rx == true)
        {
          /* code */
          Flag_Rx = false;
          length = strlen((char *)cop_rx_data);
          VALUE_Apparent = stringToNumber((char *)cop_rx_data);
          // numberToString(ADC_Voltage, (char *)cop_rx_data);
          // HAL_UART_Transmit(&huart2, (uint8_t *)cop_rx_data, strlen((char *)cop_rx_data), Max_Timeout);

          Select = CASE11;
          Flag_display = false;
        }
        break;

      case CASE11:
        if (Flag_display == false)
        {
          /* code */
          strcpy(Line1, "\n\rDa nhap DIEN AP (ADC-VALUE)      : ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)ADC_Voltage, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          strcpy(Line1, "  -  ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)VALUE_Voltage, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);

          strcpy(Line1, "\n\rDa nhap DONG DIEN (ADC-VALUE)    : ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)ADC_Current, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          strcpy(Line1, "  -  ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)VALUE_Current, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);

          strcpy(Line1, "\n\rDa nhap CONG SUAT (ADC-VALUE)    : ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)ADC_Power, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          strcpy(Line1, "  -  ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)VALUE_Power, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);

          strcpy(Line1, "\n\rDa nhap CONG SUAT PK (ADC-VALUE) : ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)ADC_Reactive, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          strcpy(Line1, "  -  ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)VALUE_Reactive, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);

          strcpy(Line1, "\n\rDa nhap CONG SUAT TP (ADC-VALUE) : ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)ADC_Apparent, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          strcpy(Line1, "  -  ");
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);
          numberToString((int)VALUE_Apparent, (char *)Line1);
          HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
          memset(Line1, 0, MAX_BUFFER);

          Next_page = CASE1;
        }

        break;

      default:
        break;
      }

      break;

    case CASE3:
      printf("Gia Tri DONG DIEN   : %ld \n\r", (int32_t)ADE7758_Read(&ade7758_1, AIRMS, 3) * VALUE_Current * 10 / ADC_Current);
      printf("Gia Tri DIEN AP     : %ld \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVRMS, 3) * VALUE_Voltage / ADC_Voltage);
      printf("Gia Tri Cong Suat   : %ld \n\r", (int32_t)ADE7758_Read(&ade7758_1, AWATTHR, 2) * VALUE_Power * 10 / ADC_Power);
      printf("Gia Tri Cong Suat Phan Khang : %ld \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVARHR, 2) * VALUE_Reactive * 10 / ADC_Reactive);
      printf("Gia Tri Cong Suat Toan Phan  : %ld \n\r", (int32_t)ADE7758_Read(&ade7758_1, AVAHR, 2) * VALUE_Apparent * 10 / ADC_Apparent);
      Next_page = CASE1;
      
      break;

    case CASE4:
      if (Flag_display == false)
      {
        /* code */
        strcpy(Line1, "\n\rDa nhap DIEN AP (ADC-VALUE)      : ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)ADC_Voltage, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        strcpy(Line1, "  -  ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)VALUE_Voltage, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);

        strcpy(Line1, "\n\rDa nhap DONG DIEN (ADC-VALUE)    : ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)ADC_Current, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        strcpy(Line1, "  -  ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)VALUE_Current, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);

        strcpy(Line1, "\n\rDa nhap CONG SUAT (ADC-VALUE)    : ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)ADC_Power, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        strcpy(Line1, "  -  ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)VALUE_Power, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);

        strcpy(Line1, "\n\rDa nhap CONG SUAT PK (ADC-VALUE) : ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)ADC_Reactive, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        strcpy(Line1, "  -  ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)VALUE_Reactive, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);

        strcpy(Line1, "\n\rDa nhap CONG SUAT TP (ADC-VALUE) : ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)ADC_Apparent, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        strcpy(Line1, "  -  ");
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);
        numberToString((int)VALUE_Apparent, (char *)Line1);
        HAL_UART_Transmit(&huart2, (uint8_t *)Line1, strlen(Line1), Max_Timeout);
        memset(Line1, 0, MAX_BUFFER);

        Next_page = CASE1;
      }
      break;

    default:
      break;
    }
    HAL_IWDG_Refresh(&hiwdg);
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

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief Retargets the C library printf function to the USART.
 * @param None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);

  return ch;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
