#ifndef MOLECULE_UTILITIES_H
#define MOLECULE_UTILITIES_H

#include "constants.h"
#include "board.h"
#include <chrono>
#include <string>

/**
  * Utility class is used for various purposes in Molecule.
  * */
class Utilities {

public:

  /**
    * Returns a unsigned 32-bit integer that represents a square denoted by (rank, file).
    * \param rank square's rank in range 0 - 7
    * \param file square's file in range 0 - 7
    * */
  static UINT32 RankFileToSquare(const UINT64 rank, const UINT64 file);

  // Returns first bit that is set
  static UINT32 getSquare(UINT64 bitboard) {
    return __builtin_ctzl(bitboard);
  }

  static void printBitboard(const UINT64 bitboard);
  static std::string moveToString(const UINT32 move);

  // Returns time in milliseconds
  static UINT64 getTimeInMilliseconds() {
    return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
  }

  static UINT32 parseMove(Board& board, const std::string move);

};

#endif // MOLECULE_UTILITIES_H
