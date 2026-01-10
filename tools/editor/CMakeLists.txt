set(LevelEditor_Sources
    LevelEditor.cpp
)

add_executable(LevelEditor ${LevelEditor_Sources})
target_link_libraries(LevelEditor WodenEngine)