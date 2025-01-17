//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "mbed.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D2);
DigitalIn passengerPresent(D3);
DigitalIn driverSeatbelt(D4);
DigitalIn passengerSeatbelt(D5);
DigitalIn ignitionButton(D6);

DigitalOut greenIndicator(LED1);
DigitalInOut sirenPin(PE_10);
DigitalOut blueIndicator(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

int introComplete = false; //boolean to tell whether the intro has been completed

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();
void ignitionCase();
void driverIntroduction();
void drivingState();

//=====[Implementations of public functions]===================================

void inputsInit() {
  driverPresent.mode(PullDown);
  passengerPresent.mode(PullDown);
  driverSeatbelt.mode(PullDown);
  passengerSeatbelt.mode(PullDown);
  ignitionButton.mode(PullDown);
}

void outputsInit() {
  sirenPin.mode(OpenDrain);
  sirenPin.input();
  blueIndicator = OFF;
  greenIndicator = OFF;
}

//=====[Main function, the program entry point after power on or reset]========

int main() {
  inputsInit();
  outputsInit();

  while (true) {
    driverIntroduction();
    drivingState();
  }
}

//=====[Implementations of public functions]===================================

/*
activates when the ignition button is pressed and acts based on a case-to-case basis
Ex. uart messages are printed based on which buttons are not pressed to start the engine
if all buttons are pressed the engine will start
*/
void ignitionCase() {
  if (ignitionButton) {
    if (driverPresent && passengerPresent && driverSeatbelt && passengerSeatbelt) {
      uartUsb.write("\nEngine started.", 15);

      greenIndicator = OFF;
      blueIndicator = ON;
      while (true) {} // keeps the code stuck here
    }

    else {
      uartUsb.write("\nIgnition inhibited", 19);
      uartUsb.write("\nReasons:", 9); 
      sirenPin.output();
      sirenPin = LOW;

      if (!driverPresent) {
        uartUsb.write("\nDriver not present.", 20);
      }
      if (!passengerPresent) {
        uartUsb.write("\nPassenger not present.", 23);
      }
      if (!driverSeatbelt) {
        uartUsb.write("\nDriver Seatbelt not fastened.", 30);
      }
      if (!passengerSeatbelt) {
        uartUsb.write("\nPassenger Seatbelt not fastened.", 33);
      }
      while (true) {} // keeps the code stuck here
    }
  }
}

/*
prints the introduction string in uart when the driver is first present
*/
void driverIntroduction() {
  if (driverPresent && !introComplete) {
    uartUsb.write("\nWelcome to enhanced alarm system model 218-W24", 46);
    introComplete = true;
  }
}

/*
tells the program what state the code is in; if green led should be on/off and/or ignition pressed
*/
void drivingState() {
  if (driverPresent && passengerPresent && driverSeatbelt &&
      passengerSeatbelt) {
    greenIndicator = ON;
  }

  else {
    greenIndicator = OFF;
  }

  ignitionCase();
}
