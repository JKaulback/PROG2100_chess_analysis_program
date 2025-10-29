# Chess Analysis Program

A sophisticated chess application built with C++ and Raylib, featuring an interactive graphical interface and comprehensive chess game logic.

## 🎯 Overview

This chess analysis program demonstrates advanced C++ programming concepts including object-oriented design, memory management, and graphical programming. The application provides a fully functional chess board with piece movement, drag-and-drop interaction, and a clean, intuitive user interface.

## ✨ Features

- **Interactive Chess Board**: Visual 8x8 chess board with high-quality graphics
- **Drag & Drop Gameplay**: Intuitive piece movement with mouse interaction
- **Complete Chess Logic**: Full implementation of chess rules and piece behavior
- **Professional GUI**: Clean, responsive interface built with Raylib
- **Modular Architecture**: Well-structured codebase with clear separation of concerns

## 🛠 Technical Stack

- **Language**: C++17
- **Graphics Library**: Raylib
- **Build System**: MinGW-w64 GCC
- **Architecture**: Object-Oriented Programming (OOP)
- **Design Patterns**: Model-View separation

## 📁 Project Structure

```
src/
├── main.cpp                    # Application entry point
├── chess_analysis_program.h/.cpp  # Main program controller
├── chess_gui.h/.cpp           # User interface and rendering
├── chess_logic.h/.cpp         # Game logic and rules
├── chess_piece_logic.h/.cpp   # Chess piece definitions and behavior
└── assets/
    ├── board.png              # Chess board texture
    └── chess_pieces/          # Individual piece sprites
        ├── wb.png, wk.png, wn.png, wp.png, wq.png, wr.png  # White pieces
        └── bb.png, bk.png, bn.png, bp.png, bq.png, br.png  # Black pieces
```

## 🏗 Architecture

The application follows a clean architectural pattern with distinct layers:

### Core Components

1. **ChessAnalysisProgram**: Main application controller managing the game loop
2. **ChessGUI**: Handles all rendering, user input, and visual feedback
3. **ChessLogic**: Implements chess rules, move validation, and game state
4. **ChessPiece**: Represents individual chess pieces with type safety

### Key Design Features

- **RAII**: Proper resource management with constructors/destructors
- **Reference Semantics**: Efficient object passing without unnecessary copies
- **Template Usage**: Type-safe containers (std::array, std::map)
- **Modern C++**: Move semantics and smart memory management

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
g++ -fdiagnostics-color=always -g src/*.cpp -o main.exe -I C:/raylib/include -L C:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
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
- Memory management and RAII
- STL containers and algorithms
- Move semantics and copy constructors
- Template programming
- Header/implementation file organization

### Software Engineering Practices
- Modular design and separation of concerns
- Code documentation and commenting
- Version control with Git
- Build automation
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

- **Clean Architecture**: Clear separation between GUI, logic, and data layers
- **Type Safety**: Comprehensive use of enums and strong typing
- **Resource Management**: Proper texture loading and cleanup
- **Error Handling**: Robust input validation and bounds checking
- **Documentation**: Extensive inline comments explaining complex logic

## 🚧 Future Enhancements

Potential areas for expansion:
- [ ] Chess move analysis and suggestions
- [ ] Game state persistence (save/load)
- [ ] Network multiplayer support
- [ ] Advanced AI opponent
- [ ] Move history and replay functionality
- [ ] Tournament bracket system

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
