// NRF51 Dongle Discomode

#include <stdlib.h>
#include <stdint.h>
#include <nrf.h>

#include <app_error.h>
#include <app_util_platform.h>
#include <app_simple_timer.h>
#include <nrf_drv_rng.h>

#define LED_RED     21
#define LED_GREEN   22
#define LED_BLUE    23

static void timer_handler(void * unused __attribute((unused)))
{
    uint8_t state;
    nrf_drv_rng_rand(&state, 1);

    uint8_t red   = (state >> 2) & 0x1;
    uint8_t green = (state >> 1) & 0x1;
    uint8_t blue  = (state >> 0) & 0x1;

    NRF_GPIO->OUT = red << LED_RED | green << LED_GREEN | blue << LED_BLUE;
}

int main(void)
{
    NRF_GPIO->DIRSET = 1 << LED_RED | 1 << LED_GREEN | 1 << LED_BLUE;
    NRF_GPIO->OUTSET = 1 << LED_RED | 1 << LED_GREEN | 1 << LED_BLUE;

    nrf_drv_rng_config_t rng_config = {
        .error_correction = false,
        .interrupt_priority = APP_IRQ_PRIORITY_LOW
    };
    APP_ERROR_CHECK(nrf_drv_rng_init(&rng_config));

    APP_ERROR_CHECK(app_simple_timer_init());
    APP_ERROR_CHECK(app_simple_timer_start(APP_SIMPLE_TIMER_MODE_REPEATED, timer_handler,
        8150, NULL));

    while(1)
    {
        __WFE();
    }
}
