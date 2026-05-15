set(WodenEngine_Sources
    Assets/BinaryBuffer.cpp
    Assets/Font.cpp
    Assets/Image.cpp
    Assets/PixelFormat.cpp
    Assets/QMapFile.cpp
    Assets/ResourceCache.cpp
    Assets/ResourceLoadingContext.cpp
    Assets/Texture.cpp

    Events/Events.cpp

    GameFramework/Actor.cpp
    GameFramework/ActorComponent.cpp
    GameFramework/ActorFactory.cpp
    GameFramework/ActorSceneComponents.cpp
    GameFramework/ActorTickSubsystem.cpp
    GameFramework/CharacterActor.cpp
    GameFramework/CollisionObjectComponents.cpp
    GameFramework/CollisionShapeComponents.cpp
    GameFramework/InputComponents.cpp
    GameFramework/InputSubsystem.cpp
    GameFramework/PhysicsSubsystem.cpp
    GameFramework/PlayerActor.cpp
    GameFramework/SceneSubsystem.cpp
    GameFramework/World.cpp

    Math/GJK.cpp
    Math/Matrix3x3.cpp
    Math/Vector3.cpp

    
    Morphic/BorderedMorph.cpp
    Morphic/FileBrowser.cpp
    Morphic/MenuBarMorph.cpp
    Morphic/Morph.cpp
    Morphic/RootMorph.cpp
    Morphic/Layout.cpp
    Morphic/SceneMorph.cpp
    Morphic/SimpleButtonMorph.cpp
    Morphic/StringMorph.cpp
    Morphic/SystemWindow.cpp
    Morphic/TableMorph.cpp
    Morphic/TextInputMorph.cpp
    Morphic/WorldMorph.cpp

    Physics/CharacterRigidBody.cpp
    Physics/CollisionObject.cpp
    Physics/CollisionShape.cpp
    Physics/ContactManifold.cpp
    Physics/ContactPoint.cpp
    Physics/PhysicsWorld.cpp
    Physics/RigidBody.cpp

    Rendering/Context.cpp
    Rendering/GuiRenderer.cpp
    Rendering/InfiniteGrid.cpp
    Rendering/LightSource.cpp
    Rendering/Material.cpp
    Rendering/MeshBuilder.cpp
    Rendering/MeshPrimitive.cpp
    Rendering/MetallicRoughnessMaterial.cpp
    Rendering/Renderable.cpp
    Rendering/RenderingScene.cpp
    Rendering/SceneRenderer.cpp
    Rendering/StaticMeshRenderable.cpp
    Rendering/SurfaceMaterial.cpp
    Rendering/VertexBinding.cpp

    SceneGraph/Scene.cpp

    Utility/FileSystem.cpp
    Utility/ReadWholeFile.cpp
    Utility/ShaderPreprocessor.cpp
    Utility/Time.cpp
)

add_library(WodenEngine ${WodenEngine_Sources})
target_link_libraries(WodenEngine Agpu ${WodenEngine_DEP_LIBS})