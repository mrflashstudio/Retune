cmake_minimum_required(VERSION 3.24)
project(Retune VERSION 0.1.0 LANGUAGES CXX)

if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Retune requires a Windows x64 toolchain")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_library(Retune SHARED
    src/Proxy/BcryptProxy.cpp
    src/Game/EndpointPatcher.cpp
    src/Game/EndpointDiscovery.cpp
    src/Browser/ShellLink.cpp
    src/Game/EndpointWriter.cpp
    src/NativeAot/FrozenStrings.cpp
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
target_link_libraries(Retune PRIVATE kernel32 shell32)

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
