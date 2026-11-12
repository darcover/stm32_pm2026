#include "stm32f1xx.h"
#include "spi.h"

void delay(uint32_t ticks)
{
    for (volatile uint32_t i = 0; i < ticks; i++)
    {
        __NOP();
    }
}

void display_cmd(uint8_t cmd)
{
    GPIOA->ODR &= ~GPIO_ODR_ODR4; 
    GPIOA->ODR &= ~GPIO_ODR_ODR1; 
    delay(1000);
    SPI1_Write(cmd);
    GPIOA->ODR |= GPIO_ODR_ODR4; 
}

void display_data(uint8_t data)
{
    GPIOA->ODR &= ~GPIO_ODR_ODR4; 
    GPIOA->ODR |= GPIO_ODR_ODR1; 
    delay(1000);
    SPI1_Write(data);
    GPIOA->ODR |= GPIO_ODR_ODR4; 
}

void display_init(void)
{
    GPIOA->ODR &= ~GPIO_ODR_ODR2;
    delay(10000);
    GPIOA->ODR |= GPIO_ODR_ODR2;
    delay(10000);

    uint8_t display_on = 0;
    display_cmd(0xAE | display_on);

    display_cmd(0x20);
    display_cmd(0x00);

    display_cmd(0x8D);
    display_cmd(0x14);

    display_cmd(0xA4);
    display_cmd(0xA6);

    display_cmd(0x21);
    display_cmd(0x00);
    display_cmd(0x7F);

    display_cmd(0x22);
    display_cmd(0x00);
    display_cmd(0x07);

    display_on = 1;
    display_cmd(0xAE | display_on);
}

void display_chessboard(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        for (uint8_t col = 0; col < 128; col++)
        {
            if (((page + (col / 8)) % 2) == 0)
            {
                display_data(0xFF);
            }
            else
            {
                display_data(0x00);
            }
        }
    }
}

int main(void)
{
    SPI1_Init();
    display_init();
    display_chessboard();

    while (1)
    {
    }
}