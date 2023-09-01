#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"

#define LEDS GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
#define PINS GPIO_PIN_0 | GPIO_PIN_4

void gpio_init();
void GPIOIntHandler();
void LED_toggle(uint8_t led_number);
void debounce();
int button_state();

