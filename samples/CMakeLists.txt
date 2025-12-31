add_executable(Sample01-Empty SampleCommon.cpp Sample01-Empty.cpp)
target_link_libraries(Sample01-Empty WodenEngine)

add_executable(Sample02-String SampleCommon.cpp Sample02-String.cpp)
target_link_libraries(Sample02-String WodenEngine)

add_executable(SceneSample01-Empty SampleCommon.cpp SceneSample01-Empty.cpp)
target_link_libraries(SceneSample01-Empty WodenEngine)

add_executable(SceneSample02-Cube SampleCommon.cpp SceneSample02-Cube.cpp)
target_link_libraries(SceneSample02-Cube WodenEngine)

add_executable(SceneSample03-TexturedCube SampleCommon.cpp SceneSample03-TexturedCube.cpp)
target_link_libraries(SceneSample03-TexturedCube WodenEngine)

add_executable(SceneSample04-TexturedCube2 SampleCommon.cpp SceneSample04-TexturedCube2.cpp)
target_link_libraries(SceneSample04-TexturedCube2 WodenEngine)
