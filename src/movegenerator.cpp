#include "../headers/movegenerator.h"
#include "../headers/utilities.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <cassert>

/*
0000 0000 0000 0000 0000 0011 1111 -> From 0x3F
0000 0000 0000 0000 1111 1100 0000 -> To >> 6, 0x3F
0000 0000 0000 1111 0000 0000 0000 -> Captured >> 12, 0xF
0000 0000 0001 0000 0000 0000 0000 -> EP 0x10000
0000 0000 0010 0000 0000 0000 0000 -> Pawn Start 0x20000
0000 0011 1100 0000 0000 0000 0000 -> Promoted Piece >> 18, 0xF
0000 0100 0000 0000 0000 0000 0000 -> Castle 0x400000
1111 1000 0000 0000 0000 0000 0000 -> Move Score/Value 0xF800000
*/

constexpr INT32 VictimScore[] {
  0,
  100, 200, 300, 400, 500, 600, 0,
  100, 200, 300, 400, 500, 600, 0
};

SliderAttacks sliderAttacks;
INT32 MvvLvaTable[PIECE_TYPE_SIZE][PIECE_TYPE_SIZE];

void (*generateMovesFunctions[PIECE_TYPE_SIZE]) (const Board& board, const UINT32 from, std::vector<MoveScore>& v);

void MoveGenerator::initializeGenerateMovesFunctionsArray() {
  generateMovesFunctions[WHITE_PAWN] = generateMovesFunctions[BLACK_PAWN] = generatePawnMoves;
  generateMovesFunctions[WHITE_KNIGHT] = generateMovesFunctions[BLACK_KNIGHT] = generateKnightMoves;
  generateMovesFunctions[WHITE_BISHOP] = generateMovesFunctions[BLACK_BISHOP] = generateBishopMoves;
  generateMovesFunctions[WHITE_ROOK] = generateMovesFunctions[BLACK_ROOK] = generateRookMoves;
  generateMovesFunctions[WHITE_QUEEN] = generateMovesFunctions[BLACK_QUEEN] = generateQueenMoves;
  generateMovesFunctions[WHITE_KING] = generateMovesFunctions[BLACK_KING] = generateKingMoves;
}

void MoveGenerator::initializeMvvLvaTable() {
  memset(MvvLvaTable, 0, sizeof(MvvLvaTable));

  for (UINT32 attacker = WHITE_PAWN; attacker < BLACK; ++attacker) {
    for (UINT32 victim = WHITE_PAWN; victim < BLACK; ++victim) {
      MvvLvaTable[victim][attacker] = VictimScore[victim] + 6 - ( VictimScore[attacker] / 100);
    }
  }
}

void MoveGenerator::printMove(const UINT32 move) {
  printf("from = %d, to = %d, capturedPiece = %c, isEnPassantMove = %d, isPawnStart = %d,\
  promotedPiece = %c, isCasleMove = %d\n", getFromSquare(move), getToSquare(move), PieceTypeToChar[getCapturedPiece(move)],
  getIsEnPassantMove(move), getIsPawnStart(move), PieceTypeToChar[getPromotedPiece(move)],
  getIsCastleMove(move));
}

UINT32 MoveGenerator::createMove(const UINT32 from, const UINT32 to,
  const UINT32 capturedPiece, const bool isEnPassantMove, const bool isPawnStart,
  const UINT32 promotedPiece, const bool isCastleMove, const UINT32 score) {

  UINT32 move{0};
  setFromSquare(move, from), setToSquare(move, to), setCapturedPiece(move, capturedPiece);
  setIsEnPassantMove(move, isEnPassantMove), setIsPawnStart(move, isPawnStart);
  setPromotedPiece(move, promotedPiece), setIsCastleMove(move, isCastleMove);
  //setScore(move, score);

  return move;
}

inline void MoveGenerator::addMove(const Board& board, UINT32 move, std::vector<MoveScore>& result) {
  MoveScore moveScore{move, MvvLvaTable[getCapturedPiece(move)][board.getPieceFromSquare(getFromSquare(move))] + 1000000};

  if (!getCapturedPiece(move)) { // quite move
    if (board.getKillers(0) == move) {
      moveScore.score = 900000;
    } else if (board.getKillers(1) == move) {
      moveScore.score = 800000;
    } else {
      moveScore.score = board.getHistory(board.getPieceFromSquare(getFromSquare(move)), getToSquare(move));
    }
  }

  result.push_back(moveScore);
}

void MoveGenerator::generateMoves(const Board& board, std::vector<MoveScore>& result) {
  UINT32 end   = board.getSideToMove();
  UINT32 begin = (end == WHITE) ? WHITE_PAWN : BLACK_PAWN;

  for (UINT32 pieceType = begin; pieceType < end; ++pieceType) {
    UINT64 bitboard = board.getBitboard(pieceType);

    while (bitboard) {
      UINT32 from = Utilities::getSquare(bitboard);
      bitboard ^= PowersOfTwo[from];
      generateMovesFunctions[pieceType](board, from, result);
    }
  }

}

void MoveGenerator::generateKingMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v) {
  UINT64 kingAttacks = KingAttacks[from];
  kingAttacks &= ~board.getMovingSideBitboard();

  while (kingAttacks) {
    UINT32 to = Utilities::getSquare(kingAttacks);
    kingAttacks ^= PowersOfTwo[to];
    addMove(board, createMove(from, to, board.getPieceFromSquare(to)), v);
  }

  // reduce branching!!!
  if (from == E1 && board.getSideToMove() == WHITE && !(board.getAttacksTo(board.getBitboard(BOTH), from) & ~board.getMovingSideBitboard())) {
    if (board.hasCastlingRights(WHITE_KING_SIDE_CASTLE) && board.getPieceFromSquare(7) == WHITE_ROOK) {
      if (!(board.getBitboard(BOTH) & 0x60)) {
        if (!(board.getAttacksTo(board.getBitboard(BOTH), from + 1) & ~board.getMovingSideBitboard())) {
          if (!(board.getAttacksTo(board.getBitboard(BOTH), from + 2) & ~board.getMovingSideBitboard())) {
            addMove(board, createMove(from, from + 2, 0, 0, 0, 0, 1), v);
          }
        }
      }
    }
    if (board.hasCastlingRights(WHITE_QUEEN_SIDE_CASTLE) && board.getPieceFromSquare(0) == WHITE_ROOK) {
      if (!(board.getBitboard(BOTH) & 0xE)) {
        if (!((board.getAttacksTo(board.getBitboard(BOTH), from - 1) | board.getAttacksTo(board.getBitboard(BOTH), from - 2)) & ~board.getMovingSideBitboard())) {
          addMove(board, createMove(from, from - 2, 0, 0, 0, 0, 1), v);
        }
      }
    }
  } else if (from == E8 && board.getSideToMove() == BLACK && !(board.getAttacksTo(board.getBitboard(BOTH), from) & ~board.getMovingSideBitboard())) {
    if (board.hasCastlingRights(BLACK_KING_SIDE_CASTLE) && board.getPieceFromSquare(63) == BLACK_ROOK) {
      if (!(board.getBitboard(BOTH) & 0x6000000000000000)) {
        if (!((board.getAttacksTo(board.getBitboard(BOTH), from + 1) | board.getAttacksTo(board.getBitboard(BOTH), from + 2)) & ~board.getMovingSideBitboard())) {
          addMove(board, createMove(from, from + 2, 0, 0, 0, 0, 1), v);
        }
      }
    }
    if (board.hasCastlingRights(BLACK_QUEEN_SIDE_CASTLE) && board.getPieceFromSquare(56) == BLACK_ROOK) {
      if (!(board.getBitboard(BOTH) & 0xE00000000000000)) {
        if (!((board.getAttacksTo(board.getBitboard(BOTH), from - 1) | board.getAttacksTo(board.getBitboard(BOTH), from - 2)) & ~board.getMovingSideBitboard())) {
          addMove(board, createMove(from, from - 2, 0, 0, 0, 0, 1), v);
        }
      }
    }
  }

}

void MoveGenerator::generateQueenMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v) {
  UINT64 queenAttacks = sliderAttacks.QueenAttacks(board.getBitboard(BOTH), from);
  queenAttacks &= ~board.getMovingSideBitboard();

  while (queenAttacks) {
    UINT32 to = Utilities::getSquare(queenAttacks);
    queenAttacks ^= PowersOfTwo[to];
    addMove(board, createMove(from, to, board.getPieceFromSquare(to)), v);
  }
}

void MoveGenerator::generateRookMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v) {
  UINT64 rookAttacks = sliderAttacks.RookAttacks(board.getBitboard(BOTH), from);
  rookAttacks &= ~board.getMovingSideBitboard();

  while (rookAttacks) {
    UINT32 to = Utilities::getSquare(rookAttacks);
    rookAttacks ^= PowersOfTwo[to];
    addMove(board, createMove(from, to, board.getPieceFromSquare(to)), v);
  }
}

void MoveGenerator::generateKnightMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v) {
  UINT64 knigthAttacks = KnigthAttacks[from];
  knigthAttacks &= ~board.getMovingSideBitboard();

  while (knigthAttacks) {
    UINT32 to = Utilities::getSquare(knigthAttacks);
    knigthAttacks ^= PowersOfTwo[to];
    addMove(board, createMove(from, to, board.getPieceFromSquare(to)), v);
  }
}

void MoveGenerator::generateBishopMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v) {
  UINT64 bishopAttacks = sliderAttacks.BishopAttacks(board.getBitboard(BOTH), from);
  bishopAttacks &= ~board.getMovingSideBitboard();

  while (bishopAttacks) {
    UINT32 to = Utilities::getSquare(bishopAttacks);
    bishopAttacks ^= PowersOfTwo[to];
    addMove(board, createMove(from, to, board.getPieceFromSquare(to)), v);
  }
}

void MoveGenerator::generatePawnMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v) {
  UINT32 sideToMove = board.getSideToMove();
  UINT32 pawnType = (sideToMove == WHITE) ? WHITE_PAWN : BLACK_PAWN;

  // generatePawnAttacks
  UINT64 pawnAttacks = (pawnType == WHITE_PAWN) ? WhitePawnAttacks[from] : BlackPawnAttacks[from];
  pawnAttacks &= board.getBitboard(pawnType == WHITE_PAWN ? BLACK : WHITE);

  while (pawnAttacks) {
    UINT32 to = Utilities::getSquare(pawnAttacks);
    pawnAttacks ^= PowersOfTwo[to];

    UINT32 capturedPiece = board.getPieceFromSquare(to);

    if (to < 8 || to > 55) { // promotion!!!
      if (pawnType == WHITE_PAWN) {
        for (UINT32 promPiece = WHITE_KNIGHT; promPiece < WHITE_KING; ++promPiece) {
          addMove(board, createMove(from, to, capturedPiece, 0, 0, promPiece), v);
        }
      } else {
        for (UINT32 promPiece = BLACK_KNIGHT; promPiece < BLACK_KING; ++promPiece) {
          addMove(board, createMove(from, to, capturedPiece, 0, 0, promPiece), v);
        }
      }
      continue;
    }

    addMove(board, createMove(from, to, capturedPiece), v);
  }

  // generatePawnMoves
  if (sideToMove == WHITE) {
    if (!board.getPieceFromSquare(from + 8)) {
      if (from > 47) { // handle promotion
        for (UINT32 promPiece = WHITE_KNIGHT; promPiece < WHITE_KING; ++promPiece) {
          addMove(board, createMove(from, from + 8, 0, 0, 0, promPiece), v);
        }
      } else {
        addMove(board, createMove(from, from + 8), v);
      }

      if(from < 16 && !board.getPieceFromSquare(from + 16)) { // double move at the beginning
        addMove(board, createMove(from, from + 16, 0, 0, 1), v);
      }
    }
  } else { // black pawns
    if (from > 7 && !board.getPieceFromSquare(from - 8)) {
      if (from < 16) { // handle promotion
        for (UINT32 promPiece = BLACK_KNIGHT; promPiece < BLACK_KING; ++promPiece) {
          addMove(board, createMove(from, from - 8, 0, 0, 0, promPiece), v);
        }
      } else {
        addMove(board, createMove(from, from - 8), v);
      }

      if(from > 47 && from < 56 && !board.getPieceFromSquare(from - 16)) { // double move at the beginning
        addMove(board, createMove(from, from - 16, 0, 0, 1), v);
      }
    }
  }

  // add enpassant moves
  UINT32 enPassantSquare = board.getEnPassantSquare();
  UINT64 enPassantMove = (sideToMove == WHITE ? (WhitePawnEnPassant[from] & PowersOfTwo[enPassantSquare]) : (BlackPawnEnPassant[from] & PowersOfTwo[enPassantSquare]));
  if (enPassantMove) {
    v.push_back(MoveScore{createMove(from, enPassantSquare, 0, 1), 105 + 1000000});
  }
}
