// Copyright 2016 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// 2 variable shape oscillators with sync, FM and crossfading.

#include "plaits/dsp/engine/virtual_analog_engine0.h"

#include <algorithm>

#include "stmlib/dsp/parameter_interpolator.h"

namespace plaits {

using namespace std;
using namespace stmlib;

void VirtualAnalogEngine0::Render(
    const EngineParameters& parameters,
    float* out,
    float* aux,
    size_t size,
    bool* already_enveloped) {
  
  // 1 = variable waveshape controlled by TIMBRE.
  // 2 = variable waveshape controlled by MORPH, detuned by HARMONICS.
  // OUT = 1 + 2.
  // AUX = 1 + sync 2.
  const float auxiliary_detune = ComputeDetuning(parameters.harmonics);
  const float primary_f = NoteToFrequency(parameters.note);
  const float auxiliary_f = NoteToFrequency(parameters.note + auxiliary_detune);
  const float sync_f = NoteToFrequency(
      parameters.note + parameters.harmonics * 48.0f);

  float shape_1 = parameters.timbre * 1.5f;
  CONSTRAIN(shape_1, 0.0f, 1.0f);

  float pw_1 = 0.5f + (parameters.timbre - 0.66f) * 1.4f;
  CONSTRAIN(pw_1, 0.5f, 0.99f);

  float shape_2 = parameters.morph * 1.5f;
  CONSTRAIN(shape_2, 0.0f, 1.0f);

  float pw_2 = 0.5f + (parameters.morph - 0.66f) * 1.4f;
  CONSTRAIN(pw_2, 0.5f, 0.99f);
  
  primary_.Render<false>(
      primary_f, primary_f, pw_1, shape_1, temp_buffer_, size);
  auxiliary_.Render<false>(auxiliary_f, auxiliary_f, pw_2, shape_2, aux, size);
  for (size_t i = 0; i < size; ++i) {
    out[i] = (aux[i] + temp_buffer_[i]) * 0.5f;
  }
  
  sync_.Render<true>(primary_f, sync_f, pw_2, shape_2, aux, size);
  for (size_t i = 0; i < size; ++i) {
    aux[i] = (aux[i] + temp_buffer_[i]) * 0.5f;
  }
}

}  // namespace plaits
