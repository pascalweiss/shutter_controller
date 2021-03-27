// --------config for NRF24 transceiver --------

// If you don't use NRF24 set this to 0. Set it to 1 otherwise
#define RF24_ENABLE 1    

// the percentual change of the RF24 value, at which a shutter activity is triggered
#define THRESHOLD_RF24 0.05   

// Number of the digital pin that is used for the CE connector
byte PIN_RF24_CE  = 9;   

// Number of the digital pin that is used for the CSN connector
byte PIN_RF24_CSN = 10;  