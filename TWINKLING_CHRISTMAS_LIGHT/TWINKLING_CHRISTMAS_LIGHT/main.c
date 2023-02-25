/*
 * main.c
 *
 * Created: 21/02/2023 21:28:40
 * Author: Misha Zaslavskis
 */ 

/* Set clock frequency of MCU than we can handle with delay function correctly */
#define F_CPU 14745600

/* The defines value of OC1A for easier to find */
#define FIVE_HERTZ_MODE_PRESCALER_256 5759
#define TEN_HERTZ_MODE_PRESCALER_256 2879
#define TWENTY_HERTZ_MODE_PRESCALER_256 1439
#define FOURTY_HERTZ_MODE_PRESCALER_256 719
#define ONE_HUNDRED_HERTZ_MODE_PRESCALER_256 287
#define ONE_EIGHTHS_HERTZ_MODE_PRESCALER_1024 57599
#define ONE_FOURTH_HERTZ_MODE_PRESCALER_1024 28799
#define HALF_HERTZ_MODE_PRESCALER_1024 14399
#define ONE_HERTZ_MODE_PRESCALER_1024 7199
#define TWO_HERTZ_MODE_PRESCALER_1024 3599

/* The defines value of LED mode */
#define FADE_BOTH_LED 0
#define FADE_SINGLE_LED 1
#define BLINK_BOTH 2
#define SWITCH_SIDE_OF_LED 3
#define STEADY_ON_BOTH_LEDS 4
#define STEADY_ON_FORWARD_LED 5
#define STEADY_ON_BACKWARD_LED 6
#define TURN_OFF_ALL_LEDS 7

#define DEFAULT_FREQUNCY 2879
#define BUFFER_SIZE 32

/* Include standard AVR library */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Including of my sources code */
#include "uart.h"
#include "led_pwm.h"

unsigned int _brightness = 127;
unsigned int _led_mode = 0;
unsigned int _period_mode = 3;
unsigned int _time_step_mode = 0;
unsigned int _current_brigthness = 0;
unsigned int _led_state = 0;
unsigned int _led_increment = 0; 
unsigned int _side_of_led_state = 0;

int main(void)
{
	/* Initialize UART */
	UART_init();
	/* Initialize LED */
	led_init();
	/* Initialize TIM1 with default prescaler 256 */
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100;
	TCCR1C = 0b00000000;
	/* Mask interrupt for timer function */
	TIMSK1 |= (1 << OCIE1A);
	/* Set up output compare of timer 1 */
	OCR1A = DEFAULT_FREQUNCY;
	/* enable global interrupt */
	sei();
	
    /* Infinite loop */
    while (1) 
    {
		/* Declaration buffer */
		uint8_t buffer[BUFFER_SIZE] = {'\0'}; // Buffer array for entering command
		
		/* Loop that find ASCII character to perform commands when hit a ENTER button */
		for (int count_buf = 0; count_buf < BUFFER_SIZE - 1; count_buf++)
		{
			do
			{
				/* Receive single characters after send character in UART console */
				buffer[count_buf] = UART_Receive_single_character();
			}
			/* Checking that no special ASCII characters. If special character is found you need to reenter letter, digits, space, carriage return */
			while ((buffer[count_buf] < 32 && buffer[count_buf] > 13) || buffer[count_buf] < 13);
			
			/* Exit receiving characters after hitting enter button in keyboard or putting carriage return in your serial port terminal */
			/* Check character has CR. If found CR character finish send UART and exit from this for loop for starting perform tasks */
			if (buffer[count_buf] == 13)
			{
				UART_Transmit_multiple_characters(buffer); // Print the buffer in UART terminal for checking purpose
				UART_Transmit_single_character('\n');
				break; //Exit from this loop operation
			}
		}
		
		/* Execute command if you entered correct name of command in serial terminal
		 * You must put a argument strlen(buffer) - 1 in each function strncmp to avoid doesn't working this commands */
		
		/* LEDs commands */
		if (strncmp(buffer, (uint8_t*)"FADE_BOTH_LEDS", strlen(buffer) - 1) == 0) _led_mode = 0; // PWM mode and step mode
		if (strncmp(buffer, (uint8_t*)"FADE_SINGLE_LED", strlen(buffer) - 1) == 0) _led_mode = 1; // PWM mode and step mode
		if (strncmp(buffer, (uint8_t*)"BLINK_BOTH", strlen(buffer) - 1) == 0) _led_mode = 2; // PWM mode and blink mode
		if (strncmp(buffer, (uint8_t*)"SWITCH_SIDE_OF_LED", strlen(buffer) - 1) == 0) _led_mode = 3; // GPIO mode and blink mode 
		if (strncmp(buffer, (uint8_t*)"STEADY_ON_BOTH_LEDS", strlen(buffer) - 1) == 0) _led_mode = 4; // PWM mode and steady on mode 
		if (strncmp(buffer, (uint8_t*)"STEADY_ON_FORWARD_LED", strlen(buffer) - 1) == 0) _led_mode = 5; // PWM mode and steady on mode
		if (strncmp(buffer, (uint8_t*)"STEADY_ON_BACKWARD_LED", strlen(buffer) - 1) == 0) _led_mode = 6; // PWM mode and steady on mode
		if (strncmp(buffer, (uint8_t*)"TURN_OFF_ALL_LEDS", strlen(buffer) - 1) == 0) _led_mode = 7; // GPIO mode and steady on mode 
		
		/* Set period of step of change led brightness  */
		if (strncmp(buffer, (uint8_t*)"SET_PWM_TIME_STEP_0S100", strlen(buffer) - 1) == 0)  _time_step_mode = 0;
		if (strncmp(buffer, (uint8_t*)"SET_PWM_TIME_STEP_0S050", strlen(buffer) - 1) == 0)  _time_step_mode = 1;
		if (strncmp(buffer, (uint8_t*)"SET_PWM_TIME_STEP_0S025", strlen(buffer) - 1) == 0)  _time_step_mode = 2;
		if (strncmp(buffer, (uint8_t*)"SET_PWM_TIME_STEP_0S013", strlen(buffer) - 1) == 0)  _time_step_mode = 3;
		if (strncmp(buffer, (uint8_t*)"SET_PWM_TIME_STEP_0S005", strlen(buffer) - 1) == 0)  _time_step_mode = 4;
		
		/* Set frequency of blinking led */
		if (strncmp(buffer, (uint8_t*)"SET_BLINK_FREQ_0.125HZ", strlen(buffer) - 1) == 0) _period_mode = 0;
		if (strncmp(buffer, (uint8_t*)"SET_BLINK_FREQ_0.25HZ", strlen(buffer) - 1) == 0) _period_mode = 1;
		if (strncmp(buffer, (uint8_t*)"SET_BLINK_FREQ_0.5HZ", strlen(buffer) - 1) == 0) _period_mode = 2;
		if (strncmp(buffer, (uint8_t*)"SET_BLINK_FREQ_1HZ", strlen(buffer) - 1) == 0) _period_mode = 3;
		if (strncmp(buffer, (uint8_t*)"SET_BLINK_FREQ_2HZ", strlen(buffer) - 1) == 0) _period_mode = 4;
		
		/* Message that for setting PWM brightness */
		else
		{
			if(strncmp(buffer, (uint8_t*)"SET_BRIGHTNESS_LIGHT=", 21) == 0)
			{
				UART_Transmit_multiple_characters("Set your brightness! \r\n"); // Warn we changed value of LED
				_brightness = atoi(&buffer[21]); // convert for char* to int value
			}
		}
		
		/* LED actions non-repeatability every time unless send a command */
		
		/* Set modes of LED dependent what command you choose (PWM vs GPIO digital mode)*/
		switch(_led_mode)
		{
			/* For number LED command set GPIO digital mode*/
			case 3:
			case 7:
			set_mode_led(TWO_STATE_MODE);
			break;
			
			/* For other number LED command set PWM mode in MCU pins with OC0A and OC0B output compare mode  */
			default:
			set_mode_led(PWM_MODE);
			break;	
		}
		
		/* Set frequency of blink for LED mode 2nd and 3th*/
		if (_led_mode == 2 || _led_mode == 3)
		{
			/* Set prescaler of TIM1 1024 */
			TCCR1B |= (1 << CS12);
			TCCR1B &= ~(1 << CS11);
			TCCR1B |= (1 << CS10);
			/* Switch frequency/period of TIM1*/
			switch(_period_mode)
			{
				case 0: OCR1A = ONE_EIGHTHS_HERTZ_MODE_PRESCALER_1024; break;
				
				case 1: OCR1A = ONE_FOURTH_HERTZ_MODE_PRESCALER_1024; break;
				
				case 2: OCR1A = HALF_HERTZ_MODE_PRESCALER_1024; break;
				
				case 3: OCR1A = ONE_HERTZ_MODE_PRESCALER_1024; break;
				
				case 4: OCR1A = TWO_HERTZ_MODE_PRESCALER_1024; break;
			}
		}
		/* For fading lights LED mode in 0 and 1*/
		else if (_led_mode == 0 || _led_mode == 1)
		{
			/* Set prescaler of TIM1 256 */
			TCCR1B |= (1 << CS12);
			TCCR1B &= ~(1 << CS11);
			TCCR1B &= ~(1 << CS10);
			/* Switch frequency/period of TIM1*/
			switch(_time_step_mode)
			{
				case 0: OCR1A = FIVE_HERTZ_MODE_PRESCALER_256; break;
				
				case 1: OCR1A = TEN_HERTZ_MODE_PRESCALER_256; break;
				
				case 2: OCR1A = TWENTY_HERTZ_MODE_PRESCALER_256; break;
				
				case 3: OCR1A = FOURTY_HERTZ_MODE_PRESCALER_256; break;
				
				case 4: OCR1A = ONE_HUNDRED_HERTZ_MODE_PRESCALER_256; break;
			}
		}
		/* Steady on or off LED mode */
		else if (_led_mode >= 4)
		{
			OCR1A = DEFAULT_FREQUNCY;
		}
    }
}

/* The interrupt callback. When interrupt is trigger in event of timer 1 match compare for OC1A */
ISR(TIMER1_COMPA_vect)
{
	switch (_led_mode)
	{
		/* Fading of all led */
		case FADE_BOTH_LED:
		{
			_led_increment++;
			if (_led_increment > 127)
			{
				_led_state = !_led_state;
				_led_increment = 0;
			}
			
			if (_led_state) change_brightness_for_all_leds(_led_increment);
			else change_brightness_for_all_leds(127 - _led_increment);
		} break;
		
		/* Fading single led */
		case FADE_SINGLE_LED:
		{
			_led_increment++;
			if (_led_increment > 255)
			{
				_side_of_led_state++;
				if (_side_of_led_state >= 4) _side_of_led_state = 0;
				_led_increment = 0;
			}
			
			switch(_side_of_led_state)
			{
				case 0:
				{
					OCR0A = 255;
					OCR0B = _led_increment;
				} break;
				
				case 1:
				{
					OCR0A = 255;
					OCR0B = 255 - _led_increment;
				} break;
				
				case 2:
				{
					OCR0A = 255 - _led_increment;
					OCR0B = 0;
				} break;
				
				case 3:
				{
					OCR0A = _led_increment;
					OCR0B = 0;
				} break;
			}
		} break;
		
		/* Blinking both sides of LEDs */
		case BLINK_BOTH:
		{
			_led_state 	= !_led_state;
			if (_led_state) change_brightness_for_all_leds(_brightness);
			else change_brightness_for_all_leds(0);
		} break;
		
		/* Blinking each side of LED*/
		case SWITCH_SIDE_OF_LED:
		{
			_led_state = !_led_state;
			if (_led_state) color_led_switch(FORWARD_LED_ON);
			else color_led_switch(REVERSE_LED_ON);
		} break;
		
		/* Steady on both of side LEDs*/
		case STEADY_ON_BOTH_LEDS:
		{
			change_brightness_for_all_leds(_brightness);
		} break;
		
		/* Steady on forward direction of side LED */
		case STEADY_ON_FORWARD_LED:
		{
			OCR0A = 255;
			OCR0B = _brightness;
		} break;
		
		/* Steady on backward direction of side LED */
		case STEADY_ON_BACKWARD_LED:
		{
			OCR0A = 255 - _brightness;
			OCR0B = 0;
		} break;
		
		/* Turn off all directions side of LEDs*/
		case TURN_OFF_ALL_LEDS:
		{
			reset_all_leds();
		} break;
	}
}