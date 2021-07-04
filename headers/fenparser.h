#ifndef MOLECULE_FENPARSER_H
#define MOLECULE_FENPARSER_H

#include "constants.h"
#include "board.h"

class FENParser {

private:
  static UINT64 parseRanks(Board& board, const std::string& fen, UINT64 index = 0);
  static UINT64 parseSideToMove(Board& board, const std::string& fen, UINT64 index);
  static UINT64 parseCastlingRights(Board& board, const std::string& fen, UINT64 index);
  static UINT64 parseEnPassantSquare(Board& board, const std::string& fen, UINT64 index);
  static void parseHalfMoveAndFullMoveClock(Board& board, const std::string& fen, UINT64 index);

public:
  static void parseFEN(Board& board, const std::string& fen);

};

#endif // MOLECULE_FENPARSER_H
