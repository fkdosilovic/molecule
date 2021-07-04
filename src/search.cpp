#include "../headers/evaluate.h"
#include "../headers/utilities.h"
#include "../headers/makemove.h"
#include "../headers/search.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

inline void Search::pickNextMove(UINT32 index, std::vector<MoveScore>& moves) {
  UINT32 bestIndex = index;
  INT32 bestScore = 0;

  for (UINT32 i = index; i < moves.size(); ++i) {
    if (moves[i].score > bestScore) {
      bestScore = moves[i].score;
      bestIndex = i;
    }
  }

  MoveScore temp = moves[index];
  moves[index] = moves[bestIndex];
  moves[bestIndex] = temp;
}

INT32 Search::alphaBeta(INT32 alpha, INT32 beta, UINT32 depth, Board& board,
  SearchInfo& info, bool doNullMove, TT& transpositionTable) {

  if (!depth) {
    return quiescenceSearch(alpha, beta, board, info, transpositionTable);
  }

  searchInfoMutex.lock();
  if ((info.numberOfNodes & NODE_CHECK) == 0) {
    check(info);
  }
  info.numberOfNodes++;
  searchInfoMutex.unlock();

  if (board.getNumberOfHalfMoves() && (board.isRepetition() || board.getNumberOfHalfMovesSinceLastPawnCaptureAdvance() >= 100)) {
    return 0;
  }

  if (board.getNumberOfHalfMoves() > (MAX_DEPTH - 1)) {
    return Evaluate::evaluatePosition(board);
  }

  //position fen 1n5k/3q3p/pp1p2pB/5r2/1PP1Qp2/P6P/6P1/2R3K1 w - - 0 1
  UINT32 square = Utilities::getSquare(board.getBitboard(board.getMovingSideKing()));
  bool inCheck = board.getAttacksTo(board.getBitboard(BOTH), square)
                 & (~board.getMovingSideBitboard());
  depth += inCheck;

  UINT32 pvMove = 0;
  INT32 score = transpositionTable.get(board, &pvMove, alpha, beta, depth);

  if (score > -INFINITE) {
    return score;
  }

  // null move
  if (doNullMove && !inCheck && board.getNumberOfHalfMoves() > 0 && depth > 3) {
    MakeMove::makeNullMove(board);
    score = -alphaBeta(-beta, -beta + 1, depth - 4, board, info, false, transpositionTable);
    MakeMove::undoNullMove(board);

    searchInfoMutex.lock();
    if (info.stopped) {
      searchInfoMutex.unlock();
      return 0;
    }
    searchInfoMutex.unlock();

    if (score >= beta) {
      return score;
    }
  }

  std::vector<MoveScore> moves;
  MoveGenerator::generateMoves(board, moves);

  UINT32 legalMoves = 0;
  INT32 alphaOld = alpha;
  UINT32 bestMove = 0;
  INT32 bestScore = -INFINITE;

  score = -INFINITE;

  if (pvMove) {
    for (UINT32 i = 0; i < moves.size(); ++i) {
      if (pvMove == moves[i].move) {
        moves[i].score = 10000000;
        break;
      }
    }
  }

  for (UINT32 i = 0; i < moves.size(); ++i) {
    pickNextMove(i, moves);

    if (!MakeMove::makeMove(board, moves[i].move)) {
      continue;
    }

    ++legalMoves;
    score = -alphaBeta(-beta, -alpha, depth - 1, board, info, true, transpositionTable);
    MakeMove::undoMove(board);

    searchInfoMutex.lock();
    if (info.stopped) {
      searchInfoMutex.unlock();
      return 0;
    }
    searchInfoMutex.unlock();

    if (score > bestScore) {
      bestScore = score;
      bestMove = moves[i].move;

      if (score > alpha) {
        if (score >= beta) {
          searchInfoMutex.lock();
          if (legalMoves == 1) {
            info.failHighFirst++;
          }
          info.failHigh++;
          searchInfoMutex.unlock();

          if (!MoveGenerator::getCapturedPiece(moves[i].move)) {
            board.addToKillers(moves[i].move);
          }

          transpositionTable.add(board, bestMove, beta, HFBETA, depth);
          return beta;
        }

        alpha = score;

        if (!MoveGenerator::getCapturedPiece(moves[i].move)) {
          board.addToHistory(board.getPieceFromSquare(MoveGenerator::getFromSquare(bestMove)),
          MoveGenerator::getToSquare(bestMove), depth);
        }
      }
    }

  }

  if (!legalMoves) {
    UINT32 square = Utilities::getSquare(board.getBitboard(board.getMovingSideKing()));
    bool inCheck = board.getAttacksTo(board.getBitboard(BOTH), square) & (~board.getMovingSideBitboard());
    return inCheck * (-MATE + board.getNumberOfHalfMoves());
  }

  if (alpha != alphaOld) {
    transpositionTable.add(board, bestMove, bestScore, HFEXACT, depth);
  } else {
    transpositionTable.add(board, bestMove, alpha, HFALPHA, depth);
  }

  return alpha;
}

INT32 Search::quiescenceSearch(INT32 alpha, INT32 beta, Board& board, SearchInfo& info, TT& transpositionTable) {

  searchInfoMutex.lock();
  if ((info.numberOfNodes & NODE_CHECK) == 0) {
    check(info);
  }
  info.numberOfNodes++;
  searchInfoMutex.unlock();

  if (board.isRepetition() || board.getNumberOfHalfMovesSinceLastPawnCaptureAdvance() >= 100) {
    return 0;
  }

  if (board.getNumberOfHalfMoves() > MAX_DEPTH - 1) {
    return Evaluate::evaluatePosition(board);
  }

  INT32 score = Evaluate::evaluatePosition(board);

  if (score >= beta) {
    return beta;
  }

  if (score > alpha) {
    alpha = score;
  }

  std::vector<MoveScore> moves;
  MoveGenerator::generateMoves(board, moves);

  UINT32 legalMoves = 0;
  score = -INFINITE;

  for (UINT32 i = 0; i < moves.size(); ++i) {

    if (!MoveGenerator::getCapturedPiece(moves[i].move)) {
      continue;
    }

    pickNextMove(i, moves);

    if (!MakeMove::makeMove(board, moves[i].move)) {
      continue;
    }

    ++legalMoves;
    score = -quiescenceSearch(-beta, -alpha, board, info, transpositionTable);
    MakeMove::undoMove(board);

    searchInfoMutex.lock();
    if (info.stopped) {
      searchInfoMutex.unlock();
      return 0;
    }
    searchInfoMutex.unlock();

    if (score > alpha) {
      if (score >= beta) {
        searchInfoMutex.lock();
        if (legalMoves == 1) {
          info.failHighFirst++;
        }
        info.failHigh++;
        searchInfoMutex.unlock();

        return beta;
      }
      alpha = score;
    }
  }

  return alpha;
}

void Search::check(SearchInfo& info) {
  if (info.timeSet && Utilities::getTimeInMilliseconds() > info.endTime) {
    info.stopped = true;
  }
}

void Search::clear(Board& board, SearchInfo& info) {
  board.clearSearchHistory();
  board.clearSearchKillers();

  board.setNumberOfHalfMoves(0);

  info.stopped = false;
  info.numberOfNodes = 0;
  info.failHigh = 0;
  info.failHighFirst = 0;
}

void Search::search(Board& board, SearchInfo& info, TT& transpositionTable) {
  UINT32 bestMove = 0;
  INT32 bestScore = -INFINITE;

  searchInfoMutex.lock();
  clear(board, info);
  UINT32 depthLimit = info.depthLimit;
  searchInfoMutex.unlock();

  for (UINT32 depth = 1; depth <= depthLimit; ++depth) {
    bestScore = alphaBeta(-INFINITE, INFINITE, depth, board, info, true, transpositionTable);

    searchInfoMutex.lock();
    if (info.stopped) {
      searchInfoMutex.unlock();
      break;
    }
    searchInfoMutex.unlock();

    UINT32 pvMoves = transpositionTable.getPvLine(board, depth);
    bestMove = board.getFromPVArray();

    searchInfoMutex.lock();
    std::cout << "info score cp " << bestScore << " depth " << depth << " nodes " << info.numberOfNodes
              << " time " << (Utilities::getTimeInMilliseconds() - info.startTime);
    searchInfoMutex.unlock();

    pvMoves = transpositionTable.getPvLine(board, depth);

    std::cout << " pv ";
    for (UINT32 pvNum = 0; pvNum < pvMoves; ++pvNum) {
      std::cout << Utilities::moveToString(board.getFromPVArray(pvNum)) << " ";
    }
    std::cout << std::endl;

    //printf("ordering: %.2lf\n\n", (1.0f * info.failHighFirst) / info.failHigh);
  }

  std::cout << "bestmove " << Utilities::moveToString(bestMove) << std::endl;
  searchInfoMutex.unlock();
}
