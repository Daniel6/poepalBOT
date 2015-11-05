## Arduino code for poepalBOT

### Serial Motor Controller
Monitors serial communications for motor commands, passes motor commands to DC motors via Adafruit Motor Shield. This module also handles some low level driving logic such as line following and line finding. By extensions this means that line following sensors must be attached to the arduino.