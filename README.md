# Robot

The project is part of the `62491 Interdisciplinary Project` course from [DTU](http://www.dtu.dk/english) (Technical University of Denmark)

The robot is rather complex digital and analog system that consists of many components and subsystems that are interrelated in terms of its routine. The block diagram in figure outlines the basic components of the system and the connections that are between them. 

![Block diagram of the overall system](https://g0rr9g.dm2302.livefilestore.com/y4m-m7xtm7OUjdfOXTYThGoTWh5qH2_qQm5oZ1yURHpDU9BdFJEmz9o4_iw9q9iwiXbKreH4jc4HTAeVD8txWNAXmBaC_5eelmwQ61Lhz6akbVjWUmVP_Puv9jaRJjHiVfdlmW7RWPrr9j1jBTYDANesV-ogEEYx3Xu7nr1--1PbL397L9D0xPKAjQv0f3NTNuoYcg5FiCDXCBa5BKY7bLEdg?width=771&height=411&cropmode=none "Block diagram of the overall system")

Block diagram of the overall system

One reading could notice the main processor to be Atmels AVR Atmega 32 MCU that inputs from sensor board, both distance sensors and gets a feedback from encoders. It processes the inputs and controls both motors through H bridge while communication and debugging is ensured through Bluetooth module.
