<div align="center">

<a href="https://github.com/krytanik/aurion-core/graphs/contributors">![Contributors](https://img.shields.io/github/contributors/krytanik/aurion-core)</a>
<a href="https://github.com/krytanik/aurion-core/issues">![Issues](https://img.shields.io/github/issues/krytanik/aurion-core)</a>
<a href="">![License](https://img.shields.io/github/license/krytanik/aurion-core)</a>

<h1>Aurion Core</h1>

<p>
  A lightweight C++23 application framework designed to facilitate rapid development with minimal STL reliance, featuring custom memory management, modular architecture, and cross-platform window handling.
</p>

</div>

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
  - [Build Configurations](#build-configurations)
  - [Packaging](#packaging)
- [Core Modules](#core-modules)
  - [Application](#application)
  - [Events](#events)
  - [Input](#input)
  - [Logging](#logging)
  - [Memory](#memory)
  - [Types](#types)
  - [Window](#window)
- [Dependencies](#dependencies)
- [Planned Features](#planned-features)
- [License](#license)

## Overview

Aurion Core is a modular C++ framework built as a shared library (DLL) that provides essential interfaces for application development. The framework emphasizes:

- **Minimal STL Dependencies** - Custom implementations for greater control and performance
- **C++23 Modules** - Modern code organization using `.ixx` module files
- **Plugin-Based Architecture** - Extensible and configurable functionality
- **Custom Memory Management** - Three allocator strategies for different use cases

> **Note:** Currently, platform-specific implementations are only available for Windows. Linux and Mac support is planned.

## Features

| Feature                | Description                                                   |
|------------------------|---------------------------------------------------------------|
| **Custom Entry Point** | Customizable application lifecycle management                 |
| **Event System**       | Publisher/subscriber pattern with category-based routing      |
| **Input Management**   | Logical device abstractions                                   |
| **Window Management**  | Cross-platform window handling via GLFW                       |
| **Memory Allocators**  | Linear, Stack, and Pool allocators for optimized memory usage |
| **Logging**            | Color-coded console output with multiple verbosity levels     |
| **Math Library**       | Vectors, matrices, and quaternions with f32/f64 precision     |

## Project Structure

```
aurion-core/
├── CMakeLists.txt                 # Root CMake configuration
├── AurionCore/
│   ├── CMakeLists.txt             # Library target and dependency configuration
│   ├── macros/                    # Export and logging macros
│   │   ├── AurionExport.h
│   │   └── AurionLog.h
│   ├── modules/                   # C++20 module definitions (.ixx)
│   │   ├── Application/           # Application lifecycle
│   │   ├── Events/                # Event system (EventBus, listeners)
│   │   ├── Input/                 # Input devices and controls
│   │   ├── Logging/               # Console and file loggers
│   │   ├── Memory/                # Custom allocators
│   │   │   └── Allocators/        # Linear, Stack, Pool
│   │   ├── Platform/              # Platform-specific code
│   │   │   └── GLFW/              # GLFW implementation
│   │   ├── Types/                 # Primitives and math types
│   │   └── Window/                # Window interfaces
│   └── src/                       # Implementation files (.cpp)
├── premake5.lua                   # Legacy build file (deprecated)
└── LICENSE                        # MIT License
```

## Getting Started

Aurion Core is designed to be integrated with your application. Pre-built binaries are available with each [GitHub Release](https://github.com/krytanik/aurion-core/releases/latest).

### Prerequisites

- [CMake](https://cmake.org/) 4.2 or later
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/) or later (Windows)
- C++23 compatible compiler with module support
- Git (required for GLFW to be fetched automatically)

### Building from Source

1. Clone the repository:
   ```sh
   git clone https://github.com/KrytaniK/aurion-core.git
   cd aurion-core
   ```

2. Configure the build:
   ```sh
   cmake -B build
   ```

3. Build the library:
   ```sh
   cmake --build build
   ```

4. Optionally install to a local prefix:
   ```sh
   cmake --install build --prefix ./install
   ```

   This will place the DLL under `install/bin/`, static import library under `install/lib/`, module files under `install/modules/`, and macro headers under `install/include/`.

> GLFW 3.4 is fetched and built automatically via CMake's `FetchContent` — no manual download required.

### Build Configurations

Pass `-DCMAKE_BUILD_TYPE=<config>` during configuration to select a build type:

| Configuration     | Description                              |
|-------------------|------------------------------------------|
| **Debug**         | Development build with debug symbols     |
| **Release**       | Fully optimized build                    |
| **RelWithDebInfo** | Optimized build with debug info         |
| **MinSizeRel**    | Size-optimized build                     |

Example:
```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Packaging

Aurion Core uses CPack to generate distributable archives. After building, run:

```sh
cpack --config build/CPackConfig.cmake
```

This produces ZIP and TGZ archives containing the compiled binaries, module files, and headers.

## Core Modules

### Application

An abstract class derived from `IApplication` that manages the application lifecycle through `Application::StartAndRun()`. For custom lifecycle management, you may derive from `IApplication`.
```cpp
import Aurion.Application;

class MyApp : public Aurion::Application {
public:
    void Initialize(int argc, char* argv[]) override { /* Setup */ }
    void Run() override { /* Main loop */ }
    void Shutdown() override { /* Cleanup */ }
};
```

**Components:**
- `IApplication` - Lifecycle interface
- `Application` - Base class with `StartAndRun()`, `Initialize()`, `Run()`, `Shutdown()`

### Events

Publisher/subscriber event system with category-based routing.

**Components:**
- `EventBase` - Base event with timestamp and propagation control
- `EventBus` - Central event dispatcher (max 16 handlers per category)
- `IEventDispatcher` / `IEventListener` - Interfaces for custom implementations
- `EventCategoryRegistry` - Category-based handler management

**Event Categories:**
- `AC_EVENT_CATEGORY_APPLICATION`, `WINDOW`, `FILE`, `PLUGIN`, `INPUT`, `LOG`

**Features:**
- Event propagation control
- Category-based routing
- Optional timestamp tracking

### Input

Abstraction-based input system supporting logical devices.

**Components:**
- `InputDevice` - Logical input device representation
- `InputControl` - Individual controls (buttons, axes)
- `InputState` - Device state memory management
- `InputDeviceSpec` - Device metadata
- `InputDeviceCapabilities` - Button/axis/POV counts

**Event Types:**
- `InputButtonEvent` - Button press/release
- `InputAxisEvent` - Single axis input
- `InputAxis2DEvent` / `InputAxis3DEvent` / `InputAxis4DEvent` - Multi-axis input
- `InputTouchEvent` - Touch input with pressure support

> Input devices are not strictly hardware-mapped, allowing for simulated input within the application.

### Logging

Color-coded console logging with multiple verbosity levels.

**Verbosity Levels:**

| Level    | Macro                  | Color      |
|----------|------------------------|------------|
| TRACE    | `AURION_TRACE(...)`    | Gray       |
| INFO     | `AURION_INFO(...)`     | White      |
| WARN     | `AURION_WARN(...)`     | Yellow     |
| ERROR    | `AURION_ERROR(...)`    | Red        |
| CRITICAL | `AURION_CRITICAL(...)` | Bright Red |

**Components:**
- `ILogger` - Logger interface
- `ConsoleLogger` - Singleton console logger with ANSI color support
- `FileLogger` - File output (in progress)

### Memory

Custom memory allocators for optimized allocation strategies.

| Allocator         | Use Case                                | Deallocation |
|-------------------|-----------------------------------------|--------------|
| `LinearAllocator` | Temporary allocations, frame data       | Reset only   |
| `StackAllocator`  | LIFO allocations, scoped resources      | LIFO order   |
| `PoolAllocator`   | Fixed-size objects, frequent alloc/free | Free list    |

**Features:**
- 16-byte default alignment
- Move semantics (copy disabled)
- Reset capability
- Allocation headers for tracking

### Types

Custom primitive types and math utilities.

**Primitive Types:**
```cpp
// Unsigned integers
u8, u16, u32, u64

// Signed integers
i8, i16, i32, i64

// Floating point
f32 (float), f64 (double)
```

**Math Types:**

| Category    | f32 Precision                   | f64 Precision                   | Aliases                  |
|-------------|---------------------------------|---------------------------------|--------------------------|
| Vectors     | `Vec2f32`, `Vec3f32`, `Vec4f32` | `Vec2f64`, `Vec3f64`, `Vec4f64` | `fVec2/3/4`, `dVec2/3/4` |
| Matrices    | `Mat2f32`, `Mat3f32`, `Mat4f32` | `Mat2f64`, `Mat3f64`, `Mat4f64` | `fMat2/3/4`, `dMat2/3/4` |
| Quaternions | `Quatf32`                       | `Quatf64`                       | `fQuat`, `dQuat`         |

**Features:**
- Operator overloading (arithmetic, comparison, compound assignment)
- Member access via `.x/.y/.z/.w` or `.data[]`
- Matrix access via `.m11/.m12/...` pattern

### Window

Cross-platform window management via GLFW.

**Components:**
- `IWindow` - Window interface
- `Window` - Base window class
- `IWindowDriver` - Multi-window management
- `WindowProperties` - Configuration struct
- `WindowHandle` - Window reference

**Window Modes:**
- `Windowed` - Standard windowed mode
- `FullscreenExclusive` - Exclusive fullscreen
- `FullscreenBorderless` - Borderless fullscreen

**Events:**
- `WindowCreateEvent`, `WindowCloseEvent`, `WindowGetEvent`

**Platform Implementations:**
- `GLFWDriver` - GLFW-based window driver
- `GLFW_Window` - GLFW window implementation

## Dependencies

| Dependency                           | Version | Purpose                        |
|--------------------------------------|---------|--------------------------------|
| [GLFW](http://www.glfw.org)          | 3.4     | Window and input management    |
| [CMake](https://cmake.org)           | 4.2+    | Build system                   |
| Visual Studio                        | 2022+   | C++ compiler (Windows)         |
| C++ Standard                         | C++23   | Language standard with modules |

## Planned Features

- **Linux and Mac Support** - Platform-specific implementations
- **Event System Extensions** - Priority, queueing, and profiling
- **Memory Profiling** - Allocation tracking and analysis
- **Multithreading** - Thread pools, task scheduling, synchronization
- **Data Serialization** - Cross-platform reflection and versioning
- **Networking** - Server-client and peer-to-peer utilities
- **Time Tracking** - Structured time management utilities

## License

Distributed under the MIT License. See `LICENSE` for more information.

```
MIT License (c) 2025 KrytaniK
```
