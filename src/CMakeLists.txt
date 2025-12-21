set(WodenEngine_Sources
    Assets/Image.cpp
    Morphic/Morph.cpp
    Morphic/StringMorph.cpp
    Morphic/SystemWindow.cpp
    Physics/CollisionShape.cpp
    Rendering/Context.cpp
    Rendering/GuiRenderer.cpp
    Utility/ReadWholeFile.cpp
)

add_library(WodenEngine ${WodenEngine_Sources})
target_link_libraries(WodenEngine Agpu ${WodenEngine_DEP_LIBS})