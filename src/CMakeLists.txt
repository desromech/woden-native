set(WodenEngine_Sources
    Assets/BinaryBuffer.cpp
    Assets/Font.cpp
    Assets/Image.cpp
    Assets/PixelFormat.cpp
    Assets/ResourceCache.cpp
    Assets/Texture.cpp

    Math/Matrix3x3.cpp
    Math/Vector3.cpp

    Morphic/Event.cpp
    Morphic/Morph.cpp
    Morphic/SceneMorph.cpp
    Morphic/StringMorph.cpp
    Morphic/SystemWindow.cpp

    Physics/CollisionObject.cpp
    Physics/CollisionShape.cpp
    Physics/PhysicsWorld.cpp
    Physics/RigidBody.cpp

    Rendering/Context.cpp
    Rendering/GuiRenderer.cpp
    Rendering/LightSource.cpp
    Rendering/Material.cpp
    Rendering/MeshBuilder.cpp
    Rendering/MeshPrimitive.cpp
    Rendering/MetallicRoughnessMaterial.cpp
    Rendering/Renderable.cpp
    Rendering/RenderingScene.cpp
    Rendering/SceneRenderer.cpp
    Rendering/StaticMeshRenderable.cpp
    Rendering/VertexBinding.cpp

    SceneGraph/Scene.cpp

    Utility/FileSystem.cpp
    Utility/ReadWholeFile.cpp
    Utility/ShaderPreprocessor.cpp
)

add_library(WodenEngine ${WodenEngine_Sources})
target_link_libraries(WodenEngine Agpu ${WodenEngine_DEP_LIBS})