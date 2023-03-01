# TWINKLING_CHRISTMAS_LIGHT
A project make adjustable brightness and frequency blinking twinkling Christmas light by using existing or old twinkling Christmas light via PC/embedded computer over UART interface. Based via atmega328p microcontroller. This code of project is used based LED_CONTROLLER_VIA_UART_ATMEGA328P for UART communication which the describtion has deep explainion about how use source of code. Links my pervious repository https://github.com/misha-dig/LED_CONTROLLER_VIA_UART_ATMEGA328P , you may be to read about codes and credits. 

# Why I choose the project
Becuase many twinkling christmas light haven't advanced controller such able to dim and change functionality like RGB LED with remote controller by using existing old twinkling Christmas light. Changing function of LED by communication via PC/embedded computer, such Raspberry PI or server with local network to able controller remotely. You need to use USB UART Bridge adapter for computer hasn't internal UART pins for usage LED controller over computer. Also, you may be to use controller when power supply and controller of twinkling christmas light is broked is rarely case.    

# Features
* Adjusable brightness light from dimmer to brigthest level
* Wide range speed of change frequency/speed for fading and blinking LEDs
* Switch to steady mode with adjusable light and change group of colors of LEDs
* This project is show only two group of colors of LEDs (Red-Yellow group and Green-Blue group)
* This project is support for LED device with change color dependenting polarity connected LED. Most notable than Bi-color LED can change color with dependent what polarity +/- connected to DC power source. 
* Open-Source code of this project, you can custom and modify command for different type of LED devices (e.g. RGB LED strips or twinkling christmas light with seperated wire each color of LED that indicated 5 wires from controller block), but you need to have knowedge about electronics, programming and knowedge how work AVR microcontroller. 
* Including able to turn off LED via enter command without disconnect from socket.

# Pictures

![photo_2023-02-25_01-14-35](https://user-images.githubusercontent.com/55639759/221327978-54a15a06-51fa-4435-99aa-76a29a05ca90.jpg)

![image](https://user-images.githubusercontent.com/55639759/221334836-9c1a4fbb-2173-4b5d-af24-624250e5b2dc.png)


# Usage serial command

Please set as speed 115200 bit/s and 8bit - data, 1bit - stopbit, no parity before using this device. 

```FADE_BOTH_LEDS``` - set LED as fading all groups of color 

```FADE_SINGLE_LED``` - set LED as fading each group of color by ordering

```BLINK_BOTH``` - set LED blink all groups of color with adjusable brightness

```SWITCH_SIDE_OF_LED```- set frequency blink each group of color of LED by ordering 

```STEADY_ON_BOTH_LEDS``` - steady on all groups of color of LEDs with adjusable brightness

```STEADY_ON_FORWARD_LED``` - steady on single first group of color of LED with adjusable brightness

```STEADY_ON_BACKWARD_LED``` - steady on single second group of color of LED with adjusable brightness

```TURN_OFF_ALL_LEDS``` - Turn off all groups of color of LEDs

```SET_PWM_TIME_STEP_0S100``` - Set time of step of change brigthness of LEDs 100 ms each step.  

```SET_PWM_TIME_STEP_0S050``` - Set time of step of change brigthness of LEDs 50 ms each step.

```SET_PWM_TIME_STEP_0S025``` - Set time of step of change brigthness of LEDs 25 ms each step.

```SET_PWM_TIME_STEP_0S013``` - Set time of step of change brigthness of LEDs about 13 ms each step (exactly 12.5 ms).

```SET_PWM_TIME_STEP_0S005``` - Set time of step of change brigthness of LEDs 5 ms each step.



```SET_BLINK_FREQ_0.125HZ``` - Set frequency of blinking LED 0.125 HZ / 8 seconds period.

```SET_BLINK_FREQ_0.25HZ``` - Set frequency of blinking LED 0.25 HZ / 4 seconds period.

```SET_BLINK_FREQ_0.5HZ``` - Set frequency of blinking LED 0.5 HZ / 2 seconds period.

```SET_BLINK_FREQ_1HZ``` - Set frequency of blinking LED 1.0 HZ / 1 seconds period.

```SET_BLINK_FREQ_2HZ``` - Set frequency of blinking LED 2.0 HZ / 0.5 seconds period.

```SET_BRIGHTNESS_LIGHT=``` Set brightness of light for function steady on and blinking light. 

You can set from 0 to 127 (for both groups of color LED mode) and from 0 to 255 (for single groups of color LED  mode). For example, you can send a command via UART ```SET_BRIGHTNESS_LIGHT=100``` - that LEDs will set brightness value of 100. If you entered a command ```SET_BRIGHTNESS_LIGHT=27``` - that LEDs will set brightness value of 27.  

Hex value of carrige return - ``` 0x0d ```. Decimal value of carrige return - ``` 13 ```.

If you enterred command other than list above, you will receive error message.

Default command is ```FADE_BOTH_LEDS``` after turn on our LED controller without entering commands. 

You can change frequency/period/time of each step of change brightness light of blinking and fading LED later to make comfortable for you. 

Value of frequency/period/time of each step of change brightness light is save only during working. For example, you set steady on LED mode after setting frequency of blinking LED 2 HZ and when you re-enter blinking led after setting to steady on LED mode will you got 2 HZ blinking LED. All value of setting LED mode save in RAM memory of microcontroller. 

# Building source code
You need have to Atmel Studio 7 IDE as building code, if you don't have it tools you need to download from website https://www.microchip.com/en-us/tools-resources/develop/microchip-studio 

1. Download our this repository

2. Unzip this achieve of our this repository and open file with file extension .atsln via Atmel Studio 7 IDE software

3. After opening the IDE. You need to choose 'build' from menu bar and click build solution to get binary file.

4. Open file explorer with ```\TWINKLING_CHRISTMAS_LIGHT\TWINKLING_CHRISTMAS_LIGHT\Debug``` with folder name using name of repository and find .hex and .elf binary file. 

PS: I'll provide instruction how build source code of this project.

# Programm our firmware to microcontroller atmega328p

You need to have hardware USBtiny programmer and microcontroller atmega328 circuit board with oscillator crystal 14.7456 MHz to able programm. 

Using AVRDUDES software to programm our binary file. 

PS: I'll provide instruction how programm firmware soon and provide electronics circuit soon.
