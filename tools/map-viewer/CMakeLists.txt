set(MapViewer_Sources
    MapViewer.cpp
)

add_executable(MapViewer ${MapViewer_Sources})
target_link_libraries(MapViewer WodenEngine)