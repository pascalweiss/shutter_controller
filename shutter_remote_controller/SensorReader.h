#include <nRF24L01.h>
#include <RF24.h>

#define N_READINGS 4  // the number of sensor readings, that are stored in the history

class SensorReader {
    public:
        // Constructor
        SensorReader(byte pin_rf24_ce, byte pin_rf24_csn, bool enable_rf24, int pin_poti, 
                           int poti_max, float threshold_rf24, float threshold_poti,
                           int size_hist, int size_actl);
        void read_sensors();
        float get_target();
        float l_poti[N_READINGS];
        float l_rf24[N_READINGS];
    private: 
        RF24 *rf24;
        int SIZE_HIST;
        int PIN_POTI;
        int MAX_POTI;
        int SIZE_ACTL;
        bool ENABLE_RF24;
        float RF24_THRESHOLD;
        float POTI_THRESHOLD;
        float target;
        void push_front(float *arr, float val);
        float read_poti();
        float read_rf24();
        bool sensor_changed(float* arr, float threshold);
};