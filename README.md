The Project consists of two ECUs 
The first ECU is the Human Machine Interface, This ECU interfaces with all the modules that the Human user directly interacts with like The LCD and Keypad
Used Drivers: GPIO, Timer1, USART, Keypad, LCD
The Second ECU is the Control ECU which controls the system functions and everything else that doesn't interact directly with the user like EEPROM, Door Motor, and Buzzer.
Used Drivers: GPIO, Timer1, PWM, TWI, USART, ExternaL EEPROM, DC Motor, Buzzer.
The two ECUs communicate with each other using the USART communication protocol.
