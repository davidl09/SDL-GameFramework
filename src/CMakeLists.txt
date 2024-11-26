# Source files
set(SOURCES
    asset-manager.cpp
    vector2d.cpp
    transform.cpp
    sprite.cpp
    game.cpp
    scene.cpp
    gameobject.cpp
    collider.cpp
    mouse.cpp
    keyboard.cpp
    audiomanager.cpp
)

# Header files
set(HEADERS
    asset-manager.h
    vector2d.h
    transform.h
    sprite.h
    game.h
    scene.h
    gameobject.h
    collider.h
    mouse.h
    keyboard.h
    audiomanager.h
    animation.h
    camera.h
    debug/debug_logger.h
    ui/ui_element.h
    ui/ui_button.h
    ui/ui_text.h
)

# Create library
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        ${SDL2_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
)

# Set C++ standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)