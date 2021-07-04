#ifndef MOLECULE_MAKEMOVE_H
#define MOLECULE_MAKEMOVE_H

#include "atomicdatatypes.h"
#include "constants.h"
#include "board.h"
#include "movegenerator.h"
#include "utilities.h"

class MakeMove {

private:
  static void movePiece(Board& board, const UINT32 from, const UINT32 to);
  static void clearPiece(Board& board, const UINT32 square);
  static void addPiece(Board& board, const UINT32 square, const UINT32 piece);

public:
  static bool makeMove(Board& board, const UINT32 move);
  static void undoMove(Board& board);

  static void makeNullMove(Board& board);
  static void undoNullMove(Board& board);

  static bool moveExists(Board& board, const UINT32 move);
  
};

#endif // MOLECULE_MAKEMOVE_H
