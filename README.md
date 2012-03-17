# MWRC 2012 CI Server / Hardware hacking workshop

So... you want to connect lights and sounds to your CI server and don't want to have to do it through a computer?

*You're in luck.*

We're using:

* [Arduino Uno](http://arduino.cc/en/Main/ArduinoBoardUno)
* [Arduino Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield)
* [Sparkfun's super bright RGB LED board](http://www.sparkfun.com/tutorials/299)

Contained in this repo are source files: 1 for Arduino and a folder with a simple sinatra app, as well as a presentation and a PDF printed version of Sparkfun's assembly instructions.

The sinatra app represents a server and a protocol that represents an arbitrary remote server.

The arduino connects to the remote server and gets the status. Depending on the status, it lights up a corresponding light.

