cmake_minimum_required(VERSION 3.14)

project(GameFramework
    VERSION 1.0.0
    DESCRIPTION "A 2D game framework using SDL2"
    LANGUAGES CXX)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(GAMEFRAMEWORK_INSTALL "Generate installation target" ON)
option(GAMEFRAMEWORK_USE_SYSTEM_SDL2 "Use system SDL2 if available" ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include FetchContent for downloading dependencies if needed
include(FetchContent)

# Handle SDL2 dependencies
if(GAMEFRAMEWORK_USE_SYSTEM_SDL2)
    find_package(SDL2 QUIET)
    find_package(SDL2_image QUIET)
    find_package(SDL2_mixer QUIET)
endif()

# If any of the SDL2 components are missing, fetch them all to ensure consistency
if(NOT SDL2_FOUND OR NOT SDL2_image_FOUND OR NOT SDL2_mixer_FOUND)
    set(SDL2_DISABLE_INSTALL ON CACHE BOOL "")
    set(SDL2IMAGE_INSTALL OFF CACHE BOOL "")
    set(SDL2MIXER_INSTALL OFF CACHE BOOL "")
    
    # Fetch SDL2
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG "release-2.28.5"
    )
    
    # Fetch SDL2_image
    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image.git"
        GIT_TAG "release-2.6.3"
    )
    
    # Fetch SDL2_mixer
    FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL_mixer.git"
        GIT_TAG "release-2.6.3"
    )
    
    FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_mixer)
    
    # Create our own targets for SDL2 when using FetchContent
    add_library(SDL2_Bundled INTERFACE)
    target_link_libraries(SDL2_Bundled INTERFACE SDL2)
    
    add_library(SDL2_image_Bundled INTERFACE)
    target_link_libraries(SDL2_image_Bundled INTERFACE SDL2_image)
    
    add_library(SDL2_mixer_Bundled INTERFACE)
    target_link_libraries(SDL2_mixer_Bundled INTERFACE SDL2_mixer)
endif()

# Define the library
add_library(${PROJECT_NAME}
    src/asset-manager.cpp
    src/vector2d.cpp
    src/transform.cpp
    src/sprite.cpp
)

# Add version definition
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/gameframework/version.h
    @ONLY
)

# Define alias library
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME ${PROJECT_NAME}
    OUTPUT_NAME ${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}
)

# Set include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies
if(SDL2_FOUND AND SDL2_image_FOUND AND SDL2_mixer_FOUND)
    target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            SDL2::SDL2
            SDL2_image::SDL2_image
            SDL2_mixer::SDL2_mixer
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            SDL2_Bundled
            SDL2_image_Bundled
            SDL2_mixer_Bundled
    )
endif()

# Installation
# Installation
if(GAMEFRAMEWORK_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
    
    # Install library and bundled dependencies if using FetchContent
    if(NOT SDL2_FOUND OR NOT SDL2_image_FOUND OR NOT SDL2_mixer_FOUND)
        install(TARGETS 
            ${PROJECT_NAME}
            SDL2_Bundled
            SDL2_image_Bundled
            SDL2_mixer_Bundled
            SDL2
            SDL2_image
            SDL2_mixer
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDL2
        )
        
        # Install SDL2 headers
        file(GLOB SDL2_HEADERS 
            ${SDL2_SOURCE_DIR}/include/*.h
            ${SDL2_BINARY_DIR}/include/*.h
        )
        install(FILES ${SDL2_HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDL2
        )
        
        # Install SDL2_image headers
        file(GLOB SDL2_IMAGE_HEADERS 
            ${SDL2_image_SOURCE_DIR}/include/*.h
            ${SDL2_image_BINARY_DIR}/include/*.h
        )
        install(FILES ${SDL2_IMAGE_HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDL2
        )
        
        # Install SDL2_mixer headers
        file(GLOB SDL2_MIXER_HEADERS 
            ${SDL2_mixer_SOURCE_DIR}/include/*.h
            ${SDL2_mixer_BINARY_DIR}/include/*.h
        )
        install(FILES ${SDL2_MIXER_HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDL2
        )
    else()
        install(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif()
    
    # Install headers
    install(
        FILES
            src/asset-manager.h
            src/vector2d.h
            src/transform.h
            src/sprite.h
            src/collider.h
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    )
    
    # Install generated headers
    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/include/gameframework/version.h
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    )
    
    # Export targets
    install(
        EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    
    # Generate and install config files
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    
    # Generate and install pkg-config file
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        @ONLY
    )
    
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()

