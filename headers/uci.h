#ifndef MOLECULE_UCI_H
#define MOLECULE_UCI_H

#include "constants.h"
#include "zobrist.h"
#include "movegenerator.h"
#include "makemove.h"
#include "transpositiontable.h"
#include <string>
#include <cstdio>

class UCI {

public:
  static void uciLoop();

private:
  static void parseGoCommand(std::string command, Board& board, SearchInfo& searchInfo, TT& transpositionTable);
  static void parsePositionCommand(std::string command, Board& board, SearchInfo& searchInfo);

};

#endif // MOLECULE_UCI_H
