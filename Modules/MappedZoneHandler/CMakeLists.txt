include_directories(${PROJECT_SOURCE_DIR})

add_library(Utilities_MappedZoneHandler MODULE 
  MappedZoneHandler.cpp
  MappedZoneHandlerLuaSupport.cpp
  MappedZoneHandlers.cpp
)
set_target_properties(Utilities_MappedZoneHandler PROPERTIES OUTPUT_NAME "Module")
set_target_properties(Utilities_MappedZoneHandler PROPERTIES PREFIX "lib")

