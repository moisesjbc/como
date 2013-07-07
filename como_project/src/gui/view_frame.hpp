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

#ifndef VIEW_FRAME_H
#define VIEW_FRAME_H

#include <QFrame>
#include <QGraphicsView>
#include <QGLContext>
#include "opengl_canvas.hpp"
#include "../3d_entities/scene.hpp"

class ViewFrame : public QFrame
{
    private:
        //QGraphicsView *graphicsView;
        OpenGLCanvas* openGLCanvas;

    public:
        ViewFrame( const QString &name, shared_ptr<QOpenGLContext> glContext, shared_ptr<Scene> scene );
};

#endif // VIEW_FRAME_H
