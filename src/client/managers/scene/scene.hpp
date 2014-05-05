/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <map>
#include <list>
#include "../../models/3d/camera.hpp"
#include "../../models/users/client_user.hpp"
#include "../../../common/utilities/log.hpp"
#include "../../../common/commands/commands.hpp"
#include "../server_interface/server_interface.hpp"
#include "../../models/utilities/msl/src/shader_loader.hpp"
#include "../../managers/drawables_selection/drawables_selection.hpp"
#include <queue>
#include "../../models/3d/lights/lights.hpp"

Q_DECLARE_METATYPE( como::CommandConstPtr )

namespace como {

enum LinesBufferOffset {
    WORLD_AXIS = 0,
    GUIDE_AXIS,
    TRANSFORM_GUIDE_LINE,

    N_LINES_BUFFER_OFFSETS
};

typedef std::map< UserID, PublicUserPtr > UsersMap;

class Scene : public QOffscreenSurface
{
    Q_OBJECT

    protected:
        // Scene name
        char sceneName_[NAME_SIZE];

        // Users sharing this scene.
        UsersMap users_;

        // Log
        LogPtr log_;

        // Local user's ID.
        UserID localUserID_;
        DrawableIndex localUserNextDrawableIndex_;

        shared_ptr< QOpenGLContext > oglContext_;

        shared_ptr< LightingManager > lightingManager_;

        // Interface with the server.
        ServerInterface server_;

    private:
        // Scene's non selected drawables.
        DrawablesSelection nonSelectedDrawables;

        // Lines VAO, VBO and offsets.
        GLuint linesVAO;
        GLuint linesVBO;
        GLuint linesBufferOffsets[N_LINES_BUFFER_OFFSETS];

        GLuint uniformColorLocation;
        GLfloat defaultContourColor[4];


    public:
        /***
         * 1. Construction.
         ***/
        Scene() = delete;
        Scene( const Scene&  ) = delete;
        Scene( Scene&& ) = delete;
        Scene( LogPtr log );


        /***
         * 2. Destruction
         ***/
        virtual ~Scene();


        /***
         * 3. Initialization
         ***/

        void initOpenGL();
        void initLinesBuffer();
        bool connect( const char* host, const char* port, const char* userName );


        /***
         * 4. Users administration
         ***/
        void addUser( std::shared_ptr< const UserConnectionCommand > userConnectedCommand );
        void removeUser( UserID userID );


        /***
         * 5. Getters
         ***/

        glm::vec3 getPivotPoint() const ;
        glm::vec3 getPivotPoint( UserID userID ) const ;
        shared_ptr< QOpenGLContext > getOpenGLContext() const ;



        /***
         * 6. Setters
         ***/
    public:
        void setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a );
        void setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny );

        void setDirectionalLightColor( glm::vec3 color );

    protected:
        void setName( const char* sceneName );


        /***
         * 7. Drawables administration
         ***/
    private:
        void takeOpenGLContext();
    public:

        /***
         * 10. Drawing
         ***/
        void draw( const glm::mat4& viewProjMatrix, const int& drawGuideRect = -1 ) const ;
        void drawWorldAxis() const ;
        void drawTransformGuideLine() const ;


        /***
         * 11. Operators
         ***/
        Scene& operator=( const Scene& ) = delete ;
        Scene& operator=( Scene&& ) = delete;


        /***
         * 12. Signals
         ***/
        void emitRenderNeeded();
    signals:
        void renderNeeded();
        void userConnected( UserConnectionCommandConstPtr command );
        void userDisconnected( UserID userID );
        void connectedToScene( const QString& sceneName );
        void primitiveAdded( const QString& primitiveName, PrimitiveID primitiveID );


        /***
         * 13. Slots
         ***/
    private:
        void executeRemoteUserCommand( UserCommandConstPtr command );
        void executeRemoteDrawableCommand( DrawableCommandConstPtr command );
        void executeRemoteSelectionCommand( SelectionCommandConstPtr command );
        void executeRemotePrimitiveCommand( PrimitiveCommandConstPtr command );

    public slots:
        void executeRemoteCommand( CommandConstPtr command );
        void setAmbientLight( glm::vec3 ambientLight );

        /***
         * 14. Auxiliar methods
         ***/
    public:
        void createScenePrimitivesDirectory();
};

typedef std::shared_ptr< Scene > ScenePtr;

} // namespace como

#endif // SCENE_HPP
