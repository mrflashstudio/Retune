# Retune

A (Capo)Rhythia server switcher, built specifically for [Resonance](https://github.com/mrflashstudio/Resonance) private servers.

## Building Retune

Requires CMake 3.24+ and a Windows x64 C++20 toolchain.

```sh
cmake -S . -B build
cmake --build build --config Release
```

## Using Retune

1. Download `bcrypt.dll` from the [latest release](https://github.com/mrflashstudio/Retune/releases/latest) and place it beside `rhythia.exe`.
2. Add `--resonance-server example.com` to Rhythia's Steam launch options, replacing `example.com` with your server's domain.
3. Launch the game.

## Disabling/Uninstalling Retune

There are multiple ways to disable or uninstall Retune:

- Remove `--resonance-server` and its value from Steam launch options.
- Rename the installed `bcrypt.dll` file to something else, for example, `bcrypt.dll.disabled`.
- Delete the installed `bcrypt.dll` file to uninstall the patcher.
