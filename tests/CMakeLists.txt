set(WodenTests_Sources
    Main.cpp
    Math/Vector2.cpp
)

add_executable(WodenTests ${WodenTests_Sources})
target_link_libraries(WodenTests UnitTest++)

add_test(NAME WodenTests COMMAND WodenTests)
