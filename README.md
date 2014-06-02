COMO: Cooperative Modeller
====

## About
COMO is a project which aims to create a Cooperative Modeller, where users could share and work on a 3D scene at the same time. Two examples where this can be useful are presented:
* An online modelling course. Teacher can chat with his/her students and teach them how to create the model while doing it. When finished, students can try to make their own models while the teacher supervises the process.
* A team working on a map for a videogame. Main designer can sketch the world while the others see it. When finished, the main designer assignes each part of the map to a junior designer, so all can work in parallel while seeing how the whole map envolves.

## Roadmap
The proyect will be developed through prototypes.

### Current status
By now, COMO allows users to have fun with meshes and simple materials :). Each user can do the following things and the others will see it:
* Creating, selecting and deleting meshes.
* Translating, rotating and scalling selections of meshes. The rotations and scales can be made relative to three different pivot points: selection centroid, individual origins and world origin.
* Editing mesh's materials (one material per mesh).
* Rotating a single directional light.

### Dependencies
COMO makes use of the following APIs and libraries:
* Qt 5.2.
* Boost 1.55 (boost_system, boost_thread and boost_filesystem).
* OpenGL 4.2 and GLSL.
* GLM.

## Instalation (Ubuntu 14.04)

### Dependencies
1. Install Qt 5.2 or above. You can dowload and use the online installer [from here](http://qt-project.org/downloads) or you can also install Qt from [this Ubuntu repository](https://launchpad.net/~ubuntu-sdk-team/+archive/ppa) issuing the following commands in a terminal.
```
sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update
sudo apt-get install qtdeclarative5-dev qtcreator
```

2. Use following commands to display your OpenGL version and check that it is 4.2 or greater.
```
sudo apt-get install mesa-utils
glxinfo | grep OpenGL
```

3. Install the rest of dependencies
```
sudo apt-get install libglm-dev libboost1.55-dev libboost-system1.55.0 libboost-filesystem1.55.0 libboost-thread1.55.0 libboost-system1.55-dev libboost-thread1.55-dev libboost-filesystem1.55-dev
```

### Building

1. Clone this repository in local:

```
	git clone git@github.com:moisesjbc/como <directory>
```

2. Enter in the previous local directory.

```
	cd <directory>
```

3. Initialize and update the repository's submodules:

```
	git submodule init
	git submodule update
```

4. Open "project/client/como_project.pro" file with Qt Creator and set the build directory to "../../bin/client".

5. Open "project/server/server.pro" file with Qt Creator and set the build directory to "../../bin/server".

6. Build both projects and enjoy!
