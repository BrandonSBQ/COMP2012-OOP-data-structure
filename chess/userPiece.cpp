#include "userPiece.h"
#include "piece.h"
#include "board.h"
#include "utility.h"
// TASK 4.2: isRoyal(const Piece*)
// TODO
bool isRoyal(const Piece* piece) {
    const King* king = dynamic_cast<const King*>(piece);
    return king != nullptr;
}


// TASK 4.3: Implement any Pawn function(s) here if needed
BooleanMap Pawn::getMoves(const Board& board) const {
    BooleanMap moves;
    int direction = (this->getColor() == WHITE) ? 1 : -1;
    Position currentPosition = this->getPosition();
    _FILE file = currentPosition.file;
    _RANK rank = currentPosition.rank;

    // Forward move (1 step)
    _RANK forwardRank = static_cast<_RANK>(rank + direction);
    if (forwardRank >= 0 && forwardRank < NUM_RANKS && board.piece(file, forwardRank) == nullptr) {
        moves.cell(file, forwardRank) = true;

        // Check for initial two-step move
        if ((this->getColor() == WHITE && rank == static_cast<_RANK>(1)) || (this->getColor() == BLACK && rank == static_cast<_RANK>(6))) {
            _RANK twoStepRank = static_cast<_RANK>(rank + 2 * direction);
            if (board.piece(file, twoStepRank) == nullptr) {
                moves.cell(file, twoStepRank) = true;
            }
        }
    }

    // Capture moves (diagonal)
    int captureFiles[] = {file - 1, file + 1};
    for (int captureFile : captureFiles) {
        if (captureFile >= 0 && captureFile < NUM_FILES) {
            _RANK captureRank = static_cast<_RANK>(rank + direction);
            const Piece* targetPiece = board.piece(static_cast<_FILE>(captureFile), captureRank);
            if (targetPiece != nullptr && targetPiece->getColor() != this->getColor()) {
                moves.cell(static_cast<_FILE>(captureFile), captureRank) = true;
            }
        }
    }

    return moves;
}