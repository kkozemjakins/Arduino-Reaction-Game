# Reaction Speed Game 

This is my first *Arduino* game, i am not saying this is the most correct way, but this is the way i had made it and it works. Of course there is the room for imporovement so if you want you can leave a suggestion 
The idea is that who quicker will press the button **wins**!

## ![til](assets\VID.gif)


**Here is the list of components:**
1.Arduino Uno: The microcontroller that runs the game.

2.Countdown Light LEDs (Red, Yellow, Green): These are the core visual elements for the countdown.

3.Player LEDs (2): One for each player to indicate who won the round.

4.Buttons (3): Two for the players, and one to start the game.

5.Buzzer: An active buzzer that provides an audio cue for the "go" signal.

6.LCD Matrix (128x32): This will display to show the score.

7.Resistors (5 x 220 Ohm): These are crucial for protecting your LEDs from too much current.

**Wiring Diagram and Connections:**
This schematic shows you how to connect all the components to your Arduino. The diagram is for a common I2C OLED display, which has four pins: GND, VCC, SDA, and SCL.

- **LEDs**:

 - **Countdown Lights**: Connect the long leg (anode) of the Red, Yellow, and Green LEDs to digital pins 12, 11, and 10, respectively, with a 220 Ohm resistor on each. Connect the short leg (cathode) of each LED to a common ground rail.

 - **Player LEDs**: Connect the long leg of Player 1's LED to digital pin 5 and Player 2's LED to digital pin 6, each with a 220 Ohm resistor. Connect the short legs to ground.

- **Buttons**:

 - Connect one leg of each of the three buttons to digital pins 2 (Player 1), 3 (Player 2), and 4 (Start).

 - Connect the other leg of each button to the Arduino's GND pin. The code uses the INPUT_PULLUP setting, so you don't need any external resistors here.

- **Buzzer**:

 - Connect one leg of the buzzer to digital pin 9 and the other to GND.

- **LCD Display (I2C)**:

 - Connect the VCC pin on the display to the Arduino's 5V pin.

 - Connect the GND pin on the display to the Arduino's GND pin.

 - Connect the SDA pin on the display to the Arduino's A4 pin.

 - Connect the SCL pin on the display to the Arduino's A5 pin.



