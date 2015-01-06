// Copyright NVIDIA Corporation 2011
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#pragma once

#include <test/testfw/core/TestRender.h>
#include <test/sgrdr/framework/SgRdrBackend.h>

class Benchmark_model : public dp::testfw::core::TestRender
{
public:
  Benchmark_model();
  ~Benchmark_model();

  bool onInit( void );
  bool onRun( unsigned int i );
  bool onClear( void );

  bool onRunCheck( unsigned int i );

  bool option( const std::vector<std::string>& optionString );

protected:
  dp::sg::ui::ViewStateSharedPtr createScene( void );

protected:

  dp::sgrdr::test::framework::RenderDataSgRdr* m_renderData;
  dp::sg::ui::SceneRendererSharedPtr m_sgrdr;

  std::string m_sceneFileName;
  unsigned int m_repetitions;
};

extern "C"
{
  DPTTEST_API dp::testfw::core::Test * create_benchmark_model()
  {
    return new Benchmark_model();
  }
}
