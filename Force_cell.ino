#include <ADS1220_WE.h>
#include <SPI.h>
#include <math.h>

#define ADS1220_CS_PIN    7 
#define ADS1220_DRDY_PIN  6 

ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
static constexpr float ADS1220_RANGE {8388607.0}; // = 2^23 - 1 as float
unsigned long tareDuration = 1000;  // Dauer für Tare-Kalibrierung (in Millisekunden)

// Manuelles Festlegen von gain und vref
int gain = 128;  // Verstärkungsfaktor
float vref = 5.0;  // Referenzspannung in Volt

void setup() {
  Serial.begin(115200);
  if (!ads.init()) {
    Serial.println("ADS1220 is not connected!");
    while (1);
  }
  
  ads.setConversionMode(ADS1220_SINGLE_SHOT);
  ads.setOperatingMode(ADS1220_TURBO_MODE);
  ads.setVRefValue_V(5.0);
  ads.setCompareChannels(ADS1220_MUX_1_2);
  ads.setVRefSource(ADS1220_VREF_REFP0_REFN0);
  ads.setGain(ADS1220_GAIN_128);  
  //// The data rate level with setDataRate(). The data rate itself also depends on the operating
   //  mode and the clock. If the internal clock is used or an external clock with 4.096 MHz the data
   //  rates are as follows (per second):
   //   Level               Normal Mode      Duty-Cycle      Turbo Mode
   //  ADS1220_DR_LVL_0          20               5               40         (default)
    // ADS1220_DR_LVL_1          45              11.25            90
   //  ADS1220_DR_LVL_2          90              22.5            180
   //  ADS1220_DR_LVL_3         175              44              350
   //  ADS1220_DR_LVL_4         330              82.5            660
   //  ADS1220_DR_LVL_5         600             150             1200
   //  ADS1220_DR_LVL_6        1000             250             2000
  ads.setDataRate(ADS1220_DR_LVL_4);
  ads.setLowSidePowerSwitch(ADS1220_SWITCH);
}

void loop() {

  float force = kN();
  Serial.println(force);
  
}

float kN(void) {
  int32_t rawData = ads.getRawData();  
  float calibrationFactor = 0.67;
  float resultInMV = (rawData / ADS1220_RANGE) * vref * 1000.0 / (gain * 1.0);
  float force_kN = resultInMV / calibrationFactor;
  return force_kN;
}