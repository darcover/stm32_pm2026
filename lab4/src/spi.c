#include "stm32f1xx.h"
#include "spi.h"

void SPI1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1 |
                    GPIO_CRL_MODE2 | GPIO_CRL_CNF2 |
                    GPIO_CRL_MODE4 | GPIO_CRL_CNF4 |
                    GPIO_CRL_MODE5 | GPIO_CRL_CNF5 |
                    GPIO_CRL_MODE7 | GPIO_CRL_CNF7);

    GPIOA->CRL |= GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE4;

    GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_CNF5_1 |
                  GPIO_CRL_MODE7 | GPIO_CRL_CNF7_1;

    GPIOA->ODR |= GPIO_ODR_ODR4 | GPIO_ODR_ODR2;

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA |
                 SPI_CR1_MSTR | SPI_CR1_BR |
                 SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data)
{
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
    while(SPI1->SR & SPI_SR_BSY);
}

uint8_t SPI1_Read(void)
{
    SPI1->DR = 0; 
    while(!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}