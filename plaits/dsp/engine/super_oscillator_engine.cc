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
// 7 variable shape oscillators with spread, shape and pw.

#include "plaits/dsp/engine/super_oscillator_engine.h"

#include <algorithm>

#include "stmlib/dsp/parameter_interpolator.h"

namespace plaits {

using namespace std;
using namespace stmlib;

void SuperOscillatorEngine::Init(BufferAllocator* allocator) {
  primary_.Init();

  temp_buffer_ = allocator->Allocate<float>(kMaxBlockSize);
}

void SuperOscillatorEngine::Reset() {
  
}

void SuperOscillatorEngine::Render(
    const EngineParameters& parameters,
    float* out,
    float* aux,
    size_t size,
    bool* already_enveloped) {

  // 1 = variable waveshape controlled by TIMBRE.
  // 2 = variable waveshape controlled by MORPH, detuned by HARMONICS.
  // OUT = 1 + 2.
  // AUX = 1 + sync 2.
  const float primary_f = NoteToFrequency(parameters.note);

  float shape = parameters.morph;
  CONSTRAIN(shape, 0.0f, 1.0f);

  float pw = parameters.timbre;
  CONSTRAIN(pw, 0f, 1.0f);
  
  primary_.Render<false>(
      primary_f, primary_f, pw, shape, out, size);
  
}

}  // namespace plaits
