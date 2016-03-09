// Copyright (c) 2012-2016, NVIDIA CORPORATION. All rights reserved.
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


#include <test/testfw/manager/Manager.h>
#include <dp/Assert.h>

using namespace dp;
using namespace testfw;
using namespace core;

TestList& getTestList()
{
  static TestList testList;
  return testList;
}

bool registerTest( const std::string& testName, TestObject* testObject )
{
  TestList& testList = getTestList();

  TestList::iterator it = testList.find( testName );
  if( it == testList.end() )
  {
    testList[testName] = testObject;
    return true;
  }

  DP_ASSERT( !"test already registered!" );
  return false;
}


extern "C"
{
  DPTTEST_API void appendTests( std::vector<TestObject>& testsOut )
  {
    TestList& testList = getTestList();

    for( TestList::iterator it = testList.begin(); it != testList.end(); ++it )
    {
      testsOut.push_back( *(*it).second );
    }
  }

  DPTTEST_API TestObject* getTest( const char* testName )
  {
    TestList& testList = getTestList();

    TestList::iterator it = testList.find( testName );
    if( it == testList.end() )
    {
      return nullptr;
    }

    return testList[testName];
  }
}