#include "../headers/transpositiontable.h"

UINT32 TT::getPvLine(Board& board, const UINT32 depth) {
  UINT32 move = probe(board);
  UINT32 count = 0;

  while (move > 0 && count < depth) {
    if (!MakeMove::moveExists(board, move)) {
      break;
    }

    MakeMove::makeMove(board, move);
    board.addToPVArray(count, move);
    ++count;
    move = probe(board);
  }

  while (board.getNumberOfHalfMoves() > 0) {
      MakeMove::undoMove(board);
  }

  return count;
}