#ifndef MOLECULE_ZOBRIST_H
#define MOLECULE_ZOBRIST_H

#include "atomicdatatypes.h"
#include "constants.h"
#include "board.h"
#include <set>
#include <random>

class Zobrist {

public:
  static void generateZobristKeys(UINT64 (&pieceKeys)[PIECE_TYPE_SIZE][BOARD_SIZE],
                                  UINT64* sideToMoveKey, UINT64* castleKeys,
                                  UINT64* enPassantKey);

private:

  static std::random_device rd;
  static std::mt19937 gen;
  static std::uniform_int_distribution<UINT64> dis;

  static void generatePieceKeys(UINT64 (&pieceKeys)[PIECE_TYPE_SIZE][BOARD_SIZE],
                                std::set<UINT64>& keysGenerated);
  static void generateSideToMoveKey(UINT64* key, std::set<UINT64>& keysGenerated);
  static void generateCastlingRightsKeys(UINT64* keys, std::set<UINT64>& keysGenerated);
  static void generateEnPassantKeys(UINT64* keys, std::set<UINT64>& keysGenerated);
};

#endif // MOLECULE_ZOBRIST_H
