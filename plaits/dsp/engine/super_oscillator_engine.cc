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
  for (int i = 0; i < 14; ++i) {
    float rank = (static_cast<float>(i) * 0.5 - 3.0) / 3.0;
    super_voice_[i].Init(rank);
  }

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

  const float f0 = NoteToFrequency(parameters.note);

  float shape = parameters.morph;
  CONSTRAIN(shape, 0.0f, 1.0f);

  float pw = parameters.timbre * 0.5f;
  pw = shape > 0.5 ? 0.5f + pw : 0.5f - pw;
  CONSTRAIN(pw, 0.0f, 1.0f);
  
  const float spread = parameters.harmonics * parameters.harmonics * 0.7f;

  const float amplitude = 0.14f * (1.2f - (parameters.morph * 0.2f)) * (1.0f + spread * 0.2f);

  fill(&out[0], &out[size], 0.0f);
  fill(&aux[0], &aux[size], 0.0f);
  for (int i = 0; i < 14; ++i) {
    super_voice_[i].Render(
        f0,
        shape,
        pw,
        spread,
        size,
        temp_buffer_
    );
    if((i & 1) == 0){
      for (size_t j = 0; j < size; ++j) {
        out[j] += temp_buffer_[j] * amplitude;
      }
    }else{
      for (size_t j = 0; j < size; ++j) {
        aux[j] += temp_buffer_[j] * amplitude;
      }
    }
  }

}

}  // namespace plaits
