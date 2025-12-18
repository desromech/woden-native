set(WodenTests_Sources
    Main.cpp
    Math/Scalar.cpp
    Math/Vector2.cpp
    Math/Vector3.cpp
    Math/Vector4.cpp
)

add_executable(WodenTests ${WodenTests_Sources})
target_link_libraries(WodenTests WodenEngine UnitTest++)

add_test(NAME WodenTests COMMAND WodenTests)
