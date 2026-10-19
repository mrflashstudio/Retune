cmake_minimum_required(VERSION 3.24)
project(Retune VERSION 0.1.0 LANGUAGES C CXX)

if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Retune requires a Windows x64 toolchain")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_library(minhook STATIC
    vendor/minhook/src/buffer.c
    vendor/minhook/src/hook.c
    vendor/minhook/src/trampoline.c
    vendor/minhook/src/hde/hde64.c
)
target_include_directories(minhook PUBLIC vendor/minhook/include)

add_library(Retune SHARED
    src/Proxy/BcryptProxy.cpp
    src/Game/Patcher.cpp
    src/Game/PatchDiscovery.cpp
    src/Game/StringDecoderHooks.cpp
    src/Game/StringRedirector.cpp
    src/Game/Failure.cpp
    src/NativeAot/FrozenString.cpp
    src/Arguments/ServerArguments.cpp
    src/Scanning/Image.cpp
    src/Scanning/Pattern.cpp
)
if(MSVC)
    target_sources(Retune PRIVATE src/Proxy/Exports/Bcrypt.h)
else()
    target_sources(Retune PRIVATE src/Proxy/Exports/Bcrypt.def)
endif()
set_target_properties(Retune PROPERTIES PREFIX "" OUTPUT_NAME bcrypt)
target_compile_definitions(Retune PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(Retune PRIVATE kernel32 shell32 minhook)

function(ConfigureNativeTarget targetName)
    if(MSVC)
        target_compile_options(${targetName} PRIVATE /W4 /WX /GS /permissive-)
        target_link_options(${targetName} PRIVATE /DYNAMICBASE /NXCOMPAT)
    else()
        target_compile_options(${targetName} PRIVATE -Wall -Wextra -Werror -fstack-protector-strong)
        target_link_options(${targetName} PRIVATE -static -Wl,--dynamicbase,--nxcompat)
    endif()
endfunction()
ConfigureNativeTarget(Retune)

install(TARGETS Retune RUNTIME DESTINATION . LIBRARY DESTINATION .)
