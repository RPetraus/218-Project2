// This program simulates an enhanced vehicle alarm system with various safety features.
// It uses input signals to detect driver and passenger presence, seatbelt fastening,
// and ignition button presses, and outputs status via LEDs and UART messages.

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
// UART interface for communication with a computer or external device
// USBTX and USBRX represent transmit and receive pins, respectively
// 115200 is the baud rate for serial communication

//=====[Declaration and initialization of public global variables]=============

int introComplete = false; // Tracks whether the welcome message for the driver has been displayed

//=====[Declarations (prototypes) of public functions]=========================

// Initializes all input pins and sets their modes
void inputsInit();

// Initializes output pins and their default states
void outputsInit();

// Handles ignition logic based on input conditions
void ignitionCase();

// Displays a welcome message for the driver upon first detection
void driverIntroduction();

// Updates the driving state based on safety conditions
void drivingState();

//=====[Implementations of public functions]===================================

void inputsInit() {
  // Configure input pins with pull-down resistors to ensure default LOW state
  driverPresent.mode(PullDown);
  passengerPresent.mode(PullDown);
  driverSeatbelt.mode(PullDown);
  passengerSeatbelt.mode(PullDown);
  ignitionButton.mode(PullDown);
}

void outputsInit() {
  // Configure sirenPin as open-drain to allow external pull-ups
  sirenPin.mode(OpenDrain);
  sirenPin.input();

  // Ensure all indicators are off initially
  blueIndicator = OFF; // Blue LED: Indicates engine running
  greenIndicator = OFF; // Green LED: Indicates safe state for operation
}

//=====[Main function, the program entry point after power on or reset]========

int main() {
  inputsInit();  // Initialize all input pins and their modes
  outputsInit(); // Initialize output pins and their default states

  // Main loop: Continuously check and update system states
  while (true) {
    driverIntroduction(); // Display welcome message if the driver is detected
    drivingState();       // Update driving indicators based on safety inputs
    ignitionCase();       // Handle ignition logic and system response
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
    // Check if all required conditions are met for engine start
    if (driverPresent && passengerPresent && driverSeatbelt && passengerSeatbelt) {
      uartUsb.write("\nEngine started.", 15);

      // Turn off the green indicator and activate the blue indicator
      greenIndicator = OFF;
      blueIndicator = ON;

      // Infinite loop to simulate engine running; stops further program execution
      while (true) {} 
    }
    else {
      // Display ignition failure message and reasons via UART
      uartUsb.write("\nIgnition inhibited", 19);
      uartUsb.write("\nReasons:", 9); 
      sirenPin.output();
      sirenPin = LOW; // Activate siren signal

      // Report individual reasons for failure
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

      // Infinite loop to halt the program in a failed state
      while (true) {} 
    }
  }
}

/*
prints the introduction string in uart when the driver is first present
*/
void driverIntroduction() {
  if (driverPresent && !introComplete) {
    // Display the welcome message via UART if the driver is detected for the first time
    uartUsb.write("\nWelcome to enhanced alarm system model 218-W24", 46);
    introComplete = true;
  }
}

/*
tells the program what state the code is in; if green led should be on/off and/or ignition pressed
*/
void drivingState() {
  // The green indicator lights up when all safety conditions (driver and passenger present,
  // seatbelts fastened) are met, indicating that the vehicle is in a safe state to operate.
  if (driverPresent && passengerPresent && driverSeatbelt && passengerSeatbelt) {
    greenIndicator = ON; // All conditions met, safe state
  } else {
    greenIndicator = OFF; // One or more conditions not met, unsafe state
  }
}
