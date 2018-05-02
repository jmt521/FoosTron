# FoosTron

## Hardware Breakdown
* Arduino Mega
* Break bream sensors for goal sensing
* 7 segment displays for goal counters
* 3D printed display plates
* LED strips to illuminate table
* Several buttons to adjust goals and start games, 2 with controllable LEDs
* A whole lot of wire

## Software
### Aruduino Sketch
* Separate classes to handle control of LEDs and 7 segment displays
* Sends game data in JSON over serial port

### Web Scoreboard
* Node, Express, and Socket.IO
* Receives data from Arduino over serial port and sends to client via web socket

