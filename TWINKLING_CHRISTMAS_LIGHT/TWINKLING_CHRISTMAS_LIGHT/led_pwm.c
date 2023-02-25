/*
 * led_pwm.c
 *
 * Created: 12/02/2023 22:47:30
 *  Author: Misha Zaslavskis
 */ 
#include "led_pwm.h"

void led_init()
{
	/* Set up GPIO PD5 and PD6 as OUTPUT mode */
	DDRD |= (1 << DDD5); // Set output mode for PD5 pin in MCU
	DDRD |= (1 << DDD6); // Set output mode for PD6 pin in MCU
	/* Initialize TIM0 PMW mode */
	TCCR0A = 0b11100011; // Set OC0A on compare match with inverted mode and OC0B on compare match without inverted mode
	TCCR0B = 0b00000011; // Set prescaler 64
	/* Reset GPIO for PD5 and PD6 via OCR0x setting like set LOW level for all LEDs */
	OCR0A = 255;
	OCR0B = 0;
}
void change_brightness_for_all_leds(unsigned int brightness_level)
{
	if (brightness_level > 127) brightness_level = 127;
	else if (brightness_level < 0) brightness_level = 0;
	
	OCR0A = 255 - brightness_level;
	OCR0B = brightness_level;
}
void change_brightness_for_single_leds(unsigned int led_pin_pwm, unsigned int brightness_level)
{
	led_pin_pwm = brightness_level;
}
void set_led(unsigned int gpio_register, unsigned int gpio_state)
{
	if (gpio_state) PORTD |= (1 << gpio_register);
	else PORTD &= ~(1 << gpio_register);
}
void color_led_switch(int direction_polarity_of_led)
{
	if (direction_polarity_of_led == FORWARD_LED_ON)
	{
		PORTD |= (1 << PORTD5);
		PORTD &= ~(1 << PORTD6);
	}
	else if (direction_polarity_of_led == REVERSE_LED_ON)
	{
		PORTD |= (1 << PORTD6);
		PORTD &= ~(1 << PORTD5);
	}
}
void set_mode_led(unsigned int mode)
{
	if (mode == PWM_MODE)
	{
		TCCR0A = 0b11100011; // Set OC0A on compare match with inverted mode and OC0B on compare match without inverted mode
	}
	else
	{
		TCCR0A = 0b00000011; // Set normal mode timer and disable any PMW pins  
	}
}
void reset_all_leds()
{
	PORTD &= ~(1 << PORTD5);
	PORTD &= ~(1 << PORTD6);
}

