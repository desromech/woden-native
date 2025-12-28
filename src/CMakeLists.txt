set(WodenEngine_Sources
    Assets/BinaryBuffer.cpp
    Assets/Font.cpp
    Assets/Image.cpp

    Morphic/Morph.cpp
    Morphic/SceneMorph.cpp
    Morphic/StringMorph.cpp
    Morphic/SystemWindow.cpp

    Physics/CollisionShape.cpp

    Rendering/Context.cpp
    Rendering/GuiRenderer.cpp
    Rendering/MeshBuilder.cpp
    Rendering/MeshPrimitive.cpp
    Rendering/Renderable.cpp
    Rendering/SceneRenderer.cpp
    Rendering/StaticMeshRenderable.cpp

    SceneGraph/Scene.cpp

    Utility/ReadWholeFile.cpp
)

add_library(WodenEngine ${WodenEngine_Sources})
target_link_libraries(WodenEngine Agpu ${WodenEngine_DEP_LIBS})