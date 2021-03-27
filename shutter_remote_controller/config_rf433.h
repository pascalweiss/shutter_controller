// -------- config for 433Mhz sender module --------


// The value that is sent for moving the shutter up.
// You have to sniff it from a remote control
#define RF433_UP "10101000001010110101011000000000" 

// The value that is sent for moving the shutter down.
// You have to sniff it from a remote control
#define RF433_DOWN "10100000001010110101011000000000" 

// Length of the pulse at which the module sends/receives data. 
// See rcSwitch documentation
#define RF433_PULSE_LENGTH 200

// the protocol that is used for sending/receing data
// See rcSwitch documentation
#define RF433_PROTOCOL 2

// set this to the corresponding digital pin
byte PIN_RF433 = 7;