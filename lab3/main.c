#include <stdint.h>
#include "stm32f1xx.h"

__attribute__((noinline)) void delay_ms(uint32_t ms)
{
    __asm volatile (
        "push {r0}\r\n"
        "mov R0, %0\r\n"
        "_loop:\r\n"               
            "cmp R0, #0\r\n"       
            "beq _exit\r\n"       
            "sub R0, R0, #1\r\n"  
            "nop\r\n"              
            "b _loop\r\n"          
        "_exit:\r\n"
        "pop {r0}\r\n"
        :: "r"(9000 * ms)         
    );
}

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13)) | GPIO_CRH_MODE13_0;

    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14)) | GPIO_CRH_CNF14_1;
    GPIOC->ODR |= GPIO_ODR_ODR14;

    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15)) | GPIO_CRH_CNF15_1;
    GPIOC->ODR |= GPIO_ODR_ODR15;


    uint32_t delay_time = 512;
    uint32_t i;

    while (1)
    {
        if (!(GPIOC->IDR & GPIO_IDR_IDR14))
        {
            if (delay_time > 8) 
            {
                delay_time >>= 1;
            }
            for (i = 0; i < 300; i++) { __NOP(); }  
            while (!(GPIOC->IDR & GPIO_IDR_IDR14)); 
        }

        if (!(GPIOC->IDR & GPIO_IDR_IDR15))
        {
            if (delay_time < 32768) 
            {
                delay_time <<= 1;
            }
            for (i = 0; i < 300; i++) { __NOP(); }  
            while (!(GPIOC->IDR & GPIO_IDR_IDR15)); 
        }

        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        delay_ms(delay_time);

        GPIOC->ODR |= GPIO_ODR_ODR13;
        delay_ms(delay_time);
    }
}