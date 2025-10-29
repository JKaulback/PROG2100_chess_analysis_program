# Chess Analysis Program

A sophisticated chess application built with C++ and Raylib, featuring an interactive graphical interface, comprehensive chess game logic, and a clean modular architecture implementing the Controller design pattern.

## 🎯 Overview

This chess analysis program demonstrates advanced C++ programming concepts including object-oriented design, design patterns, memory management, and graphical programming. The application provides a fully functional chess board with piece movement, drag-and-drop interaction, and a professional modular codebase that follows software engineering best practices.

## ✨ Features

- **Interactive Chess Board**: Visual 8x8 chess board with high-quality graphics
- **Drag & Drop Gameplay**: Intuitive piece movement with mouse interaction
- **Move Validation System**: Comprehensive move validation with clear error reporting
- **Centralized Configuration**: Professional configuration management system
- **Complete Chess Logic**: Full implementation of chess rules and piece behavior
- **Professional GUI**: Clean, responsive interface built with Raylib
- **Controller Architecture**: Enhanced MVC pattern with move validation coordination
- **Modular Structure**: Organized folder structure with logical component separation
- **Input Handling**: Dedicated input processing system independent of rendering
- **Clean Dependencies**: No circular dependencies, clear component relationships
- **Type Safety**: Modern C++ with type aliases and namespace management

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
│   ├── chess_analysis_program.h     # Enhanced MVC Controller with move validation
│   └── chess_analysis_program.cpp
├── core/                           # Game logic and validation systems
│   ├── chess_logic.h              # Pure state management (Model)
│   ├── chess_logic.cpp
│   ├── chess_move_validator.h      # Move validation system
│   └── chess_move_validator.cpp
├── rendering/                      # User interface and rendering layer
│   ├── chess_gui.h                # Graphics and visual presentation (View)
│   └── chess_gui.cpp
├── input/                          # Input handling and processing
│   ├── chess_input_handler.h      # Controller-integrated input processing
│   └── chess_input_handler.cpp
├── config/                         # Configuration management
│   └── config.h                    # Centralized configuration constants
└── assets/                         # Game resources and textures
    ├── board.png                   # Chess board texture
    └── chess_pieces/               # Individual piece sprites
        ├── wb.png, wk.png, wn.png, wp.png, wq.png, wr.png  # White pieces
        └── bb.png, bk.png, bn.png, bp.png, bq.png, br.png  # Black pieces
```

## 🏗 Architecture

The application implements a **Model-View-Controller (MVC)** architecture with clear separation of concerns:

### Core Components

1. **ChessAnalysisProgram** (Controller): Orchestrates validation, logic, and UI components
2. **ChessGUI** (View): Pure rendering with centralized configuration
3. **ChessLogic** (Model): Pure state management with utility methods for validation
4. **ChessMoveValidator**: Dedicated move validation with extensible architecture
5. **ChessInputHandler**: Controller-integrated input processing system
6. **Config Namespace**: Centralized configuration management system

### Architectural Benefits

- **No Circular Dependencies**: Clean, one-way dependency flow
- **Single Responsibility**: Each component has one clear purpose
- **Loose Coupling**: Components can be modified independently
- **High Cohesion**: Related functionality grouped logically
- **Easy Testing**: Each layer can be unit tested in isolation

### Enhanced Data Flow

```
User Input → InputHandler → Controller → MoveValidator → ChessLogic
                ↓              ↓            ↓
           Controller → View (ChessGUI) → Screen Output
                ↓
           Config System → All Components
```

### Key Design Features

- **Enhanced Controller Pattern**: Coordinates validation, state, and presentation
- **Centralized Configuration**: Namespace-based config with compile-time constants
- **Move Validation Architecture**: Extensible validation system with clear error reporting
- **Type Aliases**: Clean code with `using` declarations for complex types
- **Namespace Aliases**: Shortened syntax while maintaining clarity
- **RAII**: Proper resource management with constructors/destructors
- **Reference Semantics**: Efficient object passing without unnecessary copies
- **Template Usage**: Type-safe containers (std::array, std::vector)
- **Modern C++**: Move semantics, constexpr, and smart memory management
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
g++ -fdiagnostics-color=always -g src/main.cpp src/application/chess_analysis_program.cpp src/core/chess_logic.cpp src/core/chess_move_validator.cpp src/rendering/chess_gui.cpp src/input/chess_input_handler.cpp -o main.exe -I C:/raylib/include -L C:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

### Running the Application

```bash
./main.exe
```

## 🎮 How to Use

1. **Launch the Program**: Run the executable to open the chess board
2. **Move Pieces**: Click and drag any piece to move it
3. **Move Validation**: The system validates moves and provides feedback for invalid attempts
4. **Board Boundaries**: Only moves within the 8x8 board are accepted
5. **Visual Feedback**: Pieces highlight during drag operations
6. **Error Handling**: Invalid moves are rejected with clear validation messages

## 🧠 Learning Outcomes

This project demonstrates proficiency in:

### C++ Programming Concepts
- Object-Oriented Programming (OOP)
- Design Patterns (Enhanced Model-View-Controller)
- Memory management and RAII
- STL containers and algorithms (std::array, std::vector, std::string)
- Move semantics and copy constructors
- Template programming and type traits
- Namespace management and aliases
- Type aliases with `using` declarations
- Compile-time constants with `constexpr`
- Header/implementation file organization
- Forward declarations and dependency management

### Software Engineering Practices
- Enhanced modular design with clear separation of concerns
- Clean architecture principles with validation layer
- Configuration management and centralized constants
- Dependency inversion and loose coupling
- Single Responsibility Principle (SRP)
- Extensible architecture for future enhancements
- Professional error handling and validation
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

## �️ Architecture Highlights

### Configuration Management System
```cpp
// Centralized configuration with namespace organization
namespace Config::Window {
    constexpr int WIDTH = 1920;
    constexpr int HEIGHT = 1080;
    constexpr int TARGET_FPS = 60;
}

// Usage with namespace aliases
namespace WinCfg = Config::Window;
InitWindow(WinCfg::WIDTH, WinCfg::HEIGHT, WinCfg::TITLE);
```

### Move Validation Architecture
```cpp
// Type alias for cleaner code
using MoveResult = ChessMoveValidator::MoveResult;

// Controller coordination
bool attemptMove(int fromRank, int fromFile, int toRank, int toFile) {
    auto result = moveValidator.validateMove(logic, fromRank, fromFile, toRank, toFile);
    if (result == MoveResult::VALID) {
        logic.executeMove(fromRank, fromFile, toRank, toFile);
        return true;
    }
    return false;
}
```

### Enhanced MVC Pattern
- **Controller**: Orchestrates validation and state management
- **Model**: Pure state with utility methods for validation queries  
- **View**: Configuration-driven rendering with namespace aliases
- **Validator**: Dedicated validation component with extensible architecture

## 🏆 Academic Context

**Course**: PROG2100 - Advanced Programming  
**Institution**: Nova Scotia Community College (NSCC)  
**Term**: Fall 2025  
**Focus Areas**: Advanced C++, Data Structures, OOP Design, Software Architecture

## 🔍 Code Quality Highlights

- **Enhanced MVC Architecture**: Model-View-Controller with dedicated validation layer
- **Configuration Management**: Centralized config system with namespace organization
- **Move Validation System**: Extensible validation with clear error reporting
- **Type Safety**: Comprehensive use of enums, type aliases, and strong typing
- **Modern C++ Features**: constexpr, namespace aliases, and using declarations
- **No Circular Dependencies**: Well-designed component relationships
- **Modular Structure**: Organized folder hierarchy for easy navigation and maintenance
- **Resource Management**: Proper texture loading and cleanup with RAII
- **Error Handling**: Robust validation system with detailed feedback
- **Controller Coordination**: Centralized orchestration without tight coupling
- **Single Responsibility**: Each class has one clear, well-defined purpose
- **Extensible Design**: Easy to add new validation rules and features
- **Professional Patterns**: Industry-standard architectural approaches
- **Documentation**: Extensive inline comments explaining complex logic and architecture decisions

## 🚧 Future Enhancements

The modular architecture makes these enhancements straightforward to implement:

### Gameplay Features
- [ ] Enhanced piece movement validation (piece-specific rules)
- [ ] Chess move analysis and suggestions
- [ ] Move history and replay functionality  
- [ ] Advanced AI opponent (new `src/ai/` module)
- [ ] Game state persistence (save/load functionality)
- [ ] Tournament bracket system
- [ ] Special moves (castling, en passant, promotion)

### Technical Improvements
- [x] **Configuration system** (`src/config/` module) ✅ **IMPLEMENTED**
- [x] **Move validation architecture** ✅ **IMPLEMENTED** 
- [ ] Advanced validation rules (check, checkmate, stalemate)
- [ ] Audio system (`src/audio/` module)
- [ ] Network multiplayer (`src/network/` module)
- [ ] Unit testing framework (`tests/` directory)
- [ ] Performance profiling and optimization
- [ ] Cross-platform build system (CMake)
- [ ] Logging system for debugging and analysis

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
