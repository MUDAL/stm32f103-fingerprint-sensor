## STM32F103-FINGERPRINT-SENSOR  
This repository contains sample codes for interfacing an stm32f103 microcontroller with a fingerprint sensor.  
The fingerprint library developed for this application was inspired by the **Adafruit Fingerprint Library**.  
The Adafruit library was ported from an Arduino environment to a Keil uVision environment.  
Optimizations were made to the library as data reception from the sensor is handled via Direct Memory Access (DMA).  
The STM32 microcontroller and fingerprint sensor communicate via the **UART** protocol.  
The device drivers (UART,GPIO,SysTick,DMA,RCC) used in the fingerprint library were written from scratch hence the memory   
footprint of the library is much smaller when compared to the usage of the STM32Cube HAL drivers.   

## Fingerprint sensor details  
The Adafruit compatible fingerprint sensors use UART for communicating with a microcontroller.   
Some popular fingerprint sensor modules include:      
1. AS608 sensor  
2. R305 sensor
3. R307 sensor, etc.  

The AS608 sensor was used in testing this library.    
It requires a 3.3v power supply, and it is compatible with microcontrollers that operate with 3.3v logic level.  
It has a default baud rate of 57600 for UART communication.  
In order to communicate with the sensor, some data must be transmitted to the sensor by the microcontroller.  
Upon receiving data from the microcontroller, the fingerprint sensor gives a feedback (or response) which is also called a **packet**.  
More information on the contents of a data **packet** can be found in the manual below.  
[fingerprint_sensor_manual.pdf](https://github.com/MUDAL/stm32f103-fingerprint-sensor/files/8353945/fingerprint_sensor_manual.pdf)  

## More information
I'm open to collaboration and improvement of the codebase. The codes can be customized to match different microcontrollers and   
applications. Porting the library from platform to platform isn't an issue. All that is required is the understanding of UART  
communication (transmission and reception).  

Special thanks to **Adafruit** for their contributions to open source hardware and software. The original code developed by  
Adafruit (which was modified and optimized in this repo) can be found in the following link:  
https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library  

Comparisons between the Adafruit implementation and the implementation in this repo can be made in order to understand how the  
code was ported. If more information is required, I'm available for communication.  


