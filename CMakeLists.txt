cmake_minimum_required(VERSION 2.8.5)
project(dtgame)

find_package(OpenGL REQUIRED)
add_library(dtgame-network STATIC
    Network/Message.cpp
    Network/Controller.cpp
)
add_executable(dtgame
    Main.cpp
    Engine.h
    Engine.cpp
    RenderingEngine.h
    RenderingEngine.cpp
    Universe.h
    Universe.cpp
    Actor.h
    Actor.cpp
    Player.h
    Player.cpp
    Ship.h
    Ship.cpp
)
add_executable(dtgame-server
    Server/Main.cpp
)
include_directories(
    ${GLFW_SOURCE_DIR}/include
    ${png_SOURCE_DIR}
    ${z_SOURCE_DIR}
)
target_link_libraries(dtgame-network
)
target_link_libraries(dtgame
    dtgame-network
    png
    z
    glfw
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
target_link_libraries(dtgame-server
    dtgame-network
)
cotire(dtgame-network)
cotire(dtgame)
cotire(dtgame-server)
