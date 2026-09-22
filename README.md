# Resonance Patcher

A Windows x64 DLL that redirects Rhythia to a private server.

## Build

Requires CMake 3.24+ and a Windows x64 C++20 toolchain.

```sh
cmake -S . -B build
cmake --build build --config Release
```

## Using Resonance Patcher

1. Download `bcrypt.dll` from the [latest release](https://github.com/mrflashstudio/ResonancePatcher/releases/latest) and place it beside `rhythia.exe`.
2. Add `--resonance-server example.com` to Rhythia's Steam launch options, replacing `example.com` with your server's domain.
3. Launch the game.

## Disabling/Uninstalling Resonance Patcher

There are multiple ways to disable or uninstall the patcher:

- Remove `--resonance-server` and its value from Steam launch options.
- Rename the installed `bcrypt.dll` file to something else, for example, `bcrypt.dll.disabled`.
- Delete the installed `bcrypt.dll` file to uninstall the patcher.
