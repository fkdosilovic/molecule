#include "../headers/evaluate.h"

#define POPCNT(X) __builtin_popcountll(X)

INT32 Evaluate::basicEvaluation(const Board& board) {
  INT32 score{0};

  for (UINT32 i = WHITE_PAWN; i < WHITE; ++i) {
    score += POPCNT(board.getBitboard(i)) * PieceValues[i];
  }

  for (UINT32 i = BLACK_PAWN; i < BLACK; ++i) {
    score -= POPCNT(board.getBitboard(i)) * PieceValues[i];
  }
  
  for (UINT32 pieceType = WHITE_PAWN; pieceType < WHITE; ++pieceType) {
    for (UINT32 j = 1; j < board.pieceList[pieceType][0]; ++j) {
      score += PieceSquareTables[pieceType][board.pieceList[pieceType][j]];
    }
  }

  for (UINT32 pieceType = BLACK_PAWN; pieceType < BLACK; ++pieceType) {
    for (UINT32 j = 1; j < board.pieceList[pieceType][0]; ++j) {
      score -= PieceSquareTables[pieceType][Mirror[board.pieceList[pieceType][j]]];
    }
  }

  return score * (board.getSideToMove() == WHITE ? 1 : -1);
}

INT32 Evaluate::evaluatePosition(const Board& board) {
  return basicEvaluation(board);
}
