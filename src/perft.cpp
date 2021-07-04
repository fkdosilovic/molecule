#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include "../headers/board.h"
#include "../headers/constants.h"
#include "../headers/movegenerator.h"
#include "../headers/utilities.h"
#include "../headers/makemove.h"
#include "../headers/zobrist.h"

std::vector<UINT32> officialResults;
Board board{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};

UINT64 perft(int depth) {

  if (depth == 0)  {
    return 1;
  }

  std::vector<MoveScore> moves;
  MoveGenerator::generateMoves(board, moves);

  UINT64 nodes{0};
  for (MoveScore move : moves) {
    if (MakeMove::makeMove(board, move.move)) {
      nodes += perft(depth - 1);
      MakeMove::undoMove(board);
    }
  }

  return nodes;
}

int main(int argc, char* argv[]) {
  sliderAttacks.Initialize();
  MoveGenerator::initializeGenerateMovesFunctionsArray();
  MoveGenerator::initializeMvvLvaTable();
  //Zobrist::generateZobristKeys();

  char fen[1000];

  int boardNum = 1;
  int depth = 6;

  long double total_number_of_seconds{0.0};
  UINT64 total_number_of_moves{0ULL};

  // get official results
  UINT64 num;
  FILE* f = fopen("perft_full_official.out", "r");
  while(fscanf(f, "%lld", &num) != EOF) {
    officialResults.push_back(num);
  }
  fclose(f);

  FILE* fp = fopen("perft_full.in", "r");
  FILE* fpo = fopen("perft_full.out", "w");

  while (fscanf(fp, " %[^\n]", fen) != EOF) {

    board.setUpBoard(fen);

    clock_t begin = clock();
    UINT64 number_of_moves = perft(depth);
    clock_t end = clock();

    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    printf("Board %d: number of moves for depth 6 is %lu in %.4lf seconds. (%s)\n",
            boardNum, number_of_moves, elapsed_secs, officialResults[boardNum - 1] == number_of_moves ?
                                                  "GOOD" : "BAD");

    total_number_of_moves += number_of_moves;
    total_number_of_seconds += elapsed_secs;

    fprintf(fpo, "Board %d: number of moves for depth 6 is %lu in %.4lf seconds.\n", boardNum, number_of_moves, elapsed_secs);

    ++boardNum;
  }
  fprintf(fpo, "\nMolecule can generate approx. %lu moves per second.\n", static_cast<UINT64>(total_number_of_moves / total_number_of_seconds));

  fclose(fp);
  fclose(fpo);

  std::cout << "\nMolecule can generate approx. " << static_cast<UINT64>(total_number_of_moves / total_number_of_seconds)
            << " per second.\n";

  return 0;
}
