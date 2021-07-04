#include "../headers/board.h"
#include "../headers/utilities.h"
#include "../headers/fenparser.h"
#include "../headers/constants.h"
#include <bitset>
#include <iostream>
#include <cassert>
#include <cstdio>

/** Note: Prints a bit representation as: 876543210!!! */
void Board::printBitboard(const UINT64* bitboard, const UINT32 bitboardType) const {
  std::cout << '\n';
  for (int i = 56; i > -1; i -= 8) {
    std::bitset<8> b{bitboard[bitboardType] >> i};
    std::cout << b << '\n';
  }
  std::cout << '\n';
}

void Board::printRegularBitboard(const int bitboardType) const {
  printBitboard(bitboards, bitboardType);
}

Board::Board(const std::string& fen) {
  setUpBoard(fen);
}

Board::~Board() {
}

void Board::setUpBoard(const std::string& fen) {
  positionKey = 0ULL;

  memset(bitboards, 0x0, sizeof(bitboards));
  memset(board, 0x0, sizeof(board));
  memset(pieceList, 0x0, sizeof(pieceList));
  memset(squareToPieceListIndex, 0x0, sizeof(squareToPieceListIndex));

  for (UINT32 i = 0; i < PIECE_TYPE_SIZE; ++i) {
    pieceList[i][0] = 1;
  }

  FENParser::parseFEN(*this, fen);

  // combine white and black pieces
  for (UINT32 i = 0; i < WHITE; ++i) {
    bitboards[WHITE] |= bitboards[i];
    bitboards[BLACK] |= bitboards[i + 7];
  }
  bitboards[BOTH] = bitboards[WHITE] | bitboards[BLACK];

  sliderAttacks.Initialize();
}

void Board::print() const {
  char board[10][10];

  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      board[i][j] = '.';
    }
  }

  std::cout << "\n";
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      for (int j = 1; j < BLACK; ++j) {
        if (j == WHITE) continue;
        if ((bitboards[j] & (1ULL << Utilities::RankFileToSquare(rank, file))) != 0) {
          board[rank][file] = PieceTypeToChar[j];
        }
      }
    }
  }

  // output
  for (int rank = 7; rank > -1; --rank) {
    std::cout << rank + 1 << " ";
    for (int file = 0; file < 8; ++file) {
      std::cout << board[rank][file] << " ";
    }
    std::cout << "\n";
  }

  std::cout << "\n  ";
  for (int file = 0; file < 8; ++file) {
    std::cout << static_cast<char>(file + 'A') << " ";
  }

  std::cout << "\nPosition key: " <<  positionKey << "\n";
}

UINT64 Board::getAttacksTo(UINT64 bitboard, const UINT32 square) const {
  UINT64 knights{0ULL}, kings{0ULL}, bishopsQueens{0ULL}, rooksQueens{0ULL};
  knights = bitboards[WHITE_KNIGHT] | bitboards[BLACK_KNIGHT];
  kings = bitboards[WHITE_KING] | bitboards[BLACK_KING];
  rooksQueens = bishopsQueens = bitboards[WHITE_QUEEN] | bitboards[BLACK_QUEEN];
  rooksQueens |= bitboards[WHITE_ROOK] | bitboards[BLACK_ROOK];
  bishopsQueens |= bitboards[WHITE_BISHOP] | bitboards[BLACK_BISHOP];

  UINT64 result{0ULL};
  result |= (WhitePawnAttacks[square] & bitboards[BLACK_PAWN]);
  result |= (BlackPawnAttacks[square] & bitboards[WHITE_PAWN]);
  result |= (KnigthAttacks[square] & knights);
  result |= (KingAttacks[square] & kings);
  result |= (sliderAttacks.BishopAttacks(bitboard, square) & bishopsQueens) |
            (sliderAttacks.RookAttacks(bitboard, square) & rooksQueens);

  return result;
}
