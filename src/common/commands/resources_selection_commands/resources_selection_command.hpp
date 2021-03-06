#ifndef RESOURCES_SELECTION_COMMAND_HPP
#define RESOURCES_SELECTION_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class ResourcesSelectionCommandType : std::uint8_t {
    SELECTION_UNLOCK = 0,
    SELECTION_DELETION
};

// TODO: Make constructors protected and inherit specialized commands.
class ResourcesSelectionCommand : public TypeCommand< ResourcesSelectionCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        ResourcesSelectionCommand( ResourcesSelectionCommandType commandType );
        ResourcesSelectionCommand( ResourcesSelectionCommandType commandType, UserID userID );
        ResourcesSelectionCommand() = delete;
        ResourcesSelectionCommand( const ResourcesSelectionCommand& ) = default;
        ResourcesSelectionCommand( ResourcesSelectionCommand&& ) = delete;
        COMMAND_CLONE_METHOD( ResourcesSelectionCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~ResourcesSelectionCommand() = default;


        /***
         * 3. Operators
         ***/
        ResourcesSelectionCommand& operator = ( const ResourcesSelectionCommand& ) = delete;
        ResourcesSelectionCommand& operator = ( ResourcesSelectionCommand&& ) = delete;
};

} // namespace como

#endif // RESOURCES_SELECTION_COMMAND_HPP
