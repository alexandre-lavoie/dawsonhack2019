# Dawson Hackathon 2019
Repository for team int kiwi.

# Team

Alexandre Lavoie, Rayaan Kazim, Tony Wen, Daud Khan

# Winner

Team int kiwi won the Creativity and Ingenuity Prize.

# Challenge

The second Dawson College Hackathon challenge was to navigate a robot using provided equipment through a 4x8 grid that included two line following sections and one remote control maze. Winners were determined according to the following:

1. Fastest and fewest penalties over the entire path.
2. Fastest and fewest penalties in the maze.
3. Creativity and Ingenuity of the robot desing.

# Repository

This repository contains all the code created prior/during the competition.

## Arduino

Contains the code for the bot. This includes a line follower using a color sensor, remote control, and an LED 7 segment display.

## Processing

Contains a code for communicating to an Arduino using Processing. This is obsolete, given the competition didn't use the Ultrasonic sensor.

# Plan

This was the plan of action for the competition. Using the SCRUM workflow, this competition had three main sprints: Servos, IR Receiver, and Color Sensors. A last sprint was made for design (7 segment display) and a better user experience. Following is a checklist used during the competition:

## Basic Hardware

- [x] Install Arduino Nano
- [x] Install Servo Motors and Shield
- [x] Install IR Receiver
- [x] Install Color Sensors


## Hardware Testing

- [x] Calibrate Servos
- [x] Receive Basic IR Signals
- [x] Calibrate Color Sensors

## Basic Software

- [x] Forward / Backward + Rotation of Servos
- [x] Executing Commands from IR Receiver
- [x] Receiving Color Input from Color Sensors

## Algorithms

- [x] Line Follower
- [ ] Line Avoider for Manual Control

## Design Cleanups

- [x] Car Design
- [x] Improve IR Range
- [x] Improve IR Controls