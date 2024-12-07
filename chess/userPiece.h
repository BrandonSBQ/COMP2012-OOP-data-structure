#ifndef __USERPIECE_H__
#define __USERPIECE_H__

#include "piece.h"
#include "board.h"
#include "utility.h"

bool isRoyal(const Piece* piece);

// The following 2 classes are given as example
using Knight = OmniLeaper<'N', 2, 1>;

using Rook = OmniRider<'R', 1, 0>;

// TASK 4.1: Define the 3 classes Bishop, Queen, King similar to above
// TODO
using Bishop = OmniRider<'B', 1, 1>;
using Queen = Compound<'Q', Rook, Bishop>;
using King = Compound<'K',OmniLeaper<'K',1,1>,OmniLeaper<'K',1,0>>;


// TASK 4.3: Define the Pawn class
// TODO
using P_C = Compound<'P',Leaper<'p',1,1>,Leaper<'P',-1,1>>;

class Pawn:public Divergent<'P',Leaper<'P',0,1>,P_C>{
    public:
        Pawn(Color color): Divergent<'P',Leaper<'P',0,1>,P_C>(color) {};
        Pawn* clone() const{
            Pawn* newp = new Pawn(this->getColor());
            (*newp)=(*this);
            return newp;
        }

        BooleanMap getMoves(const Board& board) const;
};



#endif // __USERPIECE_H__