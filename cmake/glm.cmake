option(BUILD_TESTING OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/glm)

set_target_properties(glm PROPERTIES FOLDER 3rdparty) # Override standard 'GLM' subfolder

set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/glm/glm)
set(GLM_LIBRARIES ${GLM_LIBRARIES} glm)
