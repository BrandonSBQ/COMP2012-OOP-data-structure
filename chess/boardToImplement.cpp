#include <iostream>
#include "board.h"
#include "piece.h"
using namespace std;

/**
 * TASK 2.1: Board copy constructor
*/
Board::Board(const Board& board):isWhiteTurn(board.isWhiteTurn),moveMap(board.moveMap)
{
    for(int r = 0; r < NUM_RANKS; r++){
        for(int f = 0; f < NUM_FILES; f++){
            if(board.pieces[r][f] != nullptr){
                pieces[r][f] = board.pieces[r][f]->clone();
            }else{
                pieces[r][f] = nullptr;
            }
        }
    }

    if(board.selectedPiece != nullptr){
        //Position pos = board.selectedPiece->getPosition();
        //selectedPiece = pieces[pos.rank][pos.file];
        selectedPiece = piece(board.selectedPiece->getPosition());
        
    }else{
        selectedPiece = nullptr;
    }

    for(int i = 0; i < 2; i++){
        if(board.royalPieces[i] != nullptr){
            Position pos = board.royalPieces[i]->getPosition();
            royalPieces[i] = pieces[pos.rank][pos.file];
        }else{
            royalPieces[i] = nullptr;
        }
    }
}

/**
 * TASK 2.2: Board destructor
*/
Board::~Board() 
{
    for (int r = 0; r < NUM_RANKS; ++r) {
        for (int f = 0; f < NUM_FILES; ++f) {
            delete pieces[r][f];
            pieces[r][f] = nullptr;
        }
    }
}

/**
 * TASK 5.1: Board::move(const Position&)
*/
void Board::move(const Position& destPos)
{
    
    if (!selectedPiece) {
        cout << "ERROR: Piece not selected, cannot call move()" << endl;
        return;
    }

    // TODO
    
    // Piece* destPiece = piece(destPos);

    
    // if (destPiece) {
    //         delete destPiece;
    // }

    // Position currentPos = selectedPiece->getPosition();


    // destPiece = selectedPiece;
    // piece(currentPos.file, currentPos.rank) = nullptr; 


    // destPiece->setPosition(destPos);
    if(piece(destPos)){
        delete piece(destPos);
    }
    piece(destPos)= selectedPiece;
    piece(selectedPiece->getPosition()) = nullptr;

    selectedPiece->setPosition(destPos);
    

    isWhiteTurn = !isWhiteTurn;
    

}

/**
 * TASK 5.2: Board::getAttackingMap() const
*/
// BooleanMap Board::getAttackingMap() const
// {
//     // TODO
//     BooleanMap attackMap; 

//     // Loop through all positions on the board
//     for (int r = 0; r < NUM_RANKS; r++) {
//         for (int f = 0; f < NUM_FILES; f++) {
//             Piece* currentPiece = pieces[r][f];
            
         
//             if (currentPiece != nullptr && (currentPiece->isWhite() == isWhiteTurn)) {
                
//                 BooleanMap pieceMoves = currentPiece->getMoves(*this);

           
//                 BooleanMap opponentMap = getOpponentMap(!isWhiteTurn);

  
//                 BooleanMap captureMap = pieceMoves &= ~opponentMap;

                
//                 attackMap |= captureMap;
//             }
//         }
//     }

    
//     return attackMap;
// }
BooleanMap Board::getAttackingMap() const {
//     BooleanMap attackMap;

//     // Loop through all positions on the board
//     for (int r = 0; r < NUM_RANKS; r++) {
//         for (int f = 0; f < NUM_FILES; f++) {
//             Position pos = {_FILE(f), _RANK(r)};
//             const Piece* piece = this->piece(pos);

//             // Check if there is a piece at the current position and if it belongs to the current player
//             if (piece && (piece->isWhite() == this->isWhiteTurn)) {
//                 // Get all possible moves for this piece
//                 BooleanMap pieceMoves = piece->getMoves(*this);

//                 // Get the map of opponent's pieces
//                 BooleanMap opponentMap = this->getOpponentMap(!isWhiteTurn);

//                 // Calculate which moves are captures
//                 BooleanMap captureMap = pieceMoves |= opponentMap;

//                 // Combine the capture map into the overall attack map
//                 attackMap |= captureMap;
//             }
//         }
//     }
//     return attackMap;
// }
    BooleanMap finalMap;
    BooleanMap OpponentMap = getOpponentMap(isWhiteTurn);
    for(_RANK i:{_1,_2,_3,_4,_5,_6,_7,_8}){
        for(_FILE j:{_A,_B,_C,_D,_E,_F,_G,_H}){
            if(piece(j,i) && (piece(j,i)->isWhite() == isWhiteTurn)){
                BooleanMap curMoveMap;
                curMoveMap = piece(j,i)->getMoves(*this);
                finalMap = (finalMap|=(curMoveMap&=OpponentMap));
            }
        }
    }
    return finalMap;
}
/**
 * TASK 5.3: Board::validateMoveMap()
*/
// void Board::validateMoveMap() {
    
//     if (!selectedPiece) {
       
//         return; 
//     }

    
//     if (!royalPieces[isWhiteTurn ? 0 : 1]) {
       
//         return; 
//     }

    
//     Position royalPosition = royalPieces[isWhiteTurn ? 0 : 1]->getPosition();
//     BooleanMap validMoveMap;

    
//     for (int r = 0; r < NUM_RANKS; r++) {
//         for (int f = 0; f < NUM_FILES; f++) {
//             if (moveMap.cell(static_cast<_FILE>(f), static_cast<_RANK>(r))) {
               
//                 Board tempBoard(*this);
//                 _FILE fileEnum = static_cast<_FILE>(f);
//                 _RANK rankEnum = static_cast<_RANK>(r);
//                 Position pos = {fileEnum, rankEnum};

                
//                 tempBoard.move(pos);  

               
//                 BooleanMap attackMap = tempBoard.getAttackingMap();

                
                
//                     validMoveMap.cell(fileEnum, rankEnum) = true; 
//                 }
                
//             }
//         }
//     }
//     moveMap = validMoveMap;
// }


void Board::validateMoveMap() {
    
    // if (!selectedPiece) {
    //     return; 
    // }

    // if (!royalPieces[isWhiteTurn ? 0 : 1]) {
    //     return; 
    // }

    // // Note: Initial royal position is no longer needed here.
    // //Position royalPosition = royalPieces[isWhiteTurn ? 0 : 1]->getPosition();
    // BooleanMap validMoveMap;

    // for (int r = 0; r < NUM_RANKS; r++) {
    //     for (int f = 0; f < NUM_FILES; f++) {
    //         if (moveMap.cell(static_cast<_FILE>(f), static_cast<_RANK>(r))) {
               
    //             Board tempBoard(*this);
    //             _FILE fileEnum = static_cast<_FILE>(f);
    //             _RANK rankEnum = static_cast<_RANK>(r);
    //             Position pos = {fileEnum, rankEnum};

    //             tempBoard.move(pos);  

    //             Position tempRoyalPosition = tempBoard.royalPieces[tempBoard.isWhiteTurn ? 0 : 1]->getPosition();

              
    //             BooleanMap attackMap = tempBoard.getAttackingMap();

              
    //             if (!attackMap.cell(tempRoyalPosition.file, tempRoyalPosition.rank)) {
    //                 validMoveMap.cell(fileEnum, rankEnum) = true; 
    //             }
                
    //         }
    //     }
    // }
    // // Update the moveMap with only the valid moves
    // moveMap = validMoveMap;
    if(!this->royalPieces[isWhiteTurn]) return;
    for(int i = 0; i < NUM_RANKS; i++){
        for(int j = 0; j < NUM_FILES; j++){
            if(moveMap.cell(static_cast<_FILE>(j),static_cast<_RANK>(i)) == true){
                Board temp(*this);
                Position dest = {static_cast<_FILE>(j),static_cast<_RANK>(i)};
                temp.move(dest);
                BooleanMap oppo = temp.getAttackingMap();
                if(oppo.cell(temp.royalPieces[isWhiteTurn]->getPosition())){
                    moveMap.cell(dest) = false;
                }
            }
        }
    }
}
// void Board::validateMoveMap() {
//     // Temporary board for simulation
//     Board tempBoard(*this);

//     // Iterate over each possible move in moveMap
//     for (int f = 0; f < NUM_FILES; f++) {
//         for (int r = 0; r < NUM_RANKS; r++) {
//             if (moveMap.cell(static_cast<_FILE>(f),static_cast<_RANK>(r))) {  // Check if the move is initially considered valid
//                 // Simulate the move on the temporary board
//                 Position destPos{static_cast<_FILE>(f), static_cast<_RANK>(r)};
//                 tempBoard.move(destPos);

//                 // Get the attacking map from the perspective of the opponent after the move
//                 BooleanMap attackingMap = tempBoard.getAttackingMap();

//                 // Determine the position of the current player's royal piece
//                 Piece* royalPiece = nullptr;
//                 for (int i = 0; i < 2; i++) {
//                     if (tempBoard.royalPieces[i] && (tempBoard.royalPieces[i]->isWhite() == tempBoard.isWhiteTurn)) {
//                         royalPiece = tempBoard.royalPieces[i];
//                         break;
//                     }
//                 }

//                 if (royalPiece != nullptr) {
//                     Position royalPos = royalPiece->getPosition();

//                     // If the royal piece is under attack after the move, mark the move as invalid
//                     if (attackingMap.cell(royalPos.file, royalPos.rank)) {
//                         moveMap.cell(static_cast<_FILE>(f), static_cast<_RANK>(r)) = false;
//                     }
//                 }

//                 // Reset the temporary board to the original state for the next iteration
//                 tempBoard = Board(*this);
//             }
//         }
//     }
// }