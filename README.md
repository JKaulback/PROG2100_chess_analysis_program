# Chess Analysis Program

A complete chess implementation in C++ featuring advanced rule validation, game state analysis, and professional graphical interface. This project demonstrates sophisticated programming concepts including complex algorithms, design patterns, and modern C++ practices with full chess rule compliance.

## 🎯 Overview

This chess analysis program is a comprehensive implementation of chess with complete rule validation, game ending detection, and position analysis. The application showcases advanced programming techniques including algorithmic game state analysis, complex move validation systems, and professional software architecture patterns.

## ✨ Features

### **Complete Chess Implementation**
- **Full Rule Validation**: All chess rules including castling, en passant, and pawn promotion
- **Advanced Game States**: Checkmate, stalemate, and draw detection (50-move rule, insufficient material)
- **Threefold Repetition**: Position history tracking with FEN-based position hashing
- **Move Generation**: Legal move detection and validation for all piece types
- **Game State Analysis**: Comprehensive end-game condition detection

### **Professional User Interface**
- **Interactive Chess Board**: High-quality 8x8 visual board with piece textures
- **Drag & Drop Gameplay**: Smooth piece movement with visual feedback
- **Game Statistics Display**: Half-move clock, current board state (FEN notation)
- **Game Over Detection**: Visual game ending notifications with result display
- **Captured Pieces Display**: Visual tracking of captured pieces for both players

### **Advanced Architecture**
- **MVC Design Pattern**: Clean separation between Model, View, and Controller
- **Move Validation System**: Dedicated validation layer with comprehensive rule checking
- **Game State Analyzer**: Separate component for game ending condition detection
- **Position History Tracking**: Efficient threefold repetition detection using hash maps
- **Configuration Management**: Centralized configuration system with namespace organization
- **Modular Design**: Clean component separation with no circular dependencies

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
│   ├── chess_logic.h              # Game state management with position tracking
│   ├── chess_logic.cpp
│   ├── chess_move_validator.h      # Comprehensive move validation system
│   ├── chess_move_validator.cpp
│   ├── chess_game_state_analyzer.h # Game ending detection (checkmate, stalemate, draws)
│   └── chess_game_state_analyzer.cpp
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

1. **ChessAnalysisProgram** (Controller): Orchestrates all game components and coordinates gameplay
2. **ChessLogic** (Model): Game state management with position history tracking for threefold repetition
3. **ChessMoveValidator**: Advanced validation system supporting all chess rules (castling, en passant, promotion)
4. **ChessGameStateAnalyzer**: Dedicated game ending detection (checkmate, stalemate, draws)
5. **ChessGUI** (View): Professional rendering with statistics display and game over screens
6. **ChessInputHandler**: Sophisticated input processing with drag-and-drop support
7. **Config Namespace**: Centralized configuration management for all components

### Architectural Benefits

- **No Circular Dependencies**: Clean, one-way dependency flow
- **Single Responsibility**: Each component has one clear purpose
- **Loose Coupling**: Components can be modified independently
- **High Cohesion**: Related functionality grouped logically
- **Easy Testing**: Each layer can be unit tested in isolation

### Advanced Data Flow

```
User Input → InputHandler → Controller → MoveValidator → ChessLogic
                ↓              ↓            ↓            ↓
           Controller → GameStateAnalyzer → View → Screen Output
                ↓              ↓            ↓
         Position History → Threefold → Game Over Detection
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

1. **Launch the Program**: Run the executable to start a new chess game
2. **Make Moves**: Click and drag pieces to move them (drag-and-drop interface)
3. **Rule Validation**: All chess rules are enforced (legal moves, turn order, check prevention)
4. **Special Moves**: Castling, en passant, and pawn promotion are fully supported
5. **Game Statistics**: View half-move clock and current board position (FEN notation)
6. **Game Endings**: The game automatically detects and displays:
   - Checkmate (winner declared)
   - Stalemate (draw)
   - 50-move rule (draw)
   - Threefold repetition (draw)
   - Insufficient material (draw)
7. **Visual Feedback**: Captured pieces are displayed, game over screens show results

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

### Game Development & Algorithms
- Advanced game logic implementation (chess rules)
- Complex state management with position history
- Algorithmic game analysis (checkmate detection, move generation)
- Graphics programming with Raylib
- Event handling and sophisticated user input processing
- Hash-based position tracking for game analysis
- Performance optimization for real-time gameplay

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

### Advanced Chess Implementation
```cpp
// Complete move validation with special move support
bool attemptMove(int srcRank, int srcFile, int destRank, int destFile) {
    MoveResult validationResult = moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
    
    if (isValidMoveResult(validationResult)) {
        // Handle special moves
        if (validationResult == MoveResult::VALID_CASTLE_KINGSIDE || 
            validationResult == MoveResult::VALID_CASTLE_QUEENSIDE) {
            logic.executeCastling(srcRank, srcFile, destRank, destFile);
        } else if (validationResult == MoveResult::VALID_EN_PASSANT) {
            logic.executeEnPassant(srcRank, srcFile, destRank, destFile);
        } else if (validationResult == MoveResult::VALID_PROMOTION) {
            logic.executePromotion(srcRank, srcFile, destRank, destFile);
        } else {
            logic.executeMove(srcRank, srcFile, destRank, destFile);
        }
        
        // Analyze game state after move
        currentGameState = gameStateAnalyzer.analyzeGameState(logic);
        return true;
    }
    return false;
}

// Threefold repetition detection using position hashing
bool hasThreefoldRepetition() const {
    for (const auto& [position, count] : positionOccurrances) {
        if (count >= 3) return true;
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

### **Advanced Algorithms & Data Structures**
- **Position Hashing**: Efficient threefold repetition using `std::unordered_map` with FEN strings
- **Move Generation**: Complex legal move detection with check/checkmate analysis
- **Game Tree Analysis**: Sophisticated game state evaluation and ending detection
- **Hash-based Tracking**: Performance-optimized position history management

### **Professional Software Architecture**
- **MVC Pattern**: Complete separation with dedicated validation and analysis layers
- **Single Responsibility**: Each component has one clear, well-defined purpose
- **SOLID Principles**: Well-designed interfaces and dependency management
- **Configuration Management**: Centralized system with compile-time constants
- **No Circular Dependencies**: Clean, maintainable component relationships

### **Modern C++ Implementation**
- **STL Mastery**: Advanced use of containers, algorithms, and iterators
- **Type Safety**: Comprehensive enums, type aliases, and strong typing
- **Memory Management**: RAII principles with proper resource cleanup
- **Modern Features**: `constexpr`, structured bindings, namespace aliases
- **Template Usage**: Type-safe generic programming where appropriate
- **Exception Safety**: Robust error handling throughout the codebase

## ✅ Implemented Features

### **Core Chess Implementation** ✅ **COMPLETE**
- [x] **Full Chess Rules**: All piece movements, turn management, board validation
- [x] **Special Moves**: Castling (kingside/queenside), en passant capture, pawn promotion
- [x] **Game State Analysis**: Checkmate, stalemate, check detection with legal move generation
- [x] **Draw Conditions**: 50-move rule, threefold repetition, insufficient material
- [x] **Position Tracking**: FEN-based position hashing for repetition detection
- [x] **Move Validation**: Comprehensive validation preventing illegal moves and self-check

### **Professional Architecture** ✅ **COMPLETE**
- [x] **MVC Pattern**: Clean separation of Model, View, and Controller
- [x] **Configuration System**: Centralized config management with namespaces
- [x] **Game State Analyzer**: Dedicated component for end-game detection
- [x] **Modular Design**: No circular dependencies, clear component relationships
- [x] **Advanced UI**: Statistics display, game over screens, captured pieces tracking

## 🚧 Potential Future Enhancements

### Advanced Features
- [ ] Move history with undo/redo functionality
- [ ] Chess notation export (PGN format)
- [ ] AI opponent with difficulty levels
- [ ] Opening book and endgame tablebase
- [ ] Network multiplayer support
- [ ] Tournament management system

### Technical Improvements
- [ ] Unit testing framework integration
- [ ] Performance profiling and optimization
- [ ] Cross-platform build system (CMake)
- [ ] Audio system for move feedback
- [ ] Save/load game functionality
- [ ] Logging system for game analysis

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
