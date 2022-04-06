# Guitar Hero Live PokeMachine
This program allows you to play clone hero or other guitar hero like game on your computer with a guitar hero live guitar. 
Actually, it is working for the PS3, the PS4 and the WIIU guitar. To make it work:
1. Plug your USB dongle
2. Synchronize your guitar
3. Unzip GHLPokeMachine
4. Start the executable
5. Press start button in the program
6. When finish, press stop

- If you connect another guitar after the application has been started, stop the application and start it again. The application search for devices only when the start button is pressed.
- If you previously installed the WinUSB driver for the PS3/Wii U dongle to use GHLtar Utility, you will need to uninstall it as this is using HidUsb.
- Your antivirus may warn you about this program. Select "know more", and then "accept the risk". 

You may test your device on this website: https://gamepad-tester.com/

Here's why without this program, it doesn't work. The PS3, the PS4 and the WIIU guitar need a poking message that enables it. When the guitar doesn't receive the poke, it will not send the status of more than one button at a time. Which is unfortunate, since you need to press the fret button and the strum bar simultaneously. GHLPokeMachine poke the guitar to enable the multi touch. Once started, when you press start, the program will pass through all your connected device and for each guitar that it see, will start a thread that will poke your guitar. If there's a connection problem or the stop button is being pressed, the thread will stop. 
