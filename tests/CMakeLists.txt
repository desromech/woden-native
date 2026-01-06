set(WodenTests_Sources
    Main.cpp
    Math/AABox.cpp
    Math/Frustum.cpp
    Math/GJK.cpp
    Math/Matrix2x2.cpp
    Math/Matrix3x3.cpp
    Math/Matrix4x4.cpp
    Math/Quaternion.cpp
    Math/Ray3D.cpp
    Math/Scalar.cpp
    Math/Sphere.cpp
    Math/TRSTransform3D.cpp
    Math/Vector2.cpp
    Math/Vector3.cpp
    Math/Vector4.cpp
    Utility/FileSystem.cpp
)

add_executable(WodenTests ${WodenTests_Sources})
target_link_libraries(WodenTests WodenEngine UnitTest++)

add_test(NAME WodenTests COMMAND WodenTests)

set(WodenRayCastingImage_Sources
    RayCastingImage/Main.cpp
)
add_executable(WodenRayCastingImage ${WodenRayCastingImage_Sources})
target_link_libraries(WodenRayCastingImage WodenEngine)

