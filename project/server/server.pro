TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/common/packets/user_accepted.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/packable.cpp \
    ../../src/common/packets/new_user.cpp \
    ../../src/common/packets/scene_update.cpp \
    ../../src/common/packets/scene_commands/scene_command.cpp \
    ../../src/common/utilities/packer.cpp \
    ../../src/common/packets/scene_commands/user_connected.cpp \
    ../../src/server/public_user.cpp \
    ../../src/common/packets/packet_communication.cpp

LIBS += -lpthread

HEADERS += \
    ../../src/server/server.hpp \
    ../../src/common/packets/user_accepted.hpp \
    ../../src/common/packets/packet.hpp \
    ../../src/common/packets/packable.hpp \
    ../../src/common/packets/new_user.hpp \
    ../../src/common/packets/packets.hpp \
    ../../src/common/packets/scene_update.hpp \
    ../../src/common/packets/scene_commands/scene_command.hpp \
    ../../src/common/packets/scene_commands/scene_commands.hpp \
    ../../src/common/utilities/packer.hpp \
    ../../src/common/packets/scene_commands/user_connected.hpp \
    ../../src/server/public_user.hpp \
    ../../src/common/packets/packet_communication.hpp

# Link boost libraries
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
