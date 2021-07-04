#include "../headers/uci.h"
#include "../headers/search.h"
#include <thread>
#include <functional>
#include <vector>
#include <iostream>

void UCI::uciLoop() {
  std::cout.rdbuf()->pubsetbuf(nullptr, 0);
  std::cin.rdbuf()->pubsetbuf(nullptr, 0);

  TT transpositionTable;
  Board board{START_FEN};
  SearchInfo searchInfo;
  std::string command;

  searchInfo.stopped = false;
  searchInfo.quit = false;

  while (true) {
    std::cout << std::flush;
    std::cin >> command;

    if (command == "isready") {
      std::cout << "readyok" << std::endl;
      //continue;
    } else if (command == "position") {
      parsePositionCommand(command, board, searchInfo);
    } else if (command == "ucinewgame") {
      board.setUpBoard(START_FEN);
      transpositionTable.clear();
    } else if (command == "go") {
      parseGoCommand(command, board, searchInfo, transpositionTable);
    } else if (command == "quit") {
      searchInfoMutex.lock();
      searchInfo.quit = true;
      searchInfoMutex.unlock();
      //break;
    } else if (command == "uci") {
      std::cout << "id name " << NAME << std::endl;
      std::cout << "id author " << AUTHOR << std::endl;
      std::cout << "uciok" << std::endl;
    } else if (command == "stop") {
      searchInfoMutex.lock();
      searchInfo.stopped = true;
      searchInfoMutex.unlock();
    }

    searchInfoMutex.lock();
    if (searchInfo.quit) {
      break;
    }
    searchInfoMutex.unlock();
  }

  searchInfoMutex.unlock();
}

void UCI::parseGoCommand(std::string command, Board& board, SearchInfo& searchInfo, TT& transpositionTable) {
  char c;
  std::string cmd, num;

  INT32 depth = -1, movesToGo = 30, moveTime = -1;
  INT32 timeT = -1, inc = 0;

  searchInfoMutex.lock();
  searchInfo.timeSet = false;
  searchInfo.stopped = false;
  searchInfo.quit = false;
  searchInfoMutex.unlock();

  while (true) {
    std::cout << std::flush;
    std::cin >> cmd;

    if (cmd != "infinite") {
      std::cout << std::flush;
      std::cin >> num;
    }

    if (cmd == "binc" && board.getSideToMove() == BLACK) inc = atoi(num.c_str());
    else if (cmd == "winc" && board.getSideToMove() == WHITE) inc = atoi(num.c_str());
    else if (cmd == "btime" && board.getSideToMove() == BLACK) timeT = atoi(num.c_str());
    else if (cmd == "wtime" && board.getSideToMove() == WHITE) timeT = atoi(num.c_str());
    else if (cmd == "movestogo") movesToGo = atoi(num.c_str());
    else if (cmd == "movetime") moveTime = atoi(num.c_str());
    else if (cmd == "depth") depth = atoi(num.c_str());

    c = getchar();
    if (c == '\n') {
      break;
    }
  }

  if (~moveTime) {
    timeT = moveTime;
    movesToGo = 1;
  }

  searchInfoMutex.lock();

  searchInfo.startTime = Utilities::getTimeInMilliseconds();
  searchInfo.depthLimit = depth;

  if (~timeT) {
    searchInfo.timeSet = true;
    timeT /= movesToGo;
    timeT -= 25;
    searchInfo.endTime = searchInfo.startTime + timeT + inc;
  }

  if (depth == -1) {
    searchInfo.depthLimit = MAX_DEPTH;
  }
  searchInfoMutex.unlock();

  std::thread searchThread{Search::search, std::ref(board), std::ref(searchInfo), std::ref(transpositionTable)};
  searchThread.detach();
}

void UCI::parsePositionCommand(std::string command, Board& board, SearchInfo& searchInfo) {
  std::string moveStr;
  std::cin >> command;

  if (command == "startpos") {
    board.setUpBoard(START_FEN);
  } else if (command == "fen") {
    command = "";
    std::cin >> moveStr;
    command += moveStr + " ";
    std::cin >> moveStr;
    command += moveStr + " ";
    std::cin >> moveStr;
    command += moveStr + " ";
    std::cin >> moveStr;
    command += moveStr + " ";
    std::cin >> moveStr;
    command += moveStr + " ";
    std::cin >> moveStr;
    command += moveStr;

    board.setUpBoard(command);
  }

  char c = getchar();

  if (c != '\n') {
    std::cin >> command;
  }

  if (command == "moves") {
    while (std::cin >> moveStr) {
      UINT32 move = Utilities::parseMove(board, moveStr);
      MakeMove::makeMove(board, move);

      c = getchar();
      if (c == '\n') break;
    }
  }

  board.setNumberOfHalfMoves(0);
  //board.print();
}
