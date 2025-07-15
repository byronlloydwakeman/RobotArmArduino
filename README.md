# Cartesian Robot Arm

The code to control a cartesian robot arm using an Arduino Uno. 

## Overview

The code reads content from the serial port (Arduino must be plugged in to send commands). </br>

These commands can be sent from any other program over the serial port, making it decoupled. </br>

The Arduino will read cartesian distances from the serial port, interpret them, and them move the stepper motors for each axis.</br>

The Arduino then sends an acknowledgement to the serial port, signifiying that it's ready for another command. </br>

The external programs should be setup to handle the buffering and ensure that commands are only send over after an acknowledgement so they're not skipped over or missed.</br>
