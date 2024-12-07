// TASK 3: Implement the 4 Piece-derived classes here
// All classes will inherit the NamedPiece class
// and implement the remaining pure virtual functions
#ifndef PIECE_TO_IMPLEMENT_H
#define PIECE_TO_IMPLEMENT_H
#include "piece.h"
#include "utility.h"
#include "board.h"

// TODO 3.1: Leaper class
template <char N, int F, int R>
class Leaper : public NamedPiece<N>
{
public:
    // Constructor: Initializes the base class with the color
    Leaper(Color color) : NamedPiece<N>(color) {}

    // Clone function: Creates a new dynamic instance of Leaper using the copy constructor
    virtual Piece *clone() const override
    {
        return new Leaper(*this);
    }

    virtual BooleanMap getMoves(const Board &board) const override
    {

        Vector move;
        if (this->color == WHITE)
        {
            move.file = F;
            move.rank = R;
        }
        else
        {
            move.file = -F;
            move.rank = -R;
        }
        Position destination = move + this->position;
        BooleanMap moveMap;
        const Piece *des = board.piece(destination);
        if (des != nullptr && des->getColor() == this->color)
        {
            return moveMap;
        }
        moveMap.cell(destination) = true;
        return moveMap;
    }
};


// TODO 3.2: Rider class
template <char N, int F, int R, int RANGE>
class Rider : public NamedPiece<N>
{
public:
    // Constructor: Initializes the base class with the color
    Rider(Color color) : NamedPiece<N>(color) {}

    // Clone function: Creates a new dynamic instance of Rider using the copy constructor
    Piece *clone() const override
    {
        return new Rider(*this);
    }

    BooleanMap getMoves(const Board &board) const override
    {
        
        Vector move;

        if (this->color == WHITE)
        {
            move.file = F;
            move.rank = R;
        }
        else
        {
            move.file = -F;
            move.rank = -R;
        }
        BooleanMap moveMap;

        Position current = this->position;

        for (int i = 0; i < RANGE; ++i)
        {
             // Move in the specified direction
            Position new_D = move+current;

            //const Piece *destPiece = board.piece(current);

            if(new_D.file < _A ||new_D.file > _H || new_D.rank >_8 || new_D.rank< _1){return moveMap;}

            
            // if (destPiece == nullptr)
            // {
            //     moveMap.cell(current) = true; // Add position if it is empty
            // }
            // else
            // {
            //     if (destPiece->getColor() != this->color)
            //     {
            //         moveMap.cell(current) = true; // Capture if it's an opponent's piece
            //     }
            //     break; // Stop moving if there is a piece
            // }
            if(board.piece(new_D) && board.piece(new_D)->getColor() != this->color){
                moveMap.cell(new_D) = 1;
                return moveMap;
            }
            if(board.piece(new_D) && board.piece(new_D)->getColor() == this->color){
                return moveMap;
            }
            moveMap.cell(new_D) = 1;
            current = new_D;
        }

        return moveMap;
    }
};


template <char N, typename P1, typename P2>
class Compound : public NamedPiece<N>
{
public:
    // Constructor: Initializes the base class with the color
    Compound(Color color) : NamedPiece<N>(color) {}

    // Clone function: Creates a new dynamic instance of Compound using the copy constructor
    Piece *clone() const override
    {
        return new Compound(*this);
    }

    // getMoves function: Combines the movesets of P1 and P2
    BooleanMap getMoves(const Board &board) const override
    {
        // Dynamically create temporary P1 and P2 objects with the same color as the Compound piece
        P1 *tempP1 = new P1(this->getColor());
        P2 *tempP2 = new P2(this->getColor());

        // Set the positions for temporary pieces to the current position of the Compound piece
        tempP1->setPosition(this->getPosition());
        tempP2->setPosition(this->getPosition());

        // Get the movesets of both component pieces using a temporary board
        Board tempBoard1 = board.getTempBoard(tempP1, this->getPosition());
        Board tempBoard2 = board.getTempBoard(tempP2, this->getPosition());

        BooleanMap movesP1 = tempP1->getMoves(tempBoard1);
        BooleanMap movesP2 = tempP2->getMoves(tempBoard2);

        // Combine the movesets using bitwise OR
        BooleanMap combinedMoves = movesP1 |= movesP2; // Use bitwise OR to combine the moves

        return combinedMoves;
    }
};

template <char N, typename M, typename C>
class Divergent : public NamedPiece<N>
{
public:
    // Constructor: Initializes the base class with the color
    Divergent(Color color) : NamedPiece<N>(color) {}

    // Clone function: Creates a new dynamic instance of Divergent using the copy constructor
    Piece *clone() const override
    {
        return new Divergent(*this);
    }

    // Get moves function: Moves like M, captures like C
    BooleanMap getMoves(const Board &board) const override
    {
        // // Create temporary M and C objects with the same color as the Divergent piece
        // M *movePiece = new M(this->getColor());
        // C *capturePiece = new C(this->getColor());

        // // Set the positions for temporary pieces to the current position of the Divergent piece
        // movePiece->setPosition(this->getPosition());
        // capturePiece->setPosition(this->getPosition());

        // // Get the movesets of both M and C
        // BooleanMap movesM = movePiece->getMoves(board);
        // BooleanMap movesC = capturePiece->getMoves(board);

        // // Get the map of opponent pieces from the board
        // BooleanMap opponentMap = board.getOpponentMap(this->getColor());

        // // Calculate the valid move positions (non-capture moves)
        // BooleanMap nonCaptureMoves = movesM;
        // nonCaptureMoves &= ~opponentMap; // Remove opponent-occupied squares from non-capture moves

        // // Calculate the capture positions (where C can move and is an opponent piece)
        // BooleanMap captureMoves = movesC;
        // captureMoves &= opponentMap; // Keep only opponent-occupied squares for capture moves

        // // Combine the non-capture and capture moves
        // BooleanMap finalMoves = nonCaptureMoves |= captureMoves;

        // return finalMoves;
        M move(this->color);
        C capture(this->color);
        move.setPosition(this->position);
        capture.setPosition(this->position);
        BooleanMap captureMap = capture.getMoves(board);
        bool isWhite = (this->color == WHITE);
        captureMap &= board.getOpponentMap(isWhite);
        BooleanMap moveMap = move.getMoves(board);
        moveMap &= ~board.getOpponentMap(isWhite);
        BooleanMap combined = captureMap;
        combined |= moveMap;
        return combined;
    }
};


#endif