/*
    Name:       ReverbTest.ino
    Created:	11/30/2020 7:56:51 AM
    Author:     TGargasz
*/

#include "DaisyDuino.h"

DaisyHardware hw;

size_t num_channels;
unsigned long currentMillis;        // To set period to poll knobs and switchs
unsigned long previousMillis = 0;   // Last time knobs and switches were polled
const unsigned long interval = 250; // Interval at which to poll (milliseconds)

// ReverbSc verb;               // This works
ReverbSc DSY_SDRAM_BSS verb;    // This does not allow loop() to process and the reverb sounds terrible

void MyCallback(float** in, float** out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        float out1, out2;
        verb.Process(in[0][i], in[1][i], &out1, &out2);

        out[0][i] = out1;
        out[1][i] = out2;
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    float sample_rate;
    // Initialize for Daisy pod at 48kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    num_channels = hw.num_channels;
    sample_rate = DAISY.get_samplerate();

    verb.Init(sample_rate);
    verb.SetFeedback(0.85f);
    verb.SetLpFreq(18000.0f);

    digitalWrite(LED_BUILTIN, HIGH); // Turn led on

    DAISY.begin(MyCallback);
}

void loop()
{
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        // save the last poll time
        previousMillis = currentMillis;

        digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
        delay(20);
    }
}
