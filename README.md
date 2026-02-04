<div align="center">

<a href="https://github.com/krytanik/aurion-core/graphs/contributors">![Contributors](https://img.shields.io/github/contributors/krytanik/aurion-core)</a>
<a href="https://github.com/krytanik/aurion-core/issues">![Issues](https://img.shields.io/github/issues/krytanik/aurion-core)</a>
<a href="">![License](https://img.shields.io/github/license/krytanik/aurion-core)</a>

<h1>Aurion Core</h1>

<p>
  A lightweight C++20 application framework designed to facilitate rapid development with minimal STL reliance, featuring custom memory management, modular architecture, and cross-platform window handling.
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
- **C++20 Modules** - Modern code organization using `.ixx` module files
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
├── core/
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
├── third_party/
│   ├── GLFW/                      # GLFW 3.4 (pre-compiled)
│   └── premake/                   # Premake5 executable
├── scripts/
│   └── Windows/
│       └── generate_projects.bat  # VS solution generator
├── config/
│   └── circleci/config.yml        # CI/CD configuration
├── premake5.lua                   # Build configuration
└── LICENSE                        # MIT License
```

## Getting Started

Aurion Core is designed to be integrated with your application. Pre-built binaries are available with each [GitHub Release](https://github.com/krytanik/aurion-core/releases/latest).

### Prerequisites

- [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/) or later
- [Premake 5](https://premake.github.io/) (included in `third_party/premake/`)
- C++20 compatible compiler with module support

### Building from Source

1. Clone the repository:
   ```sh
   git clone https://github.com/KrytaniK/aurion-core.git
   cd aurion-core
   ```

2. Generate Visual Studio solution:
   ```sh
   scripts/Windows/generate_projects.bat
   ```
   Or manually run Premake with your desired version of Visual Studio:
   ```sh
   premake5 vs202X
   ```

3. Open `AurionCore.sln` in Visual Studio and build.

### Build Configurations

| Configuration | Description                          | Defines             |
|---------------|--------------------------------------|---------------------|
| **Debug**     | Development build with debug symbols | `AURION_CORE_DEBUG` |
| **Release**   | Optimized build with debug info      | -                   |
| **Dist**      | Distribution build, fully optimized  | -                   |

**Output Directories:**
- Binaries: `build/bin/{Config}_Windows_x64/{Project}/`
- Intermediate: `build/bin-int/{Config}_Windows_x64/{Project}/`

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

> GLFW is linked statically through `glfw3dll.lib` and `glfw3.dll` to minimize versioning conflicts.

## Dependencies

| Dependency                           | Version | Purpose                        |
|--------------------------------------|---------|--------------------------------|
| [GLFW](http://www.glfw.org)          | 3.4     | Window and input management    |
| [Premake](https://premake.github.io) | 5.x     | Build system generation        |
| Visual Studio                        | 2022+   | C++ compiler (Windows)         |
| C++ Standard                         | C++20   | Language standard with modules |

## Planned Features

- **Linux and Mac Support** - Platform-specific implementations
- **CMake Support** - CMake build scripts for compatibility
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
