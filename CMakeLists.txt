cmake_minimum_required(VERSION 2.8.5)
project(mir)

option(SERVER_ONLY "Only build the server (non-graphical) components." OFF)

add_subdirectory(third-party/bullet)
add_subdirectory(third-party/sqlite3)
if(NOT ${SERVER_ONLY})
    add_subdirectory(third-party/freeglut)
    add_subdirectory(third-party/glew)
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
if(NOT ${SERVER_ONLY})
    find_package(OpenGL REQUIRED)
endif()
find_package(Boost COMPONENTS system REQUIRED)
link_directories(
    ${Boost_LIBRARY_DIRS}
)
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${freeglut_SOURCE_DIR}
    ${glew_SOURCE_DIR}
    ${GLFW_SOURCE_DIR}/include
    ${png_SOURCE_DIR}
    ${png_BINARY_DIR}
    ${z_SOURCE_DIR}
)
add_library(mir-network STATIC
    Network/Message.h
    Network/Message.cpp
    Network/Controller.h
    Network/Controller.cpp
    Network/IdentifiableObject.h
    Network/IdentifiableObject.cpp
    Network/SpatialObject.h
    Network/SpatialObject.cpp
    Network/Internal/tcp_server_connection.h
    Network/Internal/tcp_server_connection.cpp
    Network/Internal/tcp_server.h
    Network/Internal/tcp_server.cpp
    Network/Internal/tcp_client_connection.h
    Network/Internal/tcp_client_connection.cpp
    Network/Internal/tcp_client.h
    Network/Internal/tcp_client.cpp
    Network/Messages/CreateMessage.h
    Network/Messages/CreateMessage.cpp
    Network/ObjectTranslation.h
    Network/Messages/PlayerJoinMessage.h
    Network/Messages/PlayerJoinMessage.cpp
    Network/Messages/RequestMessage.h
    Network/Messages/RequestMessage.cpp
    Network/Messages/NotFoundMessage.h
    Network/Messages/NotFoundMessage.cpp
    Network/Messages/RepositionMessage.h
    Network/Messages/RepositionMessage.cpp
    Network/ObjectMessage.h
    Network/Source.h
    Network/UpdatableObject.h
)
if(NOT ${SERVER_ONLY})
    add_executable(mir
        Main.cpp
        Engine.h
        Engine.cpp
        CachedUniverse.h
        CachedUniverse.cpp
        CachedActor.h
        CachedActor.cpp
        CachedPlayer.h
        CachedPlayer.cpp
        CachedShip.h
        CachedShip.cpp
        CachedCelestialBody.h
        CachedPlanet.h
        CachedPlanet.cpp
        CachedStar.h
        CachedStar.cpp
        CachedAsteroid.h
        CachedAsteroid.cpp
        CachedAsteroidBelt.h
        CachedAsteroidBelt.cpp
        ClientObjectTranslation.h
        ClientObjectTranslation.cpp
        BaseState.h
        GameplayState.h
        GameplayState.cpp
        GameplayRenderingEngine.h
        GameplayRenderingEngine.cpp
        MainMenuState.h
        MainMenuState.cpp
        RenderableObject.h
        TextureManager.h
        TextureManager.cpp
    )
endif()
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
    Server/CelestialBody.h
    Server/Planet.h
    Server/Planet.cpp
    Server/Star.h
    Server/Star.cpp
    Server/Asteroid.h
    Server/Asteroid.cpp
    Server/AsteroidBelt.h
    Server/AsteroidBelt.cpp
    Server/ServerObjectTranslation.h
    Server/ServerObjectTranslation.cpp
)
set(mir-network_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Network)
set(mir-server_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Server)
auto_group(mir-network)
if(NOT ${SERVER_ONLY})
    auto_group(mir)
endif()
auto_group(mir-server)

target_link_libraries(mir-network
    ${Boost_LIBRARIES}
)
if(NOT ${SERVER_ONLY})
    target_link_libraries(mir
        mir-network
        png
        z
        glfw
        ${freeglut_LIBRARY}
        ${glew_LIBRARY}
        ${GLFW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${Boost_LIBRARIES}
    )
endif()
target_link_libraries(mir-server
    mir-network
    ${Boost_LIBRARIES}
)
if(MSVC)
    add_definitions(-D_WIN32_WINNT=0x0501)
else()
    target_link_libraries(mir-server
        pthread)
endif()
cotire(mir-network)
if(NOT ${SERVER_ONLY})
    cotire(mir)
endif()
cotire(mir-server)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Textures/Starfield.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Textures/Planet.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/)
