set(WodenEngine_Sources
    Assets/Image.cpp
    Morphic/Morph.cpp
    Morphic/SystemWindow.cpp
    Physics/CollisionShape.cpp
)

add_library(WodenEngine ${WodenEngine_Sources})
target_link_libraries(WodenEngine ${WodenEngine_DEP_LIBS})