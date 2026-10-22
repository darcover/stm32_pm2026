#include <stdint.h>
#include "stm32f1xx.h"

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;

    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13)) | GPIO_CRH_MODE13_0;

    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14)) | GPIO_CRH_CNF14_1;
    GPIOC->ODR |= GPIO_ODR_ODR14; // Включаем подтяжку к плюсу (Pull-up)

    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15)) | GPIO_CRH_CNF15_1;
    GPIOC->ODR |= GPIO_ODR_ODR15; // Включаем подтяжку к плюсу (Pull-up)

    GPIOA->CRL = (GPIOA->CRL & ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0)) | GPIO_CRL_CNF0_1;
    GPIOA->ODR |= GPIO_ODR_ODR0;


    uint32_t half_period_ticks = 64; 
    uint32_t current_tick = 0;
    uint32_t i;

    GPIOC->ODR &= ~GPIO_ODR_ODR13;

    while (1)
    {
        if (!(GPIOC->IDR & GPIO_IDR_IDR14))
        {
            for (i = 0; i < 30000; i++) { __NOP(); } 
            if (!(GPIOC->IDR & GPIO_IDR_IDR14))
            {
                if (half_period_ticks > 1)
                {
                    half_period_ticks >>= 1; 
                }
                while (!(GPIOC->IDR & GPIO_IDR_IDR14)) { __NOP(); }
                for (i = 0; i < 30000; i++) { __NOP(); } 
            }
        }

        if (!(GPIOC->IDR & GPIO_IDR_IDR15) || !(GPIOA->IDR & GPIO_IDR_IDR0))
        {
            for (i = 0; i < 30000; i++) { __NOP(); } 
            if (!(GPIOC->IDR & GPIO_IDR_IDR15) || !(GPIOA->IDR & GPIO_IDR_IDR0))
            {
               
                if (half_period_ticks < 4096)
                {
                    half_period_ticks <<= 1; 
                }
                while (!(GPIOC->IDR & GPIO_IDR_IDR15) || !(GPIOA->IDR & GPIO_IDR_IDR0)) { __NOP(); }
                for (i = 0; i < 30000; i++) { __NOP(); } 
            }
        }

        for (i = 0; i < 5000; i++) { __NOP(); }

        current_tick++;
        if (current_tick >= half_period_ticks)
        {
            current_tick = 0;
            GPIOC->ODR ^= GPIO_ODR_ODR13;
        }
    }
}