# Sokka — A Lightweight Tilemap Editor

---

Sokka is a C++ tilemap editor designed for 2D games built with SDL2 and OpenGL.
It provides a fast and minimal interface (powered by Dear ImGui) for creating, editing, and exporting tilemaps, inspired by the design philosophy behind Celeste’s custom level editor.

## Features

---

- Visual tilemap editing — paint, erase, and inspect tiles directly on a canvas
- Tileset browser — select tiles from a texture atlas
- Layer and entity support — manage background, foreground, and interactive tiles
- JSON export — easily integrate with your custom game engine
- Lightweight and cross-platform — built with SDL2, OpenGL, and Dear ImGui

## Project Goals

---

The goal of Sokka is to create a small, self-contained level editor for rapid prototyping 2D games.  
The project focuses on simplicity, fast iteration, and a clean developer workflow rather than being a full-fledged world editor.

## Tech Stack

---

| Component          | Library / Tool                                    |
| ------------------ | ------------------------------------------------- |
| Windowing & Input  | [SDL2](https://libsdl.org/)                       |
| GUI                | [Dear ImGui](https://github.com/ocornut/imgui)    |
| Rendering          | OpenGL                                            |
| Image Loading      | [stb_image.h](https://github.com/nothings/stb)    |
| JSON Serialization | [nlohmann/json](https://github.com/nlohmann/json) |
| Build System       | CMake                                             |

## Build Instructions

---

### 1. Clone the Repository

```bash
git clone https://github.com/<your-username>/sokka.git
cd sokka
```

### 2. Build with CMake

```bash
mkdir build && cd build
cmake ..
make
```

### 3. Run

```bash
./sokka
```

## Roadmap

---

- [ ] Tile painting and erasing
- [ ] Layer management
- [ ] Undo / Redo
- [ ] Tile property editor
- [ ] Export/import from JSON
- [ ] Zoom and camera controls
- [ ] Collision mask visualization
