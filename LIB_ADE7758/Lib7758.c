/*
 * Lib7758.c
 *
 *  Created on: Oct 21, 2024
 *      Author: PC
 */

#include "main.h"

/*
 * Set the MISO, MOSI, SCK and CS
 * SPI settings:
 * CPHA = 1 Edge
 * Prescaler = 8
 * First bit = MBS first
 * CPOL = Low
 */
void ADE7758_Init(ADE7758_SPI *spi, SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS_PORT, uint16_t CS_PIN)
{
    spi->hspi = hspi;
    spi->CS_PORT = CS_PORT;
    spi->CS_PIN = CS_PIN;
}

/**
 * @brief Read DATA from ic ADE7758
 *
 * @param spi  Opject of typedef struct ADE7758_SPI
 *
 * @param value  Value write to ic ADE7758 (D7..D0)
 *
 * @param address address registor of ADE7758 (A6..A0)
 *
 * @param length Length of Value written to ix ADE7758
 *
 * @note Frame Write Data to ADE7758 (copy form datasheet of ADE7758)
 * @note Mode: MSB Fisrt
 * @note  CLK: |       7       |  6 |  5 |  4 |  3 |  2 | 1  |  0  |       |  7   | 6  |  5 | 4  | 3  | 2  | 1  | 0  |   ...
 * @note MOSI: | START BIT = 1 | A6 | A5 | A4 | A3 | A2 | A1 | A0  |       |  D7  | D6 | D5 | D4 | D3 | D2 | D1 | D0 |   ...
 * @note MISO: |       X       |  x |  x |  x |  x |  x |  x |  x  |       |  X   | X  | X  | X  | X  | X  | X  | X  |   ...
 */
void ADE7758_Write(ADE7758_SPI *spi, int32_t value, uint8_t address, uint8_t length)
{
    /*Declare ADDRESS that we will send*/
    uint8_t pTxData[4] = {0};
    pTxData[0] = address | 0x80;

    switch (length)
    {
    case 1: // Length = 1byte
        /* code */
        pTxData[1] = (uint8_t)(value & 0xFF);
        break;
    case 2: // Length = 2bytes
        /* code */
        pTxData[1] = (uint8_t)((value >> 8) & 0xFF);
        pTxData[2] = (uint8_t)(value & 0xFF);
        break;
    case 3: // Length = 3bytes
        /* code */
        pTxData[1] = (uint8_t)((value >> 16) & 0xFF);
        pTxData[2] = (uint8_t)((value >> 8) & 0xFF);
        pTxData[3] = (uint8_t)(value & 0xFF);
        break;
    default:
        break;
    }

    /*Data that we will get*/
    uint8_t pRxData[4] = {0};

    /*CS low, Send and receive, CS high*/
    HAL_GPIO_WritePin(spi->CS_PORT, spi->CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi->hspi, pTxData, pRxData, 4, 10);
    HAL_GPIO_WritePin(spi->CS_PORT, spi->CS_PIN, GPIO_PIN_SET);
}

int32_t ADE7758_Read(ADE7758_SPI *spi, uint8_t address, uint8_t length)
{
    /*Declare ADDRESS that we will send*/
    uint8_t pTxData[4] = {0};
    pTxData[0] = address;

    /*Data that we will get*/
    uint8_t pRxData[4] = {0};

    /*CS low, Send and receive, CS high*/
    HAL_GPIO_WritePin(spi->CS_PORT, spi->CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi->hspi, pTxData, pRxData, 4, 10);
    HAL_GPIO_WritePin(spi->CS_PORT, spi->CS_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    switch (length)
    {
    case 1: // Length = 1byte
        /* code */
        return 0x0000FF & (pRxData[1] & 0xFF);
        break;
    case 2: // Length = 2bytes
        /* code */
        return 0x00FFFF & (((pRxData[1] & 0xFF) << 8) | (pRxData[2] & 0xFF));
        break;
    case 3: // Length = 3bytes
        /* code */
        return 0xFFFFFF & (((pRxData[1] & 0xFF) << 16) | ((pRxData[2] & 0xFF) << 8) | (pRxData[3] & 0xFF));
        break;
    default:
        break;
    }
    return -1;
}
