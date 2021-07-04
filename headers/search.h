#ifndef MOLECULE_SEARCH_H
#define MOLECULE_SEARCH_H

#include "constants.h"
#include "board.h"
#include "movegenerator.h"
#include "transpositiontable.h"
#include <vector>

class Search {

private:
  static void pickNextMove(UINT32 index, std::vector<MoveScore>& moves);
  static INT32 alphaBeta(INT32 alpha, INT32 beta, UINT32 depth, Board& board, SearchInfo& info, bool doNullMove, TT& transpositionTable);
  static INT32 quiescenceSearch(INT32 alpha, INT32 beta, Board& board, SearchInfo& info, TT& transpositionTable);

public:
  static void check(SearchInfo& info);
  static void clear(Board& board, SearchInfo& info);
  static void search(Board& board, SearchInfo& info, TT& transpositionTable);

};

#endif // MOLECULE_SEARCH_H
