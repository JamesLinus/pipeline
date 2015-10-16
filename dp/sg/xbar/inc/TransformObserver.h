// Copyright (c) 2010-2015, NVIDIA CORPORATION. All rights reserved.
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

#include <dp/sg/xbar/inc/Observer.h>
#include <dp/sg/core/Transform.h>

namespace dp
{
  namespace sg
  {
    namespace xbar
    {
      DEFINE_PTR_TYPES(TransformObserver);
      DEFINE_PTR_TYPES(TransformTree);

      class TransformObserver : public Observer<TransformIndex>
      {
      public:
        DEFINE_PTR_TYPES( DirtyPayload );

        class DirtyPayload : public Payload
        {
        public:
          static DirtyPayloadSharedPtr create( TransformIndex index )
          {
            return( std::shared_ptr<DirtyPayload>( new DirtyPayload( index ) ) );
          }

        protected:
          DirtyPayload( TransformIndex index )
            : Payload( index )
          {
          }
        };

        typedef std::vector<DirtyPayload*> DirtyPayloads;

      public:
        virtual ~TransformObserver();

      public:
        static TransformObserverSharedPtr create(dp::util::BitArray & dirtyTransforms)
        {
          return(std::shared_ptr<TransformObserver>(new TransformObserver(dirtyTransforms)));
        }

        void attach( dp::sg::core::TransformSharedPtr const & t, TransformIndex index );

      protected:
        TransformObserver(dp::util::BitArray & dirtyTransforms)
          : Observer<TransformIndex>()
          , m_dirtyTransforms(dirtyTransforms)
        {
        }

        void onNotify(dp::util::Event const & event, dp::util::Payload * payload);

      private:
        dp::util::BitArray & m_dirtyTransforms;
      };

    } // namespace xbar
  } // namespace sg
} // namespace dp
