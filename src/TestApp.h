/** @file
        @brief Header for application object

        @date 2014

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_TestApp_h_GUID_0b548fa0_f573_4798_ad85_c04094a04c3a
#define INCLUDED_TestApp_h_GUID_0b548fa0_f573_4798_ad85_c04094a04c3a

// Internal Includes
// - none

// Library/third-party includes

#include <vrj/Draw/OpenGL/App.h>

// Standard includes
// - none

class TestApp : public vrj::opengl::App {
  public:
    TestApp() {}
    virtual ~TestApp() {}
    /// Before starting, grab the wall info.
    virtual void init();
    /// Clear the buffer.
    virtual void bufferPreDraw();
    /// Main draw
    virtual void draw();

    /// Enforce meters.
    virtual float getDrawScaleFactor() { return 1.0f; }
};

#endif // INCLUDED_TestApp_h_GUID_0b548fa0_f573_4798_ad85_c04094a04c3a
