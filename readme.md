# Game Project

## Overview
This is a simple game project that uses the Raylib library for graphics rendering. Follow the instructions below to download and install the necessary libraries and run the project on your system.

## Requirements

- **Raylib**: A C library for game development.
- **Make**: A build automation tool to compile the project.

## Installation Instructions

### **Linux**

1. **Install Raylib dependencies**:
   Open the terminal and run the following commands to install Raylib:
   ```bash
   sudo apt-get update
   sudo apt-get install cmake build-essential
   sudo apt-get install libsdl2-dev
   sudo apt-get install libasound2-dev
   sudo apt-get install libpulse-dev
   sudo apt-get install libudev-dev
   ```

2. **Clone Raylib repository**:
   Download Raylib from its GitHub repository:
   ```bash
   git clone https://github.com/raysan5/raylib.git
   cd raylib
   ```

3. **Build and install Raylib**:
   Run the following commands to build and install Raylib:
   ```bash
   cmake .
   make
   sudo make install
   ```

### **macOS**

1. **Install Homebrew** (if you don't have it already):
   Open the terminal and run:
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install Raylib using Homebrew**:
   Once Homebrew is installed, run the following command to install Raylib:
   ```bash
   brew install raylib
   ```

### **Windows**

1. **Install Raylib**:
   - Download the latest release of Raylib for Windows from the official [Raylib website](https://www.raylib.com/).
   - Extract the files to a directory.

2. **Set up Visual Studio**:
   - Open Visual Studio and create a new C++ project.
   - Link the Raylib library by adding the `raylib.lib` file to the project’s linker settings.
   - Add the Raylib include directory to the project’s include settings.

---

## How to Run the Project

1. **Download or clone the project**:
   If you haven't already, download or clone the project:
   ```bash
   git clone https://your-repository-url.git
   cd your-project-directory
   ```

2. **Build the project**:
   Open the terminal and run the `make` command to compile the project:
   ```bash
   make
   ```

3. **Run the game**:
   Once the project has been successfully compiled, run the game with the following command:
   ```bash
   ./game
   ```

---

## Troubleshooting

- **Error: `make: command not found`**  
  If you see this error, make sure `make` is installed on your system:
  - On **Linux**, install it with: `sudo apt-get install build-essential`
  - On **macOS**, you can install it via Xcode Command Line Tools: `xcode-select --install`
  - On **Windows**, you can use tools like [MinGW](http://mingw.org/) or [Cygwin](https://www.cygwin.com/).

- **Error: Missing Raylib library**  
  If the build process cannot find Raylib, ensure that the Raylib include and library paths are set correctly in your `Makefile` or project configuration.