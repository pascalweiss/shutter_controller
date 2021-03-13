// --- config ---

#define N                        4     // the number of sensor readings, that are stored in the history
#define DELAY                    200
#define HIST_SIZE                2     // the number of old    readings, that are used to determine, if the sensor state has changed. (requirement N > HIST_SIZE + ACTL_SIZE)
#define ACTL_SIZE                2     // the number of actual readings, that are used to determine, if the sensor state has changed. (requirement N > HIST_SIZE + ACTL_SIZE)
#define DIRECTN_SIZE             12    // the number of directn commands, that are used to determine, if the system has a command cycle (shutter goes up and down repeatedly)
#define STOP_CYCLE_THRESHOLD     3     // the number of repeated STOP-directn-commands, to break the stop-cycle
#define RF24_ENABLE              1
#define RF24_MAX                 1500
#define POTI_MAX                 1023  // max value of the POTI // schlafzimmer: 3280, prototyp: 4095
#define POTI_MIN                 0
#define RF433_PULSE_LENGTH       200
#define RF433_PROTOCOL           2
#define RF433_UP                 "10000010001010110101011000000000"
#define RF433_DOWN               "10001010001010110101011000000000"
#define THRESHOLD_POTI           0.015  // the tolerance for the poti. A change of the poti value is only observed, when the difference to the new value exceeds the tolerance
#define THRESHOLD_RF24           0.05   // the percentual change of the RF24 value, at which a shutter (Rollladen) activity is triggered
#define THRESHOLD_MOVE_MOTOR     0.01  // the percentage of the difference of the target at which the motor should be activated
#define SECS_ROLL_UP             21    // the amount of seconds the motor takes to roll up the shutter
#define SECS_ROLL_DOWN           22    // the amount of seconds the motor takes to roll down the shutter
#define THRESHOLD_OVERSHOOT_UP   0.98  // the threshold at which the shutter is pulled up additionally for some seconds
#define SECS_OVERSHOOT_UP        3     // the seconds that the shutter is pulled up additionally, when the position reached THRESHOLD_OVERSHOOT_UP
#define THRESHOLD_OVERSHOOT_DOWN 0.02  // the threshold at which the shutter is pulled down additionally for some seconds
#define SECS_OVERSHOOT_DOWN      3     // the seconds that the shutter is pulled up additionally, when the position reached THRESHOLD_OVERSHOOT_UP


byte PIN_RF24_CE     = 9;
byte PIN_RF24_CSN    = 10;
byte PIN_RELAIS_UP   = 4;
byte PIN_RELAIS_DOWN = 5;
byte PIN_POTI        = A0;
byte PIN_RF433       = 7;


// --- setup ---

enum motor_direction { STOPPED, DOWN, UP, STOP, CYCLE_BREAK}; 
enum sensor          { POTI, RF_24 };
