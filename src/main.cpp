#include "../headers/movegenerator.h"
#include "../headers/board.h"
#include "../headers/zobrist.h"
#include "../headers/makemove.h"
#include "../headers/search.h"
#include "../headers/uci.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>

std::mutex searchInfoMutex;

int main(int argc, char* argv[]) {

  Zobrist::generateZobristKeys(PieceHashKeys, &SideToMoveKey, CastlePermissionKeys, EnPassantKeys);
  MoveGenerator::initializeGenerateMovesFunctionsArray();
  MoveGenerator::initializeMvvLvaTable();
  sliderAttacks.Initialize();
  
  std::thread uciThread{UCI::uciLoop};
  uciThread.join();

  return 0;
}
