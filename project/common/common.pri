#-------------------------------------------------
# Project file including options and files that are common to both client and
# server.
#-------------------------------------------------

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# Common libraries
LIBS += -lpthread -lboost_system -lboost_thread -lboost_filesystem

# C++11 support (http://qt-project.org/forums/viewthread/19989)
CONFIG += c++11

INCLUDEPATH += ../../src

# C++ flags
QMAKE_CXXFLAGS_WARN_ON += -Wall -Werror
QMAKE_CXXFLAGS += -Wall -Werror -Wextra -pthread -pedantic-errors -std=c++11

# Common headers (used by both client and server).
HEADERS += \
    ../../src/common/packables/packable.hpp \
    ../../src/common/packables/composite_packable.hpp \
    ../../src/common/packets/packet.hpp \
    ../../src/common/packets/packets.hpp \
    ../../src/common/packets/new_user_packet.hpp \
    ../../src/common/packets/user_acceptance_packet.hpp \
    ../../src/common/packets/scene_update_packet.hpp \
    ../../src/common/utilities/log.hpp \
    ../../src/common/commands/command.hpp \
    ../../src/common/commands/commands.hpp \
    ../../src/common/commands/user_commands/user_command.hpp \
    ../../src/common/commands/user_commands/user_connection_command.hpp \
    ../../src/common/commands/user_commands/user_disconnection_command.hpp \
    ../../src/common/commands/drawable_commands/drawable_command.hpp \
    ../../src/common/commands/drawable_commands/drawable_selection_command.hpp \
    ../../src/common/commands/selection_commands/selection_response_command.hpp \
    ../../src/common/commands/selection_commands/selection_transformation_command.hpp \
    ../../src/common/commands/selection_commands/full_deselection_command.hpp \
    ../../src/common/commands/selection_commands/selection_command.hpp \
    ../../src/common/commands/selection_commands/selection_deletion_command.hpp \
    ../../src/common/packables/packables.hpp \
    ../../src/common/packables/packable_string.hpp \
    ../../src/common/packets/packet_header.hpp \
    ../../src/common/commands/packable_commands_list.hpp \
    ../../src/common/packables/packable_file.hpp \
    ../../src/common/commands/primitive_commands/primitive_creation_command.hpp \
    ../../src/common/packables/array/packable_array.hpp \
    ../../src/common/packables/packable_integer.hpp \
    ../../src/common/commands/primitive_commands/primitive_command.hpp \
    ../../src/common/users/user.hpp \
    ../../src/common/commands/type_command.hpp \
    ../../src/common/commands/light_commands/light_creation_command.hpp \
    ../../src/common/commands/light_commands/directional_light_creation_command.hpp \
    ../../src/common/packables/packable_wrapper.hpp \
    ../../src/common/packables/packable_float.hpp \
    ../../src/common/packables/array/packable_array_3.hpp \
    ../../src/common/packables/array/packable_color.hpp \
    ../../src/common/packables/abstract_packable_wrapper.hpp \
    ../../src/common/commands/material_commands/material_command.hpp \
    ../../src/common/commands/material_commands/material_creation_command.hpp \
    ../../src/common/commands/material_commands/material_commands.hpp \
    ../../src/common/commands/material_commands/material_modification_command.hpp \
    ../../src/common/commands/material_commands/abstract_material_modification_command.hpp \
    ../../src/common/commands/light_commands/light_command.hpp \
    ../../src/common/commands/light_commands/light_commands.hpp \
    ../../src/common/utilities/observer_pattern/observer.hpp \
    ../../src/common/utilities/observer_pattern/observable.hpp \
    ../../src/common/commands/user_commands/user_parameter_change_command.hpp \
    ../../src/common/commands/parameter_change_command.hpp \
    ../../src/common/commands/light_commands/light_color_change_command.hpp \
    ../../src/common/utilities/observer_pattern/observable_container.hpp \
    ../../src/common/utilities/observer_pattern/container_observer.hpp \
    ../../src/common/utilities/paths.hpp \
    ../../src/common/commands/primitive_category_commands/primitive_category_command.hpp \
    ../../src/common/commands/primitive_category_commands/primitive_category_creation_command.hpp \
    ../../src/common/commands/primitive_category_commands/primitive_category_commands.hpp \
    ../../src/common/commands/primitive_commands/primitive_commands.hpp \
    ../../src/common/managers/primitives/abstract_primitives_manager.hpp \
    ../../src/common/ids/resource_id.hpp \
    ../../src/common/ids/user_id.hpp \
    ../../src/common/packables/ids/packable_resource_id.hpp \
    ../../src/common/commands/primitive_commands/primitive_instantiation_command.hpp \
    ../../src/common/primitives/primitives_importer.hpp \
    ../../src/common/primitives/obj_primitives_importer.hpp \
    ../../src/common/exceptions/file_not_open_exception.hpp \
    ../../src/common/primitives/primitive_info.hpp \
    ../../src/common/packables/conditional_packable_wrapper.hpp \
    ../../src/common/primitives/primitive_file.hpp \
    ../../src/common/mesh_info/mesh_vertex_data.hpp \
    ../../src/common/mesh_info/mesh_normal_data.hpp \
    ../../src/common/mesh_info/mesh_texture_data.hpp \
    ../../src/common/mesh_info/mesh_info.hpp \
    ../../src/common/mesh_info/mesh_opengl_data.hpp \
    ../../src/common/mesh_info/material_info.hpp \
    ../../src/common/mesh_info/texture_info.hpp \
    ../../src/common/scene/basic_scene.hpp \
    ../../src/common/mesh_info/polygon_group_data.hpp \
    ../../src/common/commands/light_commands/light_ambient_coefficient_change_command.hpp \
    ../../src/common/commands/resource_commands/resource_command.hpp \
    ../../src/common/commands/resources_selection_commands/resources_selection_command.hpp


# Common sources (used by both client and server).
SOURCES += \
    ../../src/common/packables/composite_packable.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/new_user_packet.cpp \
    ../../src/common/packets/user_acceptance_packet.cpp \
    ../../src/common/packets/scene_update_packet.cpp \
    ../../src/common/commands/command.cpp \
    ../../src/common/commands/user_commands/user_command.cpp \
    ../../src/common/commands/user_commands/user_connection_command.cpp \
    ../../src/common/commands/user_commands/user_disconnection_command.cpp \
    ../../src/common/commands/drawable_commands/drawable_command.cpp \
    ../../src/common/commands/drawable_commands/drawable_selection_command.cpp \
    ../../src/common/commands/selection_commands/selection_response_command.cpp \
    ../../src/common/commands/selection_commands/selection_transformation_command.cpp \
    ../../src/common/commands/selection_commands/full_deselection_command.cpp \
    ../../src/common/commands/selection_commands/selection_command.cpp \
    ../../src/common/commands/selection_commands/selection_deletion_command.cpp \
    ../../src/common/packets/packet_header.cpp \
    ../../src/common/commands/packable_commands_list.cpp \
    ../../src/common/packables/packable_file.cpp \
    ../../src/common/commands/primitive_commands/primitive_creation_command.cpp \
    ../../src/common/commands/primitive_commands/primitive_command.cpp \
    ../../src/common/users/user.cpp \
    ../../src/common/commands/light_commands/light_creation_command.cpp \
    ../../src/common/commands/light_commands/directional_light_creation_command.cpp \
    ../../src/common/packables/packable_float.cpp \
    ../../src/common/packables/array/packable_color.cpp \
    ../../src/common/commands/material_commands/material_command.cpp \
    ../../src/common/commands/material_commands/material_creation_command.cpp \
    ../../src/common/commands/material_commands/abstract_material_modification_command.cpp \
    ../../src/common/commands/light_commands/light_command.cpp \
    ../../src/common/utilities/observer_pattern/observable.cpp \
    ../../src/common/commands/user_commands/user_parameter_change_command.cpp \
    ../../src/common/commands/light_commands/light_color_change_command.cpp \
    ../../src/common/commands/primitive_category_commands/primitive_category_command.cpp \
    ../../src/common/commands/primitive_category_commands/primitive_category_creation_command.cpp \
    ../../src/common/managers/primitives/abstract_primitives_manager.cpp \
    ../../src/common/ids/resource_id.cpp \
    ../../src/common/packables/ids/packable_resource_id.cpp \
    ../../src/common/commands/primitive_commands/primitive_instantiation_command.cpp \
    ../../src/common/primitives/obj_primitives_importer.cpp \
    ../../src/common/exceptions/file_not_open_exception.cpp \
    ../../src/common/primitives/primitive_info.cpp \
    ../../src/common/primitives/primitive_file.cpp \
    ../../src/common/scene/basic_scene.cpp \
    ../../src/common/commands/light_commands/light_ambient_coefficient_change_command.cpp \
    ../../src/common/packables/packable_string.cpp \
    ../../src/common/commands/resource_commands/resource_command.cpp \
    ../../src/common/commands/resources_selection_commands/resources_selection_command.cpp
