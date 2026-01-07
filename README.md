<div align="center">

<a href="https://github.com/krytanik/aurion-core/graphs/contributors">![Contributors](https://img.shields.io/github/contributors/krytanik/aurion-core)</a>
<a href="https://github.com/krytanik/aurion-core/issues">![Issues](https://img.shields.io/github/issues/krytanik/aurion-core)</a>
<a href="">![License](https://img.shields.io/github/license/krytanik/aurion-core)</a>

<h1>Aurion Core</h1>

<p>
  Aurion Core is designed to faciliate the rapid development of new applications by providing lightweight library of interfaces with minimal reliance on the standard template library.
</p>

</div>


## Getting Started

Aurion Core is primarily meant to be used in conjunction with your application. To this extent, pre-built binaries have been included with each [GitHub Release](https://github.com/krytanik/aurion-core/releases/latest)
> Note: Currently, platform-specific implementations have only been created for Windows. Linux and Mac implementations are planned!

### Building From Source

#### Prerequisites
- A valid installation of [**Visual Studio Community 2022**](https://visualstudio.microsoft.com/vs/community/) or later
- A valid installation of [**Premake 5**](https://premake.github.io/) or later

#### Installation
To begin, clone the repositiory into your destination folder via

```
git clone https://github.com/KrytaniK/aurion-core.git
```

From here, simply navigate to the `scripts` folder and run one of the following setup files:
- `Windows/generate_projects.bat` will generate the Visual Studio Solution files.

## Key Features

### Window Management
Aurion provides a few interfaces for working with window objects:
- **Window Interface**: For deriving custom window implementations
- **Window Driver Interface**: For managing a collection of windows
- **Window Context**: An extension for use with the plugin system

In addition to these interfaces, a cross-platform implementation using GLFW is provided.
> Note: To minimize potential versioning conflicts, glfw is linked statically through glfw3dll.lib and glfw3.dll, and is included with the project.

:heavy_check_mark: **Plugin-Based Architecture** – Extendable and overwritable functionality through configurable plugins.  
:heavy_check_mark: **Custom Entry Point** – Customizable application entry points.   
:heavy_check_mark: **Debug Tools** – Logging (console, file).   
:heavy_check_mark: **Memory Utility** – Basic utilities for better memory management.   
:heavy_check_mark: **Window Management** – Cross-Platform window management through GLFW.    
:heavy_check_mark: **Input Management** – Basic I/O support for common devices.   
:heavy_check_mark: **Events** – Various interfaces for handling application events (publisher/subscriber, listeners, event bus, etc.).   
:heavy_check_mark: **File System** – A streamlined way to handle files.   

Because loading plugins dynamically at runtime is platform-specific, Aurion offers implementations for the following platforms:
- Windows via `WindowsPluginLoader`

### Input
One of the most complex systems for any application is input handling, especially if you intend to write such a system from scratch. Aurion offers a unique approach to handling this issue:
- **Input Context**: An interface designed to facilitate the encapsulated creation of 'logical input devices' and their respective memory layouts.
- **Input Device**: A logical unit to represent a physical device (such as a keyboard or mouse).
- **Input Device Layout**: A structure representing the full memory layout of a device, including information about each device control.
- **Input Control**: The logical representation for unique input types. These can be things such as buttons, vector positions, rotations, and more.

Because the interface was designed to be abstract, logical devices aren't required to map to hardware. You can simulate input completely from within the application.

### File System
Aurion provides two core systems for working with files:

#### File System (CORE):
- **File System Interface**: An interface for deriving platform-specific file system implementations.
- **File Handle**: A basic, platform-agnostic class designed to encapsulate file reading and writing operations.
- **File Data**: An abstract representation of a file's contents. Used internally by the File Handle.
- **Directory Handle**: Similar to the file handle, the directory handle is a platform-agnostic class designed for basic directory operations.

#### Virtual File System:
> Note: Example implementations are a WIP.

The virtual file system is simply an interface designed to allow implementations to 'mount' system directories so derived applications may use relative 'virtual' pathing instead of full system paths.
One such example would be to map the directory:
```
"C:\Users\You\Documents\MyFolder\NestedFolder\DeeplyNestedFolder\AnotherDeeplyNestedFolder"
```
to something like:
```
"MyDirectory"
```
Then, for comparison, file access from within the application could look something like:
```
myFileLoader.LoadFile("MyDirectory/MyFile.txt");
```
instead of an absolute or relative system path:
```
myFileLoader.LoadFile("C:/Users/You/Documents/MyFolder/NestedFolder/DeeplyNestedFolder/AnotherDeeplyNestedFolder/MyFile.txt");
```

### Event System
A basic interface has been defined for deriving custom listener/dispatcher implementations through `IEventDispatcher` and `IEventListener`.

## Planned Features
- **Linux and Mac Support** for platform-specific features that require it.
- **Event System Extensions** to allow for event priority, queueing, and profiling.
- **Memory Profiling Utils** to track unique allocations, copies, etc. for custom memory structures.
- **Multithreading Support** through custom thread pools, task scheduling, and synchronization utils.
- **Cross-platform Data Serialization** to handle data reflection and versioning.
- **Basic Networking Utilities** for various server-client and client-client interactions.
- **Time Tracking** for a simplified and structured way to track and use time in applications.


<!---
## **🚀 Getting Started**

Pre-compiled binaries for each platform can be found [Here]()
--->

<!---
### **🔹 Prerequisites**

- **OS Support:** Windows, Mac , Linux
- **Dependencies:** List required dependencies.
- **Compiler:** Minimum compiler versions & requirements.
--->

<!---
### **🔹 Installation**

```sh
# Clone the repository
git clone https://github.com/your-org/repo-name.git
cd repo-name

# Build with Premake
premake5 vs2022  # (or gmake2 for Linux/macOS)

# Compile
make               # Linux/macOS
MSBuild solution.sln  # Windows

# Run the application (if applicable)
./bin/Debug/project-name
```
--->
