// -------- general config --------


// The number of sensor readings, that are stored in the history.
// Relates to other constants like HIST_SIZE or ACTL_SIZE.
#define N 4     

// The duration (in millis) the program waits after each iteration
#define DELAY 200


// -------- state change detection --------

// The number of old readings, that are used to determine, 
// if the sensor state has changed. 
// (requirement N > HIST_SIZE + ACTL_SIZE)
#define HIST_SIZE 2     

// The number of actual readings, that are used to determine, 
// if the sensor state has changed. 
// (requirement N > HIST_SIZE + ACTL_SIZE)
#define ACTL_SIZE 2     


// -------- cycle detection --------

// the number of directn commands, that are used to determine, 
// if the system has a command cycle (shutter goes up and down 
// in an endless loop)
#define DIRECTN_SIZE 12    

// the number of repeated STOP-directn-commands, to break the stop-cycle
#define STOP_CYCLE_THRESHOLD 3     


// -------- motor control --------

// the percentage of the difference of the target-value 
// at which the motor should be activated
#define THRESHOLD_MOVE_MOTOR 0.01  

// the amount of seconds the motor takes to roll up the shutter
#define SECS_ROLL_UP 23 // bedroom 21, living-room 23

// the amount of seconds the motor takes to roll down the shutter
#define SECS_ROLL_DOWN 23 // bedroom 22, living-room 23


// -------- motor state recalibration --------

// The threshold at which the shutter is pulled up additionally for some seconds. 
// If we do this it is ensured that the shutter is actually at the highest point. 
// This has the effect that the estimated state of the shutter gets recalibrated.
#define THRESHOLD_OVERSHOOT_UP   0.98  

// the seconds that the shutter is pulled up additionally, 
// when the position reached THRESHOLD_OVERSHOOT_UP
#define SECS_OVERSHOOT_UP        3     

// The threshold at which the shutter is pulled down additionally for some seconds. 
// If we do this it is ensured that the shutter is actually at the lowest point. 
// This has the effect that the estimated state of the shutter gets recalibrated.
#define THRESHOLD_OVERSHOOT_DOWN 0.02  

// the seconds that the shutter is pulled down additionally, 
// when the position reached THRESHOLD_OVERSHOOT_DOWN
#define SECS_OVERSHOOT_DOWN      3     
