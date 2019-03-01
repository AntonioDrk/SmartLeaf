# SmartLeaf 
## Forget about your plants for months,  a project focused on automating simple tasks of everyday life.

*Photo of plant*

# Getting Started

## Components needed

- Arduino Nano

- Medium Breadboard

- Bluetooth Module Master Slave HC-05

- Soil Humidity Sensor Module

- Water Level Module

- 2n2222 Transistor (NPN)

- Mini Submersible Water Pump 3 (3V-5V)

- Wires to connect everything (Jumpers or Rigid)

## Prerequisites

- Arduino IDE installed, can be found [here](https://www.arduino.cc/en/main/software)

- Making a 3d model of a plant pot that can store water and be able to fit the pump in

- A mobile application able to communicate with the HC-05 module (**if you have an iPhone you can't do this**)

## About the project

This project aims to automate the watering of plants by constantly reading the humidity of the soil and trying to keep it at a given level by the user. It also introduces a "lazy watering" button so that the user can relax, press a button that activates the pump for 3 seconds watering the plants.

## Controls

The communication from the phone with the HC-05 module is made with a sort of "state machine". 

There are 3 main states:

1. "Interogation" state, upon recieving the number ( '1' ) or in ascii the number 49 , it sends back a pair "x,y" where **x** is the soil humidity and **y** is the water level

2. "Recieving" state, upon recieving the number ( '2' ) or in ascii the number 50, it awaits another input to set the desired soil moisture percentage ( the pump will be activated every 1 hour if the desired soil moisture isn't reached )

3. "Activation" state, upon recieving the number ( '3' ) or in ascii the number 51 , it writes to the analog pin hooked up to the pump so that it gets activated for 3 seconds.

## Video

*Coming soon*