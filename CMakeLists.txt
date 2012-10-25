cmake_minimum_required(VERSION 2.8.5)
project(mir)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
link_directories(
    ${Boost_LIBRARY_DIRS}
)
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${GLFW_SOURCE_DIR}/include
    ${png_SOURCE_DIR}
    ${z_SOURCE_DIR}
)
add_library(mir-network STATIC
    Network/Message.h
    Network/Message.cpp
    Network/Controller.h
    Network/Controller.cpp
    Network/IdentifiableObject.h
    Network/IdentifiableObject.cpp
    Network/Internal/tcp_server_connection.h
    Network/Internal/tcp_server_connection.cpp
    Network/Internal/tcp_server.h
    Network/Internal/tcp_server.cpp
)
add_executable(mir
    Main.cpp
    Engine.h
    Engine.cpp
    RenderingEngine.h
    RenderingEngine.cpp
    CachedUniverse.h
    CachedUniverse.cpp
    CachedActor.h
    CachedActor.cpp
    CachedPlayer.h
    CachedPlayer.cpp
    CachedShip.h
    CachedShip.cpp
    CachedSpatial.h
    CachedSpatial.cpp
)
add_executable(mir-server
    Server/Main.cpp
    Server/Universe.h
    Server/Universe.cpp
    Server/Actor.h
    Server/Actor.cpp
    Server/Player.h
    Server/Player.cpp
    Server/Ship.h
    Server/Ship.cpp
    Server/Spatial.h
    Server/Spatial.cpp
)
target_link_libraries(mir-network
    ${Boost_LIBRARIES}
)
target_link_libraries(mir
    mir-network
    png
    z
    glfw
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${Boost_LIBRARIES}
)
target_link_libraries(mir-server
    mir-network
    ${Boost_LIBRARIES}
)
if(MSVC)
    add_definitions(-D_WIN32_WINNT=0x0501)
endif()
cotire(mir-network)
cotire(mir)
cotire(mir-server)
