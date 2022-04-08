# Guitar Hero Live PokeMachine
This application allows you to play clone hero or other guitar hero like game on your computer with a guitar hero live guitar (6 frets). Currently, the application is working for the PS3, the PS4 and the WIIU dongles.

## How to use it
1. Download the file named GHLPokeMachine.zip in the releases section 
2. Unzip GHLPokeMachine.zip
3. Start the executable
4. Plug your USB dongle
5. Synchronize your guitar
6. Press the start button in the program
7. When finished, press stop

- If you connect another guitar after the application has been started, stop the application and start it again. The application search for devices only when the start button is pressed.
- If you previously installed the WinUSB driver for the PS3/Wii U dongle to use GHLtar Utility, you will need to uninstall it as this is using HidUsb.
- Your antivirus may warn you about this program. Select "know more", and then "accept the risk". 

You may test your device on this website: https://gamepad-tester.com/

## How it works
Here's the goal of this application. The PS3, the PS4 and the WIIU dongles needs a poking message that enables it. When the dongles doesn't receive the poke, it will not send the status of more than one button at a time. Which is unfortunate, since you need to press the fret button and the strum bar simultaneously. GHLPokeMachine pokes the dongle to enable the multi touch. Once started, when you press start, the program will pass through all your connected device and for each dongle that it see, will start a thread that will poke your dongle. If there's a connection problem or the stop button is being pressed, the thread will stop. 
