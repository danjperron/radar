radar
=====

Although the project name is radar, it actually uses sonars, and is built for indoor mapping for a quadcopter.

This code is just for display. The required libraries are not included.

radar.py
----
This is the code that runs on the computer. It receives data from the Arduino through USB-serial communication and then redraws the radar view. 

radar.ino
----
This is the Arduino code. It reads from the sonars and transmit the data to the computer. Between every reading, the servo motor is rotated a bit. The code supports any number of sonars at the same time.
