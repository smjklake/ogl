# CMake entry point
cmake_minimum_required(VERSION 3.10)
project(Tutorials)

# Set OpenGL preference to avoid compatibility warnings
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)


if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)")
endif ()
if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()
if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()


# Compile external dependencies
add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if (INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif (INCLUDE_DISTRIB)


include_directories(
        external/AntTweakBar-1.16/include/
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        .
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

# Tutorial 1
add_executable(tutorial01_first_window
        examples/tutorial01_first_window/tutorial01.cpp
)
target_link_libraries(tutorial01_first_window
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial01_first_window PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial01_first_window/")
create_target_launcher(tutorial01_first_window WORKING_DIRECTORY "examples/tutorial01_first_window/")

# Tutorial 2
add_executable(tutorial02_red_triangle
        examples/tutorial02_red_triangle/tutorial02.cpp
        common/shader.cpp
        common/shader.hpp

        examples/tutorial02_red_triangle/SimpleFragmentShader.fragmentshader
        examples/tutorial02_red_triangle/SimpleVertexShader.vertexshader
)
target_link_libraries(tutorial02_red_triangle
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial02_red_triangle PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial02_red_triangle/")
create_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "examples/tutorial02_red_triangle/")
create_default_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "examples/tutorial02_red_triangle/") # tut 1 is not the default or people would complain that tut 2 doesn't work

# Tutorial 3
add_executable(tutorial03_matrices
        examples/tutorial03_matrices/tutorial03.cpp
        common/shader.cpp
        common/shader.hpp

        examples/tutorial03_matrices/SimpleTransform.vertexshader
        examples/tutorial03_matrices/SingleColor.fragmentshader
)
#set_target_properties(tutorial03_matrices PROPERTIES RUNTIME_OUTPUT_DIRECTORY /test1)
target_link_libraries(tutorial03_matrices
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial03_matrices PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial03_matrices/")
create_target_launcher(tutorial03_matrices WORKING_DIRECTORY "examples/tutorial03_matrices/") # Visual

# Tutorial 4
add_executable(tutorial04_colored_cube
        examples/tutorial04_colored_cube/tutorial04.cpp
        common/shader.cpp
        common/shader.hpp

        examples/tutorial04_colored_cube/TransformVertexShader.vertexshader
        examples/tutorial04_colored_cube/ColorFragmentShader.fragmentshader
)
target_link_libraries(tutorial04_colored_cube
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial04_colored_cube PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial04_colored_cube/")
create_target_launcher(tutorial04_colored_cube WORKING_DIRECTORY "examples/tutorial04_colored_cube/")

# Tutorial 5
add_executable(tutorial05_textured_cube
        examples/tutorial05_textured_cube/tutorial05.cpp
        common/shader.cpp
        common/shader.hpp
        common/texture.cpp
        common/texture.hpp

        examples/tutorial05_textured_cube/TransformVertexShader.vertexshader
        examples/tutorial05_textured_cube/TextureFragmentShader.fragmentshader
)
target_link_libraries(tutorial05_textured_cube
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial05_textured_cube PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial05_textured_cube/")
create_target_launcher(tutorial05_textured_cube WORKING_DIRECTORY "examples/tutorial05_textured_cube/")

# Tutorial 6
add_executable(tutorial06_keyboard_and_mouse
        examples/tutorial06_keyboard_and_mouse/tutorial06.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp

        examples/tutorial06_keyboard_and_mouse/TransformVertexShader.vertexshader
        examples/tutorial06_keyboard_and_mouse/TextureFragmentShader.fragmentshader
)
target_link_libraries(tutorial06_keyboard_and_mouse
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial06_keyboard_and_mouse PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial06_keyboard_and_mouse/")
create_target_launcher(tutorial06_keyboard_and_mouse WORKING_DIRECTORY "examples/tutorial06_keyboard_and_mouse/")

# Tutorial 7
add_executable(tutorial07_model_loading
        examples/tutorial07_model_loading/tutorial07.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp

        examples/tutorial07_model_loading/TransformVertexShader.vertexshader
        examples/tutorial07_model_loading/TextureFragmentShader.fragmentshader
)
target_link_libraries(tutorial07_model_loading
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial07_model_loading PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial07_model_loading/")
create_target_launcher(tutorial07_model_loading WORKING_DIRECTORY "examples/tutorial07_model_loading/")

# Tutorial 8
add_executable(tutorial08_basic_shading
        examples/tutorial08_basic_shading/tutorial08.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp

        examples/tutorial08_basic_shading/StandardShading.vertexshader
        examples/tutorial08_basic_shading/StandardShading.fragmentshader
)
target_link_libraries(tutorial08_basic_shading
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial08_basic_shading PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial08_basic_shading/")
create_target_launcher(tutorial08_basic_shading WORKING_DIRECTORY "examples/tutorial08_basic_shading/")

# Tutorial 9
add_executable(tutorial09_vbo_indexing
        examples/tutorial09_vbo_indexing/tutorial09.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial09_vbo_indexing/StandardShading.vertexshader
        examples/tutorial09_vbo_indexing/StandardShading.fragmentshader
)
target_link_libraries(tutorial09_vbo_indexing
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial09_vbo_indexing PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial09_vbo_indexing/")
create_target_launcher(tutorial09_vbo_indexing WORKING_DIRECTORY "examples/tutorial09_vbo_indexing/")

# Tutorial 9 - AssImp model loading
add_executable(tutorial09_AssImp
        examples/tutorial09_vbo_indexing/tutorial09_AssImp.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp

        examples/tutorial09_vbo_indexing/StandardShading.vertexshader
        examples/tutorial09_vbo_indexing/StandardShading.fragmentshader
)
target_link_libraries(tutorial09_AssImp
        ${ALL_LIBS}
        assimp
)
set_target_properties(tutorial09_AssImp PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")
# Xcode and Visual working directories
set_target_properties(tutorial09_AssImp PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial09_vbo_indexing/")
create_target_launcher(tutorial09_AssImp WORKING_DIRECTORY "examples/tutorial09_vbo_indexing/")

# Tutorial 9 - several objects
add_executable(tutorial09_several_objects
        examples/tutorial09_vbo_indexing/tutorial09_several_objects.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial09_vbo_indexing/StandardShading.vertexshader
        examples/tutorial09_vbo_indexing/StandardShading.fragmentshader
)
target_link_libraries(tutorial09_several_objects
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial09_several_objects PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial09_vbo_indexing/")
create_target_launcher(tutorial09_several_objects WORKING_DIRECTORY "examples/tutorial09_vbo_indexing/")

# Tutorial 10
add_executable(tutorial10_transparency
        examples/tutorial10_transparency/tutorial10.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial10_transparency/StandardShading.vertexshader
        examples/tutorial10_transparency/StandardTransparentShading.fragmentshader
)
target_link_libraries(tutorial10_transparency
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial10_transparency PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial10_transparency/")
create_target_launcher(tutorial10_transparency WORKING_DIRECTORY "examples/tutorial10_transparency/")

# Tutorial 11
add_executable(tutorial11_2d_fonts
        examples/tutorial11_2d_fonts/tutorial11.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/text2D.hpp
        common/text2D.cpp

        examples/tutorial11_2d_fonts/StandardShading.vertexshader
        examples/tutorial11_2d_fonts/StandardShading.fragmentshader
        examples/tutorial11_2d_fonts/TextVertexShader.vertexshader
        examples/tutorial11_2d_fonts/TextVertexShader.fragmentshader

)
target_link_libraries(tutorial11_2d_fonts
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial11_2d_fonts PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial11_2d_fonts/")
create_target_launcher(tutorial11_2d_fonts WORKING_DIRECTORY "examples/tutorial11_2d_fonts/")

# Tutorial 12
add_executable(tutorial12_extensions
        examples/tutorial12_extensions/tutorial12.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial12_extensions/StandardShading.vertexshader
        examples/tutorial12_extensions/StandardShading_WithSyntaxErrors.fragmentshader
)
target_link_libraries(tutorial12_extensions
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial12_extensions PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial12_extensions/")
create_target_launcher(tutorial12_extensions WORKING_DIRECTORY "examples/tutorial12_extensions/")

# Tutorial 13
add_executable(tutorial13_normal_mapping
        examples/tutorial13_normal_mapping/tutorial13.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/text2D.hpp
        common/text2D.cpp
        common/tangentspace.hpp
        common/tangentspace.cpp

        examples/tutorial13_normal_mapping/NormalMapping.vertexshader
        examples/tutorial13_normal_mapping/NormalMapping.fragmentshader
)
target_link_libraries(tutorial13_normal_mapping
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial13_normal_mapping PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial13_normal_mapping/")
create_target_launcher(tutorial13_normal_mapping WORKING_DIRECTORY "examples/tutorial13_normal_mapping/")


# Tutorial 14
add_executable(tutorial14_render_to_texture
        examples/tutorial14_render_to_texture/tutorial14.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/text2D.hpp
        common/text2D.cpp

        examples/tutorial14_render_to_texture/StandardShadingRTT.vertexshader
        examples/tutorial14_render_to_texture/StandardShadingRTT.fragmentshader
        examples/tutorial14_render_to_texture/Passthrough.vertexshader
        examples/tutorial14_render_to_texture/WobblyTexture.fragmentshader
)
target_link_libraries(tutorial14_render_to_texture
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial14_render_to_texture PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial14_render_to_texture/")
create_target_launcher(tutorial14_render_to_texture WORKING_DIRECTORY "examples/tutorial14_render_to_texture/")


# Tutorial 15
add_executable(tutorial15_lightmaps
        examples/tutorial15_lightmaps/tutorial15.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial15_lightmaps/TransformVertexShader.vertexshader
        examples/tutorial15_lightmaps/TextureFragmentShaderLOD.fragmentshader
)
target_link_libraries(tutorial15_lightmaps
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial15_lightmaps PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial15_lightmaps/")
create_target_launcher(tutorial15_lightmaps WORKING_DIRECTORY "examples/tutorial15_lightmaps/")

# Tutorial 16, simple version
add_executable(tutorial16_shadowmaps_simple
        examples/tutorial16_shadowmaps/tutorial16_SimpleVersion.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial16_shadowmaps/ShadowMapping_SimpleVersion.vertexshader
        examples/tutorial16_shadowmaps/ShadowMapping_SimpleVersion.fragmentshader
        examples/tutorial16_shadowmaps/DepthRTT.vertexshader
        examples/tutorial16_shadowmaps/DepthRTT.fragmentshader
)
target_link_libraries(tutorial16_shadowmaps_simple
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial16_shadowmaps_simple PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial16_shadowmaps/")
create_target_launcher(tutorial16_shadowmaps_simple WORKING_DIRECTORY "examples/tutorial16_shadowmaps/")


# Tutorial 16
add_executable(tutorial16_shadowmaps
        examples/tutorial16_shadowmaps/tutorial16.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/tutorial16_shadowmaps/ShadowMapping.vertexshader
        examples/tutorial16_shadowmaps/ShadowMapping.fragmentshader
        examples/tutorial16_shadowmaps/DepthRTT.vertexshader
        examples/tutorial16_shadowmaps/DepthRTT.fragmentshader
        examples/tutorial16_shadowmaps/Passthrough.vertexshader
        examples/tutorial16_shadowmaps/SimpleTexture.fragmentshader
)
target_link_libraries(tutorial16_shadowmaps
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial16_shadowmaps PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial16_shadowmaps/")
create_target_launcher(tutorial16_shadowmaps WORKING_DIRECTORY "examples/tutorial16_shadowmaps/")

# Tutorial 17
add_executable(tutorial17_rotations
        examples/tutorial17_rotations/tutorial17.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/quaternion_utils.cpp
        common/quaternion_utils.hpp

        examples/tutorial17_rotations/StandardShading.vertexshader
        examples/tutorial17_rotations/StandardShading.fragmentshader
)
target_link_libraries(tutorial17_rotations
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
)
# Xcode and Visual working directories
set_target_properties(tutorial17_rotations PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial17_rotations/")
create_target_launcher(tutorial17_rotations WORKING_DIRECTORY "examples/tutorial17_rotations/")

# User playground
add_executable(playground
        src/main.cpp
        common/shader.cpp
        common/shader.hpp
)
target_link_libraries(playground
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(playground PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
create_target_launcher(playground WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/")


# Misc 5, with glReadPixels
add_executable(misc05_picking_slow_easy
        examples/misc05_picking/misc05_picking_slow_easy.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/misc05_picking/StandardShading.vertexshader
        examples/misc05_picking/StandardShading.fragmentshader
        examples/misc05_picking/Picking.vertexshader
        examples/misc05_picking/Picking.fragmentshader
)
target_link_libraries(misc05_picking_slow_easy
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
)
# Xcode and Visual working directories
set_target_properties(misc05_picking_slow_easy PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/misc05_picking/")
create_target_launcher(misc05_picking_slow_easy WORKING_DIRECTORY "examples/misc05_picking/")

# Misc 5, with custom ray-box intersection
add_executable(misc05_picking_custom
        examples/misc05_picking/misc05_picking_custom.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/misc05_picking/StandardShading.vertexshader
        examples/misc05_picking/StandardShading.fragmentshader
)
target_link_libraries(misc05_picking_custom
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
)
# Xcode and Visual working directories
set_target_properties(misc05_picking_custom PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/misc05_picking/")
create_target_launcher(misc05_picking_custom WORKING_DIRECTORY "examples/misc05_picking/")

# Misc 5, with Bullet Physics
add_executable(misc05_picking_BulletPhysics
        examples/misc05_picking/misc05_picking_BulletPhysics.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        examples/misc05_picking/StandardShading.vertexshader
        examples/misc05_picking/StandardShading.fragmentshader
)
target_link_libraries(misc05_picking_BulletPhysics
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
        BulletDynamics
        BulletCollision
        LinearMath
)
# Xcode and Visual working directories
set_target_properties(misc05_picking_BulletPhysics PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/misc05_picking/")
create_target_launcher(misc05_picking_BulletPhysics WORKING_DIRECTORY "examples/misc05_picking/")


add_executable(tutorial18_billboards
        examples/tutorial18_billboards_and_particles/tutorial18_billboards.cpp
        common/shader.cpp
        common/shader.hpp
        common/texture.cpp
        common/texture.hpp
        common/controls.cpp
        common/controls.hpp
        examples/tutorial18_billboards_and_particles/Billboard.fragmentshader
        examples/tutorial18_billboards_and_particles/Billboard.vertexshader
)

target_link_libraries(tutorial18_billboards
        ${ALL_LIBS}
)

# Xcode and Visual working directories
set_target_properties(tutorial18_billboards PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial18_billboards_and_particles/")
create_target_launcher(tutorial18_billboards WORKING_DIRECTORY "examples/tutorial18_billboards_and_particles/")

add_executable(tutorial18_particles
        examples/tutorial18_billboards_and_particles/tutorial18_particles.cpp
        common/shader.cpp
        common/shader.hpp
        common/texture.cpp
        common/texture.hpp
        common/controls.cpp
        common/controls.hpp
        examples/tutorial18_billboards_and_particles/Particle.fragmentshader
        examples/tutorial18_billboards_and_particles/Particle.vertexshader
)

target_link_libraries(tutorial18_particles
        ${ALL_LIBS}
)

# Xcode and Visual working directories
set_target_properties(tutorial18_particles PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "examples/tutorial18_billboards_and_particles/")
create_target_launcher(tutorial18_particles WORKING_DIRECTORY "examples/tutorial18_billboards_and_particles/")


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*")
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$")
