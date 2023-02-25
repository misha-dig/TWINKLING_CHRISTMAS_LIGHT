/*
 * ...
 *
 * Created: 1/25/2023 01:39:33
 * Author : Misha Zaslavskis
 */
#ifndef __LED_H
#define __LED_H

#include <avr/io.h>
#include <stdint.h>

#define HIGH 1
#define LOW 0

#define PWM_MODE 1
#define TWO_STATE_MODE 0

#define FORWARD_LED_ON 0
#define REVERSE_LED_ON 1

#define LED_PIN_PD5 PORTD5
#define LED_PIN_PD6 PORTD6

#define LED_PWM_PIN_PD5 OCR0B
#define LED_PWM_PIN_PD6 OCR0A

void led_init();
void change_brightness_for_all_leds(unsigned int brightness_level);
void change_brightness_for_single_leds(unsigned int led_pin_pwm, unsigned int brightness_level);
void set_led(unsigned int gpio_register, unsigned int gpio_state);
void set_mode_led(unsigned int mode);
void color_led_switch(int direction_polarity_of_led);
void reset_all_leds();

#endif // __LED_H