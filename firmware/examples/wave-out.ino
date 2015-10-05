/* 
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "PhotonWaveOut/PhotonWaveOut.h"

// connect a 8Ohm speaker to pin D1 and d2
// using two pins, we can output PWM on both pins D1 for positive wave parts and D2 for the
// negative parts (AC DDS)
// if we run out of pins, we can also connect the speaker to GND and D1 and only write to D1
// this will be quiter and put more stress onto the speaker (DC DDS)
//
// add a capacitor between both pins and add a resist on one pin to form a low-pass filter

const int pwm_p = D1;
const int pwm_n = D2;

// internal LED to signal activity
const int led = D7;

// button to trigger playback
const int btn = D0;

WaveOut *wave;
#include "wave_data.h"

void setup() {
  pinMode(led, OUTPUT);
  wave = new WaveOut(pin_p, pin_n);
}

bool clicked = false;
bool playing = false;

void loop() {
  // read the state of the pushbutton value:
  int buttonState = digitalRead(btn);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    if (!clicked) {
      clicked = true;
    }
  } else {
    if (clicked) {
      if (playing) {
        wave->stop();
        playing = false;
        digitalWrite(led, LOW);
      } else {
        wave->play(wave_data, sizeof(wave_data), true);
        playing = true;
        digitalWrite(led, HIGH);
      }
      clicked = false;
    }
  }
}

