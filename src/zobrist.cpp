#include "../headers/zobrist.h"
#include <set>

UINT64 PieceHashKeys[PIECE_TYPE_SIZE][BOARD_SIZE];
UINT64 SideToMoveKey;             // for black
UINT64 CastlePermissionKeys[16];  // for each combination one key
UINT64 EnPassantKeys[BOARD_SIZE + 1];          // for each column one key

std::random_device Zobrist::rd;
std::mt19937 Zobrist::gen(Zobrist::rd());
std::uniform_int_distribution<UINT64> Zobrist::dis(1, MAX_UINT64);

void Zobrist::generateZobristKeys(UINT64 (&pieceKeys)[PIECE_TYPE_SIZE][BOARD_SIZE],
                                  UINT64* sideToMoveKey, UINT64* castleKeys,
                                  UINT64* enPassantKeys) {

  std::set<UINT64> keysGenerated;
  generatePieceKeys(pieceKeys, keysGenerated);
  generateSideToMoveKey(sideToMoveKey, keysGenerated);
  generateCastlingRightsKeys(castleKeys, keysGenerated);
  generateEnPassantKeys(enPassantKeys, keysGenerated);
}

void Zobrist::generatePieceKeys(UINT64 (&pieceKeys)[PIECE_TYPE_SIZE][BOARD_SIZE],
                                std::set<UINT64>& keysGenerated) {
  UINT64 randKey{0ULL};
  keysGenerated.insert(randKey);

  for (UINT32 pieceType = WHITE_PAWN; pieceType < BLACK; ++pieceType) {
    if (PieceTypeToChar[pieceType] == '.' || !PieceTypeToChar[pieceType]) {
      continue;
    }

    for (UINT64 square = 0; square < BOARD_SIZE; ++square) {
      while (keysGenerated.find(randKey) != keysGenerated.end()) {
        randKey = dis(gen);
      }

      pieceKeys[pieceType][square] = randKey;
      keysGenerated.insert(randKey);
    }
  }
}

void Zobrist::generateSideToMoveKey(UINT64* key, std::set<UINT64>& keysGenerated) {
  UINT64 randKey{0ULL};
  while (keysGenerated.find(randKey) != keysGenerated.end()) {
    randKey = dis(gen);
  }

  *key = randKey;
  keysGenerated.insert(randKey);
}

void Zobrist::generateCastlingRightsKeys(UINT64* keys, std::set<UINT64>& keysGenerated) {
  UINT64 randKey{0ULL};
  for (UINT32 i = 0; i < 16; ++i) {
    while (keysGenerated.find(randKey) != keysGenerated.end()) {
      randKey = dis(gen);
    }

    keys[i] = randKey;
    keysGenerated.insert(randKey);
  }
}

void Zobrist::generateEnPassantKeys(UINT64* keys, std::set<UINT64>& keysGenerated) {
  UINT64 randKey{0ULL};

  // generate hash keys for white en passant squares
  for (UINT32 i = A4; i < A4; ++i) {
    while (keysGenerated.find(randKey) != keysGenerated.end()) {
      randKey = dis(gen);
    }

    keys[i] = randKey;
    keysGenerated.insert(randKey);
  }

  // generate hash keys for black en passant squares
  for (UINT32 i = A6; i < A7; ++i) {
    while (keysGenerated.find(randKey) != keysGenerated.end()) {
      randKey = dis(gen);
    }

    keys[i] = randKey;
    keysGenerated.insert(randKey);
  }
}
