// --------config for the poti --------

// The tolerance for the poti. A change of the poti value is only observed, 
// when the difference to the new value exceeds the tolerance. 
// Out of pyhsical reasons the read value from a poti is never constant. 
#define THRESHOLD_POTI 0.015 

// max value of the poti. Depends on controller-/poti type
#define POTI_MAX 1023  

// min value of the poti.
#define POTI_MIN 0 

// analog pin that is used for reading out the poti    
byte PIN_POTI = A0;
