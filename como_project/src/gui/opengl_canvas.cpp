/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "opengl_canvas.hpp"
#include <iostream>
using namespace std;

/***
 * 1. Initialization and destruction
 ***/

OpenGLCanvas::OpenGLCanvas(QWidget *parent) :
    QGLWidget(parent)
{
    scene = NULL;
    setFocusPolicy( Qt::StrongFocus );

    setAcceptDrops( true );
}

OpenGLCanvas::OpenGLCanvas( QGLContext * context, Scene* scene, QWidget *parent ) :
    QGLWidget( context, parent )
{
    cout << "OpenGLCanvas created" << endl;
    this->scene = scene;

    setFocusPolicy( Qt::StrongFocus );

    setAcceptDrops( true );
}


OpenGLCanvas::~OpenGLCanvas()
{
    //delete scene;
}


void OpenGLCanvas::initializeGL()
{
    // Set clear color.
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    // Set OpenGL depth test.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
}


/***
 * 2. Events
 ***/

void OpenGLCanvas::keyPressEvent( QKeyEvent *e )
{
    cout << "Key press event" << endl;
  switch ( e->key() )
  {
    case Qt::Key_Left:
      scene->translateSelectedDrawables( -0.01f, 0.0f, 0.0f );
    break;
    case Qt::Key_Right:
      scene->translateSelectedDrawables( 0.01f, 0.0f, 0.0f );
    break;
    default:
      cout << "Unknown key (" << e->key() << ")" << endl;
    break;
  }
  updateGL();
}


void OpenGLCanvas::dragMoveEvent(QDragMoveEvent *dragMoveEvent )
{
    // The event needs to be accepted here
    cout << "Drag move event accepted" << endl;
    dragMoveEvent->accept();
}

void OpenGLCanvas::mouseMoveEvent( QMouseEvent *mouseMoveEvent )
{
    mouseMoveEvent->accept();
    cout << "Mouse move event (" << mouseMoveEvent->x() << ", " << mouseMoveEvent->y() << ")!" << endl;
}

/***
 * 3. Updating and drawing
 ***/

void OpenGLCanvas::paintGL()
{
    cout << "Drawing GL" << endl;
    glViewport( 0, 0, width(), height() );

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw geometries.
    //glViewport( 0, 0, width()/2, height()/2 );
    scene->draw();

    /*glViewport( width()/2, height()/2, width()/2, height()/2 );
    scene->draw();*/

    // Flush.
    glFlush();
}


void OpenGLCanvas::resizeGL( int w, int h )
{
    cout << "Resizing" << endl;

    // Viewport occuppies the full canvas.
    glViewport( 0, 0, w, h );
    /*
    // Set a orthographic projection cube.
    // http://stackoverflow.com/questions/2571402/c-opengl-glortho-please-explain-the-usage-of-this-command
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);

    // Initialize modelview matrix to identity one.
    // TODO: is this necessary?
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    */
}
