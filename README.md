# setting up the app:

1. download keil v4 and install it which is the IDE I used to write and compile the code
2. you need to have the tm4c123gh6pm evalaution board to run and test the code
3. to see the messages printed through the UART you need to have putty.exe to communicate with the board through pc ports
4. no sensors, push buttons nor leds are required to test the code as the board contains 2 push buttons, 3 leds and a temperature sensor
___

# how to run the code:

1. import the files under the folder named code files into a new project in keil
2. set the target device in the project options to tm4c123gh6pm and select frequenct to 16MHz
3. connect the board to the pc through any usb port and install the required drivers in order to be able to upload the code to the board
4. compile and upload the code to the board
5. push the Reset button in the board
6. open the putty.exe to see the UART messages
___

# assumptions I've made:

1. every 3 seconds there's a timer interrupt which trigger the ADC to sample the temperature sensor reading and send it in celcius to the UART
2. portF in the board has 3 leds and 2 switches (PF0-> sw1, PF1:PF3->led1:led3, PF4->sw2)
3. the required function has 2 arguments: (can be tested with different inputs in the main.c file)
	1. the first argument is the pin number of the input switch (0 for sw1, 4 for sw2)
	2. the second argument is the pin number of the output switch (1 for red led, 2 for blue led, 4 for green led)
4. when a switch is pushed the led is toggled and the status of the led is sent to the UART and displayed through putty