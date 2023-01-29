# Buzzer
Interface for piezo buzzers based on the Arduino framework.

No more `delay()` while playing sounds on a buzzer!
Create custom melodies and pass them to your Buzzer object.

Everything will play automatically from a single function call!

## Usage

This library is intended to be used with the Arduino framework. It has been tested with the **Arduino Uno** and **ESP32** boards.

### Creating a buzzer object
The `Buzzer` class constructor takes a GPIO pin number as argument, plus a channel number from 0 to 15 if you are using the ESP32 board. There is an optional `Buzzer.init()` function if you use the empty constructor.

Refer to the `/examples` folder for more context.
### Creating a melody
Melodies are created with the `Melody_t` struct provided with this library.

Refer to the `/examples` folder for more context.

### Playing a melody
The buzzer object plays the melodies automatically from the `Buzzer.step()` function. This function must be placed in the `loop()` function in order for the buzzer to play.

Refer to the `/examples` folder for more context.