#include <iostream>
#include "utility.h"
using namespace std;

ostream& operator<<(ostream& os, const Position& pos)
{
    os << static_cast<char>(pos.file + 'a') << static_cast<char>(pos.rank + '1');
    return os;
}

// TASK 1: Implement the 6 operator overloading functions of Position, Vector and BooleanMap
// You need to write the full function prototypes and implementations


// TODO 1.1: Position::operator==(const Position&) const
bool Position::operator==(const Position& pos) const{
    if(this->file == pos.file && this->rank == pos.rank){
        return true;
    }

    return false;
}

// // TODO 1.2: Vector::operator+(const Position&) const
// Position Vector::operator+(const Position& pos) const {
//     // Calculate potential new file and rank
//     int newFile = static_cast<int>(pos.file) + file;
//     int newRank = static_cast<int>(pos.rank) + rank;

//     // Check if the new file and rank are within the bounds of the chessboard
//     if (newFile >= 0 && newFile <= 7 && newRank >= 0 && newRank <= 7) {
//         // Valid position within bounds
//         Position newPos;
//         newPos.file = static_cast<_FILE>(newFile);
//         newPos.rank = static_cast<_RANK>(newRank);
//         return newPos;
//     } else {
//         // Out of bounds, return the original position
//         return pos;
//     }
// }
// Function to safely add a Vector to a Position, checking for out-of-bounds scenarios
Position Vector::operator+(const Position& pos) const {
    // Calculate the new file and rank indices, initially as integers
    int newFileIndex = static_cast<int>(pos.file) + this->file;
    int newRankIndex = static_cast<int>(pos.rank) + this->rank;

    // Check if the new file index is within the valid chessboard boundaries
    if (newFileIndex < _A || newFileIndex > _H) {
        // If out of bounds, return the original position
        return pos;
    }

    // Check if the new rank index is within the valid chessboard boundaries
    if (newRankIndex < _1 || newRankIndex > _8) {
        // If out of bounds, return the original position
        return pos;
    }

    // If both indices are within bounds, cast them back to their respective enum types and return the new position
    Position newPos;
    newPos.file = static_cast<_FILE>(newFileIndex);
    newPos.rank = static_cast<_RANK>(newRankIndex);
    return newPos;
}

// TODO 1.3: Vector::operator*(int) const
Vector Vector::operator*(int mul) const{
    return Vector{file * mul, rank * mul};
}
// TODO 1.4: BooleanMap::operator|=(const BooleanMap&)
BooleanMap& BooleanMap::operator|=(const BooleanMap& other){
    for(int r = 0; r < NUM_RANKS; r++){
        for(int f = 0 ; f < NUM_FILES; f++){
            this->cells[r][f] = this->cells[r][f] || other.cells[r][f];
        }
    }

    return *this;
}
// TODO 1.5: BooleanMap::operator&=(const BooleanMap&)
BooleanMap& BooleanMap::operator&=(const BooleanMap& other){
    for(int r = 0; r < NUM_RANKS; r++){
        for(int f = 0 ; f < NUM_FILES; f++){
            this->cells[r][f] = this->cells[r][f] && other.cells[r][f];
        }
    }

    return *this;
}
// TODO 1.6: BooleanMap::operator~() const
BooleanMap BooleanMap::operator~() const {
    BooleanMap result;
    for (int r = 0; r < NUM_RANKS; ++r) {
        for (int f = 0; f < NUM_FILES; ++f) {
            result.cells[r][f] = !this->cells[r][f];
        }
    }
    return result;
}