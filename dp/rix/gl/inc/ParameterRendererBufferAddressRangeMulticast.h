// Copyright (c) 2016, NVIDIA CORPORATION. All rights reserved.
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

#include <dp/rix/gl/inc/ParameterCacheStream.h>
#include <dp/rix/gl/inc/ParameterRendererStreamBuffer.h>
#include <dp/gl/BufferUpdater.h>
#include <memory>

namespace dp
{
  namespace rix
  {
    namespace gl
    {

      typedef void (GLAPIENTRY * PFNGLLGPUNAMEDBUFFERSUBDATANVXPROC) (GLbitfield gpuMask, GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);

      /************************************************************************/
      /* ParameterRendererBufferAddressRangeMulticast                         */
      /************************************************************************/

      class ParameterRendererBufferAddressRangeMulticast : public ParameterRendererStreamBuffer
      {
      public:
        ParameterRendererBufferAddressRangeMulticast( ParameterCacheEntryStreamBuffers const& parameterCacheEntries, dp::gl::BufferSharedPtr const& buffer
                                                    , GLenum target, size_t bindingIndex, GLsizeiptr bindingLength
                                                    , bool batchedUpdates, size_t containerSize, uint32_t numberOfGPUs);

        virtual void activate();

        virtual void render( void const* cache );
        virtual void update( void* cache, void const* container );
        virtual size_t getCacheSize() const;

      protected:
        GLenum                                  m_target;
        dp::gl::BufferSharedPtr                 m_buffer;
        GLint                                   m_bindingIndex;
        GLuint64                                m_baseAddress;
        GLsizeiptr                              m_bindingLength;
        size_t                                  m_containerSize;
        uint32_t                                m_numberOfGPUs;
        std::unique_ptr<uint8_t[]>              m_cacheData;

        // private, added until glew supports the extension
        PFNGLLGPUNAMEDBUFFERSUBDATANVXPROC glLGPUNamedBufferSubDataNVX;
      };

    } // namespace gl
  } // namespace rix
} // namespace dp

