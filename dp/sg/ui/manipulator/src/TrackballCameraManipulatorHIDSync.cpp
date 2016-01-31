// Copyright NVIDIA Corporation 2010
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


#include <dp/sg/ui/manipulator/TrackballCameraManipulatorHIDSync.h>

namespace dp
{
  namespace sg
  {
    namespace ui
    {
      namespace manipulator
      {

        TrackballCameraManipulatorHIDSync::TrackballCameraManipulatorHIDSync( )
          : TrackballCameraManipulator()
          , PID_Pos(0)
          , PID_Orbit(0)
          , PID_Pan(0)
          , PID_Shift(0)
          , PID_Control(0)
          , PID_HotSpot(0)
          , PID_HotSpot_Depth(0)
        {
        }

        TrackballCameraManipulatorHIDSync::~TrackballCameraManipulatorHIDSync()
        {
        }

        bool TrackballCameraManipulatorHIDSync::updateFrame( float dt )
        {
          if (!m_hid)
          {
            return false;
          }

          if ( m_hid->getValue<bool>( PID_Shift ) )
          {
            lockMajorAxis();
          }
          else
          {
            unlockMajorAxis();
          }

          bool orbit = m_hid->getValue<bool>( PID_Orbit );
          bool pan = m_hid->getValue<bool>( PID_Pan );
          bool control = m_hid->getValue<bool>( PID_Control );
          bool shift = m_hid->getValue<bool>( PID_Shift );
          bool hotspot = m_hid->getValue<bool>( PID_HotSpot );
          bool hotspot_depth = m_hid->getValue<bool>( PID_HotSpot_Depth );

          setCursorPosition( m_hid->getValue<dp::math::Vec2i>( PID_Pos ) );
          setWheelTicks( m_hid->getValue<int>( PID_Wheel ) );

          TrackballCameraManipulator::Mode mode = TrackballCameraManipulator::Mode::NONE;
          if ( hotspot )
          {
            mode = TrackballCameraManipulator::Mode::LOOKAT;
          }
          else if ( hotspot_depth)
          {
            mode = TrackballCameraManipulator::Mode::LOOKAT_DEPTH;
          }
          else 
          {
            if ( orbit && pan && control && shift )  // vertigo
            {
              mode = TrackballCameraManipulator::Mode::ZOOM_DOLLY;
            }
            if ( orbit && pan && control && !shift )  // zoom
            {
              mode = TrackballCameraManipulator::Mode::ZOOM_FOV;
            }
            if ( orbit && pan && !control && !shift )  // dolly
            {
              mode = TrackballCameraManipulator::Mode::DOLLY;
            }
            else if ( orbit && !pan && !control && !shift )
            {
              mode = TrackballCameraManipulator::Mode::ORBIT;
            }
            else if ( !orbit && pan && !control && !shift )
            {
              mode = TrackballCameraManipulator::Mode::PAN;
            }
            else if ( !orbit && pan && control && !shift )
            {
              mode = TrackballCameraManipulator::Mode::ROLL_Z;
            }
            else if ( orbit && !pan && control && !shift )
            {
              mode = TrackballCameraManipulator::Mode::ROTATE_XY;
            }
          }

          setMode( mode );

          return TrackballCameraManipulator::updateFrame( dt );
        }

        void TrackballCameraManipulatorHIDSync::setHID( HumanInterfaceDevice *hid )
        {
          m_hid = hid;

          PID_Pos = m_hid->getProperty( "Mouse_Position" );
          PID_Orbit = m_hid->getProperty( "Mouse_Left" );
          PID_Pan = m_hid->getProperty( "Mouse_Middle" );
          PID_Shift = m_hid->getProperty( "Key_Shift" );
          PID_Control = m_hid->getProperty("Key_Control");
          PID_HotSpot = m_hid->getProperty("Key_H");
          PID_HotSpot_Depth = m_hid->getProperty("Key_D");
          PID_Wheel = m_hid->getProperty("Mouse_Wheel");
        }

      } // namespace manipulator
    } // namespace ui
  } // namespace sg
} // namespace dp
