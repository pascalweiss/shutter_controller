// -------- config for 433Mhz sender module --------


// set this to the corresponding digital pin
byte PIN_RF433 = 7;

// The value that is sent for moving the shutter up.
// You have to sniff it from a remote control
//#define RF433_UP "10000000000000110101011000000000" // todo
#define RF433_UP "10000010001010110101011000000000" // bedroom
// #define RF433_UP "10100000001010110101011000000000" // livin-room

// The value that is sent for moving the shutter down.
// You have to sniff it from a remote control
//#define RF433_DOWN "10000000000010110101011000000000" // todo
#define RF433_DOWN "10001010001010110101011000000000" // bedroom
// #define RF433_DOWN "10101000001010110101011000000000" // living-room

// Length of the pulse at which the module sends/receives data. 
// See rcSwitch documentation
#define RF433_PULSE_LENGTH 10

// the protocol that is used for sending/receing data
// See rcSwitch documentation
#define RF433_PROTOCOL 2
