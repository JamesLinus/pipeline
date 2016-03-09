// Copyright (c) 2011-2016, NVIDIA CORPORATION. All rights reserved.
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

#include <dp/ui/RenderTarget.h>
#include <dp/util/DynamicLibrary.h>
#include <test/testfw/core/inc/Config.h>

namespace dp
{
  namespace testfw
  {
    namespace core
    {
      class TestRender;

      class RenderData
      {
      public:
        DPTCORE_API virtual ~RenderData();
      };

      DEFINE_PTR_TYPES( Backend );

      class Backend : public std::enable_shared_from_this<Backend>
      {
        friend class TestRender;

      public:
        virtual ~Backend(){}

        //The back end context interface
        DPTCORE_API virtual dp::ui::RenderTargetSharedPtr createDisplay( int width, int height, bool visible ) = 0;
        DPTCORE_API virtual void render( RenderData* renderData, dp::ui::RenderTargetSharedPtr renderTarget = dp::ui::RenderTargetSharedPtr() ) = 0;
        DPTCORE_API virtual void finish() = 0;
      };

    } // namespace core
  } // namespace testfw
} // namespace dp
