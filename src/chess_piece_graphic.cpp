#include "chess_piece_graphic.h"

ChessPieceGraphic::ChessPieceGraphic(const ChessPiece& piece, Vector2 position)
    : piece(piece), position(position) {
        // Load the texture for the chess piece based on its type and color
        loadPieceTexture();
        // Define the clickable area based on position and texture size
        pieceRect = Rectangle{ position.x, position.y, static_cast<float>(pieceTexture.width), static_cast<float>(pieceTexture.height) };
    }

ChessPieceGraphic::~ChessPieceGraphic() {
    if (pieceTexture.id != 0) {
        UnloadTexture(pieceTexture);
    }
}

// Copy constructor
ChessPieceGraphic::ChessPieceGraphic(const ChessPieceGraphic& other) 
    : piece(other.piece), position(other.position), pieceRect(other.pieceRect) {
    // Load our own copy of the texture
    loadPieceTexture();
}

// Copy assignment
ChessPieceGraphic& ChessPieceGraphic::operator=(const ChessPieceGraphic& other) {
    if (this != &other) {
        // Unload current texture
        if (pieceTexture.id != 0) {
            UnloadTexture(pieceTexture);
        }
        
        piece = other.piece;
        position = other.position;
        pieceRect = other.pieceRect;
        
        // Load our own copy of the texture
        loadPieceTexture();
    }
    return *this;
}

// Move constructor
ChessPieceGraphic::ChessPieceGraphic(ChessPieceGraphic&& other) noexcept
    : piece(std::move(other.piece)), position(other.position), 
      pieceRect(other.pieceRect), pieceTexture(other.pieceTexture) {
    // Take ownership of the texture
    other.pieceTexture.id = 0; // Invalidate the moved-from object
}

// Move assignment
ChessPieceGraphic& ChessPieceGraphic::operator=(ChessPieceGraphic&& other) noexcept {
    if (this != &other) {
        // Unload current texture
        if (pieceTexture.id != 0) {
            UnloadTexture(pieceTexture);
        }
        
        piece = std::move(other.piece);
        position = other.position;
        pieceRect = other.pieceRect;
        pieceTexture = other.pieceTexture;
        
        // Take ownership of the texture
        other.pieceTexture.id = 0; // Invalidate the moved-from object
    }
    return *this;
}

void ChessPieceGraphic::loadPieceTexture() {
    // Logic to load the correct texture based on piece type and color
    std::string texturePath = "src/assets/chess_pieces/";
    texturePath.append(piece.pieceToString()); // e.g., "wk" for white king
    texturePath.append(".png");

    // Safety check: Unload existing texture if one exists
    if (pieceTexture.id != 0) {
        UnloadTexture(pieceTexture);
    }

    pieceTexture = LoadTexture(texturePath.c_str()); // Chess piece is initially 150x150 px
}

void ChessPieceGraphic::draw(float scale) {
    // Draw the piece texture at its position
    DrawTextureEx(this->pieceTexture, this->position, 0.0f, scale, WHITE);
}

void ChessPieceGraphic::setPosition(Vector2 newPosition) {
    position = newPosition;
    updateClickableArea();
}

void ChessPieceGraphic::setPiece(const ChessPiece& newPiece) {
    piece = newPiece;
    
    // Unload the current texture before loading a new one to prevent memory leak
    UnloadTexture(pieceTexture);
    loadPieceTexture();
    updateClickableArea();
}

void ChessPieceGraphic::updateClickableArea() {
    pieceRect = Rectangle{ position.x, position.y, static_cast<float>(pieceTexture.width), static_cast<float>(pieceTexture.height) };
}