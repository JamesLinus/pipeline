// Copyright NVIDIA Corporation 2012
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

#include <memory>
#include <string>

namespace dp
{
  namespace fx
  {
    class ParameterSpec;
    class ParameterGroupSpec;
    typedef std::shared_ptr<ParameterGroupSpec> SmartParameterGroupSpec;

    namespace glsl
    {

      std::string getGLSLTypename( const dp::fx::ParameterSpec& parameterSpec );

      /** \brief generate glsl struct for given spec
          \param parameterGroupSpec spec to generate struct for
          \param generatedName output, gives back name of generated struct
      **/
      std::string generateStruct( const dp::fx::SmartParameterGroupSpec& parameterGroupSpec, std::string& generatedName );

      /** \brief foreach parametergroupspec::name print #define name uniformName ([arrayIndex]) accessor name **/
      std::string generateParameterAccessors( const dp::fx::SmartParameterGroupSpec& parameterGroupSpec, const std::string& uniformName, const std::string& arrayIndex, const std::string& accessor );


    } // namespace glsl
  } // namespace fx
} // namespace dp
