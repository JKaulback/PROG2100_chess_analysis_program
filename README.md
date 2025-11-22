# Chess Analysis Program

A comprehensive chess analysis platform in C++ featuring complete rule validation, game state analysis, UCI chess engine integration, and a professional graphical interface. This project demonstrates sophisticated programming concepts including complex algorithms, design patterns, modern C++ practices, and chess engine communication protocols.

## 🎯 Overview

This chess analysis program is a full-featured chess platform with complete rule validation, game ending detection, position analysis, and **UCI chess engine integration with Stockfish**. The application showcases advanced programming techniques including algorithmic game state analysis, complex move validation systems, professional software architecture patterns, and external process communication for chess engine analysis.

## ✨ Features

### **Complete Chess Implementation**
- **Full Rule Validation**: All chess rules including castling, en passant, and pawn promotion
- **Advanced Game States**: Checkmate, stalemate, and draw detection (50-move rule, insufficient material)
- **Threefold Repetition**: Position history tracking with FEN-based position hashing
- **Move Generation**: Legal move detection and validation for all piece types
- **Game State Analysis**: Comprehensive end-game condition detection
- **Undo/Redo System**: Full position history with move reversal capabilities

### **UCI Chess Engine Integration**
- **Stockfish Integration**: Embedded Stockfish chess engine for position analysis
- **UCI Protocol**: Full Universal Chess Interface communication protocol implementation
- **Real-time Analysis**: Live engine evaluation with best move suggestions
- **Engine Management**: Start, stop, and configure chess engine analysis
- **Position Communication**: FEN-based position sharing with the engine

### **Professional User Interface**
- **Interactive Chess Board**: High-quality 8x8 visual board with coordinate labels
- **Drag & Drop Gameplay**: Smooth piece movement with visual feedback
- **Component-Based Rendering**: Modular UI components for different interface elements
- **Game Statistics Display**: Half-move clock, current board state (FEN notation)
- **Move History Panel**: Complete game notation with navigation support
- **Engine Analysis Display**: Real-time engine evaluation and move suggestions
- **Captured Pieces Display**: Visual tracking of captured pieces for both players
- **Board Flip Feature**: Toggle board orientation for different player perspectives

### **Advanced Architecture**
- **Component-Based Design**: Modular rendering system with dedicated UI components
- **UCI Engine Architecture**: Separate analysis engine subsystem with process management
- **Move Validation System**: Comprehensive validation layer with multiple validator types
- **Game State Management**: Sophisticated state tracking with position history
- **Configuration Management**: Namespace-organized configuration with compile-time constants
- **Texture Management**: Centralized resource management for graphical assets
- **Input Processing**: Dedicated input handling with drag-and-drop support

## 🛠 Technical Stack

- **Language**: C++17
- **Graphics Library**: Raylib
- **Chess Engine**: Stockfish (embedded via UCI protocol)
- **Build System**: MinGW-w64 GCC
- **Architecture**: Component-Based Design with MVC principles
- **Communication**: UCI (Universal Chess Interface) protocol
- **Design Patterns**: Component Architecture, Observer Pattern, Strategy Pattern

## 📁 Project Structure

```
src/
├── main.cpp                                    # Application entry point
├── analysis_engine/                          # UCI chess engine subsystem
│   ├── stockfish.exe                         # Embedded Stockfish chess engine
│   ├── uci_engine.h/.cpp                     # Main UCI engine management
│   ├── uci_communication.h/.cpp              # UCI protocol communication
│   ├── uci_process.h/.cpp                    # Process management for engine
│   └── uci_analysis_parser.h/.cpp            # Engine output parsing and analysis
├── application/                               # Main application coordination layer
│   ├── chess_analysis_program.h              # Primary controller with engine integration
│   └── chess_analysis_program.cpp
├── core/                                     # Game logic and validation systems
│   ├── chess_move.h/.cpp                     # Move representation and utilities
│   ├── chess_move_validator.h/.cpp           # Comprehensive move validation system
│   ├── fen_loader.h/.cpp                     # FEN string parsing and position loading
│   ├── board/
│   │   ├── chess_board.h/.cpp                # Board state and piece management
│   ├── game_state/
│   │   ├── chess_game_state.h/.cpp           # Current game state tracking
│   │   ├── chess_game_state_analyzer.h/.cpp  # Game ending detection
│   │   └── fen_position_tracker.h/.cpp       # Position history and FEN tracking
│   └── validators/                           # Specialized move validation components
│       ├── basic_move_validator.h/.cpp       # Basic move rules
│       ├── check_validator.h/.cpp            # Check and checkmate validation
│       ├── piece_movement_validator.h/.cpp   # Piece-specific movement rules
│       └── special_move_validator.h/.cpp     # Castling, en passant, promotion
├── rendering/                                # User interface and rendering layer
│   ├── chess_gui.h/.cpp                      # Main GUI coordinator
│   └── components/                           # Modular UI rendering components
│       ├── board_comp.h/.cpp                 # Board rendering component
│       ├── board_renderer.h/.cpp             # Core board drawing logic
│       ├── piece_renderer.h/.cpp             # Piece drawing and positioning
│       ├── coordinate_renderer.h/.cpp        # Board coordinate labels
│       ├── captured_pieces_renderer.h/.cpp   # Captured pieces display
│       ├── stats_panel.h/.cpp                # Game statistics panel
│       ├── moves_comp.h/.cpp                 # Move history display
│       ├── controls_comp.h/.cpp              # Control instructions panel
│       ├── engine_comp.h/.cpp                # Engine analysis display
│       ├── game_overlay.h/.cpp               # Game over overlays
│       ├── texture_manager.h/.cpp            # Resource and texture management
│       └── ui_renderer.h/.cpp                # General UI rendering utilities
├── input/                                    # Input handling and processing
│   ├── chess_input_handler.h/.cpp            # Mouse and keyboard input processing
├── config/                                   # Configuration management
│   └── config.h                              # Namespace-organized configuration constants
└── assets/                                   # Game resources and textures
    ├── board.png                             # Chess board texture
    └── chess_pieces/                         # Individual piece sprites
        ├── wb.png, wk.png, wn.png, wp.png, wq.png, wr.png  # White pieces
        └── bb.png, bk.png, bn.png, bp.png, bq.png, br.png  # Black pieces
```

## 🏗 Architecture

The application implements a **Component-Based Architecture** with MVC principles and external process integration:

### Core Components

1. **ChessAnalysisProgram** (Controller): Orchestrates all game components and manages UCI engine integration
2. **ChessBoard** (Model): Board state and piece management with captured piece tracking
3. **ChessGameState**: Current game state with castling rights, en passant, and move counters
4. **FENPositionTracker**: Position history management with undo/redo capabilities
5. **ChessMoveValidator**: Advanced validation system with specialized validator components
6. **ChessGameStateAnalyzer**: Dedicated game ending detection (checkmate, stalemate, draws)
7. **UCIEngine**: Chess engine management with Stockfish integration and analysis parsing
8. **Component-Based GUI**: Modular rendering system with specialized UI components
9. **ChessInputHandler**: Sophisticated input processing with drag-and-drop support
10. **Config Namespace**: Centralized configuration management with namespace organization

### UCI Engine Integration

- **UCIEngine**: Main engine coordinator and analysis manager
- **UCICommunication**: Protocol-level communication with chess engines
- **UCIProcess**: Process management and lifecycle control for external engines
- **UCIAnalysisParser**: Engine output parsing and evaluation extraction

### Modular UI Components

- **BoardRenderer**: Core board and piece drawing logic
- **CoordinateRenderer**: Board coordinate labels and annotations
- **CapturedPiecesRenderer**: Visual display of captured pieces
- **StatsPanel**: Game statistics and position information
- **MovesComponent**: Move history with navigation support
- **EngineComponent**: Real-time engine analysis display
- **ControlsComponent**: User interface instructions
- **GameOverlay**: Game ending notifications and overlays
- **TextureManager**: Centralized resource and asset management

### Architectural Benefits

- **Component Isolation**: Each UI component is self-contained and reusable
- **Engine Abstraction**: UCI protocol abstraction allows different engine integration
- **Clear Separation**: Game logic, rendering, and analysis are completely separate
- **Extensible Validation**: Multiple validator types for different rule categories
- **Resource Management**: Centralized texture and asset management
- **State Management**: Comprehensive position tracking with history support

### Advanced Data Flow

```
User Input → InputHandler → Controller → MoveValidator → Board/GameState
                ↓              ↓            ↓              ↓
           Controller → GameStateAnalyzer → GUI Components → Screen Output
                ↓              ↓                ↓
         UCI Engine → Analysis Parser → Engine Component
                ↓              ↓                ↓
      Position History → FEN Tracker → Stats Panel
                ↓
           Config System → All Components
```

### Key Design Features

- **Component Architecture**: Modular, reusable UI components with clear responsibilities
- **UCI Integration**: Full Universal Chess Interface protocol implementation
- **Process Management**: External chess engine lifecycle and communication control
- **Advanced Validation**: Multiple specialized validator types for different rule categories
- **Position Tracking**: Complete game history with FEN-based position management
- **Resource Management**: Centralized texture loading and caching system
- **Configuration System**: Namespace-organized compile-time constants
- **Modern C++ Features**: Smart pointers, RAII, move semantics, and template usage
- **Clean Dependencies**: Unidirectional dependency flow with no circular references

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
g++ -fdiagnostics-color=always -g src/main.cpp src/analysis_engine/*.cpp src/application/chess_analysis_program.cpp src/core/board/chess_board.cpp src/core/game_state/*.cpp src/core/*.cpp src/core/validators/*.cpp src/rendering/chess_gui.cpp src/rendering/components/*.cpp src/input/chess_input_handler.cpp -o main.exe -I C:/raylib/include -L C:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
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
5. **Engine Analysis**: Toggle Stockfish engine analysis for position evaluation and move suggestions
6. **Move History**: View complete game notation with navigation through move history
7. **Undo/Redo**: Navigate through game history with full position restoration
8. **Board Flip**: Toggle board orientation to play from different perspectives
9. **Game Statistics**: View half-move clock, current board position (FEN notation), and captured pieces
10. **Game Endings**: The game automatically detects and displays:
    - Checkmate (winner declared)
    - Stalemate (draw)
    - 50-move rule (draw)
    - Threefold repetition (draw)
    - Insufficient material (draw)
11. **Visual Feedback**: Coordinate labels, captured pieces display, game over screens, and engine analysis

### **Keyboard Controls**
- **X**: Toggle Stockfish engine analysis on/off
- **F**: Flip board orientation
- **R**: Reset game to starting position
- **LEFT**: Undo last move
- **RIGHT**: Redo move (if available)
- **ESC**: Exit application

## 🧠 Learning Outcomes

This project demonstrates proficiency in:

### C++ Programming Concepts
- Object-Oriented Programming (OOP) with component-based design
- Design Patterns (Component Architecture, Observer, Strategy)
- Advanced memory management with smart pointers and RAII
- STL containers and algorithms (std::array, std::vector, std::unordered_map)
- Move semantics, perfect forwarding, and copy constructors
- Template programming and type traits
- Namespace management with organized configuration system
- Type aliases and using declarations for clean code
- Compile-time constants with constexpr and template metaprogramming
- Header/implementation file organization with forward declarations
- Process management and inter-process communication
- External library integration and API abstraction

### Software Engineering Practices
- Component-based architecture with modular, reusable components
- Clean architecture principles with clear abstraction layers
- External process integration and lifecycle management
- Protocol implementation (UCI - Universal Chess Interface)
- Configuration management with namespace organization
- Dependency injection and inversion principles
- Single Responsibility Principle (SRP) across all components
- Extensible architecture supporting multiple chess engines
- Professional error handling and robust validation systems
- Resource management and texture caching systems
- Code documentation and comprehensive commenting
- Version control with Git and build automation
- Cross-platform considerations and portability

### Game Development & Algorithms
- Advanced game logic implementation with complete chess rule compliance
- Complex state management with position history and undo/redo systems
- Algorithmic game analysis (checkmate detection, legal move generation)
- Chess engine communication and UCI protocol implementation
- External process management and inter-process communication
- Graphics programming with component-based rendering system
- Event handling and sophisticated drag-and-drop input processing
- Hash-based position tracking with FEN string management
- Real-time analysis integration with chess engine evaluation
- Performance optimization for real-time gameplay and engine communication
- Resource management with texture caching and loading systems

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

### UCI Engine Integration
```cpp
// UCI chess engine communication and analysis
class UCIEngine {
public:
    void enable() { isEnabled = true; startEngine(); }
    void disable() { isEnabled = false; stopEngine(); }
    void setPosition(const std::string& fenString) {
        communication.sendCommand("position fen " + fenString);
    }
    EngineAnalysis pollAnalysis() const {
        return analysisParser.parseLatestAnalysis();
    }
private:
    UCIProcess engineProcess;
    UCICommunication communication;
    UCIAnalysisParser analysisParser;
    bool isEnabled = false;
};
```

### Component-Based Rendering System
```cpp
// Modular UI component architecture
class ChessGUI {
public:
    void render(const ChessAnalysisProgram& app) {
        boardComp.render(app);
        capturedPiecesRenderer.render(app);
        statsPanel.render(app);
        movesComp.render(app);
        engineComp.render(app);
        controlsComp.render(app);
        gameOverlay.render(app);
    }
private:
    BoardComp boardComp;
    CapturedPiecesRenderer capturedPiecesRenderer;
    StatsPanel statsPanel;
    MovesComp movesComp;
    EngineComp engineComp;
    ControlsComp controlsComp;
    GameOverlay gameOverlay;
    TextureManager textureManager;
};
```

### Advanced Position History System
```cpp
// Complete undo/redo system with position state tracking
class FENPositionTracker {
public:
    void savePosition(const PositionState& state) {
        positionHistory.push_back(state);
        redoPositions.clear(); // Clear redo stack when new move is made
    }
    
    bool undoMove() {
        if (positionHistory.size() > 1) {
            redoPositions.push_back(positionHistory.back());
            positionHistory.pop_back();
            return true;
        }
        return false;
    }
    
    bool redoMove() {
        if (!redoPositions.empty()) {
            positionHistory.push_back(redoPositions.back());
            redoPositions.pop_back();
            return true;
        }
        return false;
    }
private:
    std::vector<PositionState> positionHistory;
    std::vector<PositionState> redoPositions;
};
```

### Component-Based Architecture Pattern
- **Controller**: Orchestrates game logic, engine communication, and UI coordination
- **Model Components**: ChessBoard, ChessGameState, FENPositionTracker for state management
- **View Components**: Modular rendering components with specialized responsibilities
- **Engine Integration**: UCI protocol abstraction with process management
- **Validator Architecture**: Multiple specialized validators for different rule categories

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
- **Component-Based Design**: Modular, reusable UI components with clear interfaces
- **UCI Protocol Implementation**: Full chess engine communication standard
- **Process Management**: External engine lifecycle and inter-process communication
- **Single Responsibility**: Each component has one clear, well-defined purpose
- **SOLID Principles**: Well-designed interfaces with dependency injection
- **Configuration Management**: Namespace-organized system with compile-time constants
- **Clean Dependencies**: Unidirectional dependency flow with no circular references

### **Modern C++ Implementation**
- **STL Mastery**: Advanced use of containers, algorithms, and iterators
- **Smart Pointers**: std::unique_ptr for engine and GUI resource management
- **Process Management**: External executable integration with proper lifecycle control
- **Template Programming**: Type-safe generic programming and template specialization
- **Modern Features**: constexpr, auto, structured bindings, and range-based for loops
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
- [x] **Undo/Redo System**: Full position history with move reversal capabilities

### **UCI Chess Engine Integration** ✅ **COMPLETE**
- [x] **Stockfish Integration**: Embedded chess engine with UCI protocol communication
- [x] **Real-time Analysis**: Live position evaluation and move suggestions
- [x] **Engine Management**: Start, stop, and configure engine analysis
- [x] **Process Control**: External engine lifecycle and communication management
- [x] **Analysis Parser**: Engine output parsing and evaluation extraction

### **Component-Based Architecture** ✅ **COMPLETE**
- [x] **Modular UI System**: Component-based rendering with specialized UI elements
- [x] **Configuration System**: Namespace-organized centralized config management
- [x] **Resource Management**: Centralized texture loading and caching system
- [x] **Input Processing**: Advanced drag-and-drop with mouse input handling
- [x] **Game State Management**: Sophisticated state tracking with history support
- [x] **Clean Dependencies**: Unidirectional dependency flow with no circular references

### **Professional User Interface** ✅ **COMPLETE**
- [x] **Interactive Chess Board**: High-quality board with coordinate labels and piece textures
- [x] **Move History Panel**: Complete game notation with navigation support
- [x] **Engine Analysis Display**: Real-time Stockfish evaluation and move suggestions
- [x] **Statistics Panel**: Game state, FEN notation, and move counters
- [x] **Captured Pieces Display**: Visual tracking with organized layout
- [x] **Game Over Overlays**: Professional game ending notifications
- [x] **Board Flip Feature**: Toggle orientation for different player perspectives
- [x] **Controls Panel**: User interface instructions and keyboard shortcuts

## 🚧 Potential Future Enhancements

### Advanced Features
- [x] ~~Move history with undo/redo functionality~~ **COMPLETED**
- [x] ~~Real-time chess engine analysis~~ **COMPLETED**  
- [ ] Chess notation export (PGN format)
- [ ] Multiple chess engine support (beyond Stockfish)
- [ ] Opening book integration with engine analysis
- [ ] Endgame tablebase integration
- [ ] Network multiplayer support
- [ ] Tournament management system
- [ ] Position analysis with engine depth control
- [ ] Move annotation and commentary system

### Technical Improvements
- [ ] Unit testing framework integration
- [ ] Performance profiling and optimization
- [ ] Cross-platform build system (CMake)
- [ ] Audio system for move feedback
- [ ] Save/load game functionality (PGN support)
- [ ] Logging system for engine communication
- [ ] Configuration file system for user preferences
- [ ] Multiple UCI engine support and configuration

## 📊 Technical Specifications

- **Resolution**: 1920x1080 (Full HD)
- **Frame Rate**: 60 FPS target
- **Board Size**: Scalable (currently 50% of texture size)
- **Piece Assets**: 12 individual PNG sprites with texture management
- **Chess Engine**: Stockfish executable embedded in analysis_engine directory
- **UCI Protocol**: Full Universal Chess Interface implementation
- **Memory Management**: Smart pointers, RAII, and component-based allocation
- **Configuration**: Namespace-organized compile-time constants
- **Components**: 12+ modular UI rendering components
- **Position History**: Complete FEN-based game state tracking with undo/redo

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
