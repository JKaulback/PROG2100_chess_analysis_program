# Chess Analysis Program

A sophisticated chess application built with C++ and Raylib, featuring an interactive graphical interface, comprehensive chess game logic, and a clean modular architecture implementing the Controller design pattern.

## 🎯 Overview

This chess analysis program demonstrates advanced C++ programming concepts including object-oriented design, design patterns, memory management, and graphical programming. The application provides a fully functional chess board with piece movement, drag-and-drop interaction, and a professional modular codebase that follows software engineering best practices.

## ✨ Features

- **Interactive Chess Board**: Visual 8x8 chess board with high-quality graphics
- **Drag & Drop Gameplay**: Intuitive piece movement with mouse interaction
- **Complete Chess Logic**: Full implementation of chess rules and piece behavior
- **Professional GUI**: Clean, responsive interface built with Raylib
- **Controller Architecture**: Implements Controller design pattern for clean separation of concerns
- **Modular Structure**: Organized folder structure with logical component separation
- **Input Handling**: Dedicated input processing system independent of rendering
- **Clean Dependencies**: No circular dependencies, clear component relationships

## 🛠 Technical Stack

- **Language**: C++17
- **Graphics Library**: Raylib
- **Build System**: MinGW-w64 GCC
- **Architecture**: Object-Oriented Programming (OOP) with Controller Pattern
- **Design Patterns**: Model-View-Controller (MVC), Separation of Concerns

## 📁 Project Structure

```
src/
├── main.cpp                         # Application entry point
├── application/                     # Main application coordination layer
│   ├── chess_analysis_program.h     # Main program controller (MVC Controller)
│   └── chess_analysis_program.cpp
├── core/                           # Game logic and core functionality
│   ├── chess_logic.h              # Game rules and state management (Model)
│   └── chess_logic.cpp
├── rendering/                      # User interface and rendering layer
│   ├── chess_gui.h                # Graphics and visual presentation (View)
│   └── chess_gui.cpp
├── input/                          # Input handling and processing
│   ├── chess_input_handler.h      # User input processing system
│   └── chess_input_handler.cpp
└── assets/                         # Game resources and textures
    ├── board.png                   # Chess board texture
    └── chess_pieces/               # Individual piece sprites
        ├── wb.png, wk.png, wn.png, wp.png, wq.png, wr.png  # White pieces
        └── bb.png, bk.png, bn.png, bp.png, bq.png, br.png  # Black pieces
```

## 🏗 Architecture

The application implements a **Model-View-Controller (MVC)** architecture with clear separation of concerns:

### Core Components

1. **ChessAnalysisProgram** (Controller): Orchestrates all components and manages application flow
2. **ChessGUI** (View): Pure rendering and visual presentation layer
3. **ChessLogic** (Model): Game rules, state management, and chess logic
4. **ChessInputHandler**: Dedicated input processing system

### Architectural Benefits

- **No Circular Dependencies**: Clean, one-way dependency flow
- **Single Responsibility**: Each component has one clear purpose
- **Loose Coupling**: Components can be modified independently
- **High Cohesion**: Related functionality grouped logically
- **Easy Testing**: Each layer can be unit tested in isolation

### Data Flow

```
User Input → InputHandler → Controller → Model (ChessLogic)
                ↓              ↓
           Controller → View (ChessGUI) → Screen Output
```

### Key Design Features

- **Controller Pattern**: Central coordinator managing component interactions
- **RAII**: Proper resource management with constructors/destructors
- **Reference Semantics**: Efficient object passing without unnecessary copies
- **Template Usage**: Type-safe containers (std::array, std::map)
- **Modern C++**: Move semantics and smart memory management
- **Modular Design**: Organized folder structure for maintainability

## 🚀 Getting Started

### Prerequisites

- MinGW-w64 GCC compiler
- Raylib library (graphics framework)
- Windows 10/11 (current configuration)

### Installation & Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/JKaulback/PROG2100_chess_analysis_program.git
   cd PROG2100_chess_analysis_program
   ```

2. **Install Raylib**:
   - Download Raylib from [raylib.com](https://www.raylib.com/)
   - Extract to `C:/raylib/`

3. **Install MinGW-w64**:
   - Download from [MinGW-w64](https://www.mingw-w64.org/)
   - Extract to `C:/mingw-w64/`

### Building the Project

Using VS Code (recommended):
```bash
# Use the built-in build task
Ctrl+Shift+P → "Tasks: Run Build Task"
```

Or via command line:
```bash
g++ -fdiagnostics-color=always -g src/**/*.cpp src/*.cpp -o main.exe -I C:/raylib/include -L C:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

### Running the Application

```bash
./main.exe
```

## 🎮 How to Use

1. **Launch the Program**: Run the executable to open the chess board
2. **Move Pieces**: Click and drag any piece to move it
3. **Game Rules**: The program enforces standard chess movement rules
4. **Visual Feedback**: Pieces highlight during drag operations

## 🧠 Learning Outcomes

This project demonstrates proficiency in:

### C++ Programming Concepts
- Object-Oriented Programming (OOP)
- Design Patterns (Model-View-Controller)
- Memory management and RAII
- STL containers and algorithms
- Move semantics and copy constructors
- Template programming
- Header/implementation file organization
- Forward declarations and dependency management

### Software Engineering Practices
- Modular design and separation of concerns
- Clean architecture principles
- Dependency inversion and loose coupling
- Single Responsibility Principle (SRP)
- Code documentation and commenting
- Version control with Git
- Build automation and project structure organization
- Cross-platform considerations

### Game Development Fundamentals
- Graphics programming with Raylib
- Event handling and user input
- Game state management
- Asset loading and management
- Performance optimization

## 🏆 Academic Context

**Course**: PROG2100 - Advanced Programming  
**Institution**: Nova Scotia Community College (NSCC)  
**Term**: Fall 2025  
**Focus Areas**: Advanced C++, Data Structures, OOP Design

## 🔍 Code Quality Highlights

- **MVC Architecture**: Proper Model-View-Controller implementation with clean separation
- **No Circular Dependencies**: Well-designed component relationships
- **Modular Structure**: Organized folder hierarchy for easy navigation and maintenance
- **Type Safety**: Comprehensive use of enums and strong typing
- **Resource Management**: Proper texture loading and cleanup with RAII
- **Error Handling**: Robust input validation and bounds checking
- **Controller Pattern**: Centralized coordination without tight coupling
- **Single Responsibility**: Each class has one clear, well-defined purpose
- **Documentation**: Extensive inline comments explaining complex logic and architecture decisions

## 🚧 Future Enhancements

The modular architecture makes these enhancements straightforward to implement:

### Gameplay Features
- [ ] Chess move analysis and suggestions
- [ ] Move history and replay functionality  
- [ ] Advanced AI opponent (new `src/ai/` module)
- [ ] Game state persistence (save/load functionality)
- [ ] Tournament bracket system

### Technical Improvements
- [ ] Configuration system (`src/config/` module)
- [ ] Audio system (`src/audio/` module)
- [ ] Network multiplayer (`src/network/` module)
- [ ] Unit testing framework (`tests/` directory)
- [ ] Performance profiling and optimization
- [ ] Cross-platform build system (CMake)

## 📊 Technical Specifications

- **Resolution**: 1920x1080 (Full HD)
- **Frame Rate**: 60 FPS target
- **Board Size**: Scalable (currently 67.5% of texture size)
- **Piece Assets**: 12 individual PNG sprites
- **Memory Management**: Stack-based allocation with RAII

## 👥 Contributing

This is an academic project, but feedback and suggestions are welcome! Please feel free to:
- Report issues or bugs
- Suggest improvements
- Discuss alternative implementations

## 📞 Contact

**Developer**: Justin Kaulback
**Email**: jtkaulback@gmail.com
**LinkedIn**: [@JustinKaulback](https://www.linkedin.com/in/justin-kaulback-832b8314b/)  
**GitHub**: [@JKaulback](https://github.com/JKaulback)

## 📄 License

This project is created for educational purposes as part of the PROG2100 course at NSCC.

---

*Developed with passion for clean code and game development* 🎯
