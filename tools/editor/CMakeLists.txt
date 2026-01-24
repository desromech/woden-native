set(LevelEditor_Sources
    LevelEditor.cpp
    LevelEditorMorph.cpp
    LevelEditorMorph.hpp
    LevelEditorSceneViewMorph.cpp
    LevelEditorSceneViewMorph.hpp
    LevelModel.cpp
    LevelModel.hpp
)

add_executable(LevelEditor ${LevelEditor_Sources})
target_link_libraries(LevelEditor WodenEngine)