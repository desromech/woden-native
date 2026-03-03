set(MapPlayer_Sources
    MapPlayer.cpp
)

add_executable(MapPlayer ${MapPlayer_Sources})
target_link_libraries(MapPlayer WodenEngine)