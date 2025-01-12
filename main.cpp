//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "mbed.h"

//=====[Defines]===============================================================

#define NUMBER_OF_KEYS 4

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D2);
DigitalIn passengerPresent(D3);
DigitalIn driverSeatbelt(D4);
DigitalIn passengerSeatbelt(D5);
DigitalIn ignitionButton(D6);

DigitalOut greenIndicator(LED1);
DigitalOut alarmLed(LED3); // change to D7 and hook up alarm later
DigitalOut blueIndicator(LED2);

//=====[Declaration and initialization of public global variables]=============

bool alarmState = OFF;
bool alarmState2 = OFF;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

//=====[Implementations of public functions]===================================

void inputsInit() {
  driverPresent.mode(PullDown);
  passengerPresent.mode(PullDown);
  driverSeatbelt.mode(PullDown);
  passengerSeatbelt.mode(PullDown);
  ignitionButton.mode(PullDown);
}

void outputsInit() {
  alarmLed = OFF;
  blueIndicator = OFF;
  greenIndicator = OFF;
}

//=====[Main function, the program entry point after power on or reset]========

int main() {
  inputsInit();
  outputsInit();

  while (true) {
    if (driverPresent) {
      printf("\nWelcome to enhanced alarm system model 218-W24");
    }
    while (driverPresent) {
      if (passengerPresent && driverSeatbelt && passengerSeatbelt) {
        alarmState2 = ON;
        greenIndicator = alarmState2;
      }

      else {
        alarmState2 = OFF;
        greenIndicator = alarmState2;
      }

      if (ignitionButton) {
        if (passengerPresent && driverSeatbelt && passengerSeatbelt) {
          printf("\nEngine started.");
          greenIndicator = OFF;
          blueIndicator = ON;
          while (true) {
          } // keeps the code stuck here
        }

        else {
          printf("\nIgnition inhibited");
          printf("\nReasons:"); // needs work to show reasons
          alarmState = ON;
          alarmLed = alarmState;
          if (!driverPresent) {
            printf("\nDriver not present.");
          }
          if (!passengerPresent) {
            printf("\nPassenger not present.");
          }
          if (!driverSeatbelt) {
            printf("\nDriver Seatbelt not fastened.");
          }
          if (!passengerSeatbelt) {
            printf("\nPassenger Seatbelt not fastened.");
          }
          while (true) {
          } // keeps the code stuck here
        }
      }
    }
  }
}
