#if ! (defined(CH32V00x) || defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x) || defined(CH32V035))
#error "Processor architecture is not supported."
#endif

#ifndef CH32_DEEP_SLEEP_H
#define CH32_DEEP_SLEEP_H

#include "Arduino.h"

#define AWUPSC_MASK      ((uint32_t)0xFFFFFFF0)
#define AWUWR_MASK       ((uint32_t)0xFFFFFFC0)

// The autowakeup delay can be calculated by:
//
// t = AWUWR * AWUPSC / fLSI
//
// AWUWR:  1 ... 63
// fLSI:   128000
// AWUPSC: 2048, 4096, 10240, 61440 - lower values are possible.
// The maximum autowakeup delay is 30s.
//
// t = AWUWR * AWUPSC / 128000
// AWUWR = t * 128000 / AWUPSC

enum base_periode_t {
    BASE_MS_1   = PWR_AWU_Prescaler_128,
    BASE_MS_2   = PWR_AWU_Prescaler_256,
    BASE_MS_4   = PWR_AWU_Prescaler_512,
    BASE_MS_8   = PWR_AWU_Prescaler_1024,
    BASE_MS_16  = PWR_AWU_Prescaler_2048,
    BASE_MS_32  = PWR_AWU_Prescaler_4096,
    BASE_MS_80  = PWR_AWU_Prescaler_10240,
    BASE_MS_480 = PWR_AWU_Prescaler_61440
};

class DeepSleep {
    public:
        void init(base_periode_t base = BASE_MS_480, uint8_t num = 2) {
            // enable power interface module clock
            RCC->APB1PCENR |= RCC_APB1Periph_PWR;

            // enable low speed oscillator (LSI)
            RCC->RSTSCKR |= RCC_LSION;
            while ((RCC->RSTSCKR & RCC_LSIRDY) == 0) {}

            // enable AutoWakeUp eventAPB2PCENR
            EXTI->EVENR |= EXTI_Line9;
            EXTI->FTENR |= EXTI_Line9;

            // configure AWU prescaler
            PWR->AWUPSC &= ~AWUPSC_MASK;
            //PWR->AWUPSC |= PWR_AWU_Prescaler_2048;    // dt = 0.016s
            //PWR->AWUPSC |= PWR_AWU_Prescaler_4096;    // dt = 0.032s
            //PWR->AWUPSC |= PWR_AWU_Prescaler_10240;   // dt = 0.080s
            //PWR->AWUPSC |= PWR_AWU_Prescaler_61440;   // dt = 0.480s (DEFAULT)
            PWR->AWUPSC |= base;

            // configure AWU window comparison value
            PWR->AWUWR &= ~AWUWR_MASK;
            PWR->AWUWR |= (num==0)?1:(num>63)?63:num;    // Default: 0.961 seconds

            // enable AWU
            PWR->AWUCSR |= (1 << 1);

            // select standby on power-down
            PWR->CTLR |= PWR_CTLR_PDDS;

            // peripheral interrupt controller send to deep sleep
            PFIC->SCTLR |= (1 << 2);
        }

        void sleep() {
            __WFE();        // Enter sleep with autowakeup
            SystemInit();   // restore clock to full speed
        }

        void sleepI() {
            __WFI();        // Enter sleep with autowakeup and IRQ wakeup
            SystemInit();   // restore clock to full speed
        }
};

DeepSleep DeepSleep;

#endif
