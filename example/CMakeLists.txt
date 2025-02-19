add_subdirectory(external)

add_custom_target(copy_shaders ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_target(copy_models ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/models ${CMAKE_CURRENT_BINARY_DIR}/models)
add_custom_target(copy_textures ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures ${CMAKE_CURRENT_BINARY_DIR}/textures)

add_executable(hello_triangle hello_triangle.cpp common/common.cpp common/common.h)
target_link_libraries(hello_triangle PRIVATE glfw lib_glad fwog)

add_executable(deferred deferred.cpp common/common.cpp common/common.h)
target_include_directories(deferred PUBLIC ${tinygltf_SOURCE_DIR})
target_link_libraries(deferred PRIVATE glfw lib_glad fwog glm lib_imgui)
add_dependencies(deferred copy_shaders copy_textures)

add_executable(gltf_viewer gltf_viewer.cpp common/common.cpp common/common.h common/SceneLoader.cpp common/SceneLoader.h)
target_include_directories(gltf_viewer PUBLIC ${tinygltf_SOURCE_DIR})
target_link_libraries(gltf_viewer PRIVATE glfw lib_glad fwog glm lib_imgui)
add_dependencies(gltf_viewer copy_shaders copy_models)

add_executable(volumetric volumetric.cpp common/common.cpp common/common.h common/SceneLoader.cpp common/SceneLoader.h)
target_include_directories(volumetric PUBLIC ${tinygltf_SOURCE_DIR} vendor)
target_link_libraries(volumetric PRIVATE glfw lib_glad fwog glm lib_imgui)
add_dependencies(volumetric copy_shaders copy_models copy_textures)

add_executable(gpu_driven gpu_driven.cpp common/common.cpp common/common.h common/SceneLoader.cpp common/SceneLoader.h)
target_include_directories(gpu_driven PUBLIC ${tinygltf_SOURCE_DIR} vendor)
target_link_libraries(gpu_driven PRIVATE glfw lib_glad fwog glm lib_imgui)
add_dependencies(gpu_driven copy_shaders copy_models)

if (MSVC)
    target_compile_definitions(gltf_viewer PUBLIC STBI_MSC_SECURE_CRT)
    target_compile_definitions(volumetric PUBLIC STBI_MSC_SECURE_CRT)
    target_compile_definitions(gpu_driven PUBLIC STBI_MSC_SECURE_CRT)
endif()