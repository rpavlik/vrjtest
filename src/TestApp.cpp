/**
        @file
        @brief Implementation

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

// Internal Includes
#include "TestApp.h"

// Library/third-party includes
#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Draw/OpenGL/UserData.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Projection.h>

#include <gmtl/Output.h>
#include <gmtl/VecOps.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Standard includes
#include <iostream>
#include <cassert>
#include <cmath>

using std::cout;
using std::endl;

using namespace gmtl;

static inline void glTranslate(Vec3f const &v) {
    glTranslatef(v[0], v[1], v[2]);
}
static inline void glVertex(Vec3f const &v) { glVertex3fv(v.getData()); }

/// x goes to the right, y goes up

class Nested2d {
  public:
    Nested2d(Vec3f const &x, Vec3f const &y)
        : x_(x)
        , y_(y) {}
    static Nested2d normalized(Nested2d const &other) {
        Nested2d ret(other);
        normalize(ret.x_);
        normalize(ret.y_);
        return ret;
    }

    static Nested2d normalized(Vec3f const &x, Vec3f const &y) {
        return normalized(Nested2d(x, y));
    }

    float width() const { return length(x_); }
    float height() const { return length(y_); }

    Vec3f operator()(float x, float y) const { return x_ * x + y_ * y; }

    void glVertex(float x, float y) const {
        Vec3f v = (*this)(x, y);
        glVertex3fv(v.getData());
    }

  private:
    Vec3f x_;
    Vec3f y_;
};

static inline void drawL(Nested2d const &n) {
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_LINE_STRIP);
    n.glVertex(0, 1);
    n.glVertex(0, 0);
    n.glVertex(0, 1);
    glEnd();
}

static inline void drawR(Nested2d const &n) {
    glColor3f(0.0f, 1.0f, 0.0f); // green
    glBegin(GL_LINE_STRIP);
    n.glVertex(0, 0);
    n.glVertex(0, 1);
    n.glVertex(1, 1);
    n.glVertex(1, 0.5);
    n.glVertex(0, 0.5);
    n.glVertex(1, 0);
    glEnd();
}

static inline void drawEyeLetter(Nested2d const &n, vrj::Projection::Eye eye) {
    switch (eye) {
        case vrj::Projection::LEFT:
            drawL(n);
            break;
        case vrj::Projection::RIGHT:
            drawR(n);
            break;
    }
}

static const float fontProportion = 0.2;

void TestApp::init() {
#if 0
    jccl::ConfigManager *configMgr = jccl::ConfigManager::instance();
    configMgr->lockActive();
    jccl::Configuration *config = configMgr->getActiveConfig();
#endif
    // in case of cluster:
    // config->getByType("cluster_node", elements);
}

void TestApp::bufferPreDraw() {
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void TestApp::draw() {
    glMatrixMode(GL_MODELVIEW);

    vrj::opengl::UserData *data = dynamic_cast<vrj::opengl::DrawManager *>(
                                      getDrawManager())->currentUserData();
    assert(data);
    vrj::ViewportPtr viewport = data->getViewport();
    vrj::ProjectionPtr projection = data->getProjection();
    vrj::Projection::Eye eye = projection->getEye();

    switch (eye) {
        case vrj::Projection::LEFT:
            cout << "Left" << endl;
            break;
        case vrj::Projection::RIGHT:
            cout << "Right" << endl;
            break;
    }

    Vec3f wallcenter(0, 1.8, 0);
    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 1);
    glVertex(wallcenter - Vec3f(-.01, -.01, 0));
    glVertex(wallcenter);
    glVertex(wallcenter - Vec3f(.01, .01, 0));
    glEnd();

    boost::shared_ptr<vrj::SurfaceViewport> surf(
        boost::dynamic_pointer_cast<vrj::SurfaceViewport>(viewport));
    if (surf) {
        gmtl::Point3f ll, lr, ur, ul;
        surf->getCorners(ll, lr, ur, ul);

        cout << ll << endl << lr << endl << ur << endl << ul << endl;
        Nested2d nest(lr - ll, ul - ll);
        Nested2d normNest = Nested2d::normalized(nest);
        float width = nest.width();   // length(lr - ll);
        float height = nest.height(); // length(ul - ll);
        float minDim = std::min(width, height);

        Vec3f wallcenter = (ll + lr + ur + ul) * (1.0f / 4.0f);
        cout << "Wall center: " << wallcenter << endl;
        glBegin(GL_TRIANGLES);
        glColor3f(1, 1, 1);
        glVertex(wallcenter - Vec3f(-.01, -.01, 0));
        glVertex(wallcenter);
        glVertex(wallcenter - Vec3f(.01, .01, 0));
        glEnd();
#if 0
        glPushMatrix();
        {
            const float fontSize = fontProportion * minDim;
            /// Change origin
            glTranslate(-ll);
            /// Go to middle of surface
            glTranslate(nest(0.5, 0.5));
            if (eye == vrj::Projection::LEFT) {
                /// Letter L is one font size to the left
                glTranslate(normNest(-fontSize, 0));
            }
            /// Scale letters from [0,1]
            glScalef(fontSize, fontSize, fontSize);

            glBegin(GL_POINTS);
            glColor3f(1, 1, 1);
            glVertex3f(0, 0, 0);
            glEnd();
            //                        drawEyeLetter(normNest, eye);
        }
        glPopMatrix();
#endif
    }
}
