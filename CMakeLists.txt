cmake_minimum_required(VERSION 3.23)

# Config
set(ENGINE_NAME starlet_engine)
set(ENGINE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(ENGINE_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")
set(ENGINE_EXTERN "${CMAKE_CURRENT_SOURCE_DIR}/external")

if(NOT TARGET ${ENGINE_NAME})
  add_library(${ENGINE_NAME} STATIC)

  # Fetch dependencies
  include(FetchContent)

  # GLFW
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.4
  )
  FetchContent_MakeAvailable(glfw)

  # GLFW organization
  set_target_properties(glfw PROPERTIES FOLDER "External")
  if(TARGET update_mappings)
    set_target_properties(update_mappings PROPERTIES FOLDER "External")
  endif()
  if(TARGET uninstall)
    set_target_properties(uninstall PROPERTIES FOLDER "External")
  endif()

  # GLAD
  add_library(glad STATIC ${ENGINE_EXTERN}/src/glad/glad.c)
  target_include_directories(glad PUBLIC ${ENGINE_EXTERN}/inc)
  set_target_properties(glad PROPERTIES FOLDER "External")

  file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS ${ENGINE_SRC_DIR}/*.cpp)
  file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS ${ENGINE_INC_DIR}/starlet-engine/*.hpp)

  target_sources(${ENGINE_NAME} PRIVATE ${ENGINE_SRC} ${ENGINE_HEADERS})

  target_include_directories(${ENGINE_NAME} 
    PUBLIC 
      $<BUILD_INTERFACE:${ENGINE_INC_DIR}> 
      $<INSTALL_INTERFACE:include>
  )

  find_package(OpenGL REQUIRED)

  target_link_libraries(${ENGINE_NAME}
    PRIVATE
      glad 
      glfw 
      OpenGL::GL 
      starlet_math
      starlet_serializer
      starlet_scene
      starlet_graphics 
      starlet_controls
  )

  # IDE organization
  source_group(TREE ${ENGINE_SRC_DIR} PREFIX "Source Files" FILES ${ENGINE_SRC})
  source_group(TREE ${ENGINE_INC_DIR} PREFIX "Header Files" FILES ${ENGINE_HEADERS})
endif()
