#include "../headers/utilities.h"
#include "../headers/movegenerator.h"
#include <iostream>
#include <bitset>
#include <string>
//#include <x86intrin.h>

UINT32 Utilities::RankFileToSquare(const UINT64 rank, const UINT64 file) {
  return (rank * 8 + file);
}

void Utilities::printBitboard(const UINT64 bitboard) {
  std::cout << '\n';
  for (int i = 56; i > -1; i -= 8) {
    std::bitset<8> b{bitboard >> i};
    std::cout << b << '\n';
  }
  std::cout << '\n';
}

std::string Utilities::moveToString(const UINT32 move) {
  std::string ret{""};
  int from = MoveGenerator::getFromSquare(move);
  int to = MoveGenerator::getToSquare(move);
  int promotedPiece = MoveGenerator::getPromotedPiece(move);

  ret.push_back((from % 8) + 'a'); ret.push_back((from / 8) + '1');
  ret.push_back((to % 8) + 'a'); ret.push_back((to / 8) + '1');
  ret.push_back(PieceTypeToChar[promotedPiece]);
  return ret;
}

inline int absdiff(int x, int y) {
  return (x - y < 0) ? y - x : x - y;
}

UINT32 Utilities::parseMove(Board& board, const std::string move) {
  UINT32 ret = 0;

  int from = RankFileToSquare(move[1] - '1', move[0] - 'a');
  int to = RankFileToSquare(move[3] - '1', move[2] - 'a');

  MoveGenerator::setFromSquare(ret, from);
  MoveGenerator::setToSquare(ret, to);

  if (move.size() == 5) {
    MoveGenerator::setPromotedPiece(ret, CharToPieceType[(UINT32)move[4]]);
  }

  MoveGenerator::setCapturedPiece(ret, board.getPieceFromSquare(to));
  if ((board.getPieceFromSquare(from) == WHITE_KING || board.getPieceFromSquare(from) == BLACK_KING)
      && absdiff(from, to) == 2) {
    MoveGenerator::setIsCastleMove(ret, 1);
    return ret;
  }

  if (board.getPieceFromSquare(from) == WHITE_PAWN || board.getPieceFromSquare(from) == BLACK_PAWN) {
    if (absdiff(from, to) == 16) {
      MoveGenerator::setIsPawnStart(ret, 1);
      return ret;
    }
  }

  MoveGenerator::setIsEnPassantMove(ret, (board.getEnPassantSquare() == to));
  return ret;
}
