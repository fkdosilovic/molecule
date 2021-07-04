#include "../headers/fenparser.h"
#include "../headers/utilities.h"
#include <cctype>
#include <cstdio>

UINT64 FENParser::parseRanks(Board& board, const std::string& fen, UINT64 index) {
  int rank = 7;
  int file = 0;

  while (fen[index] != ' ') {
    if (fen[index] == '/') {
      --rank, file = 0;
      ++index;
      continue;
    }

    if (isdigit(fen[index])) {
      file += (fen[index] - '0');
      ++index;
      continue;
    }


    if (file < 8) {
      UINT64 pieceType = CharToPieceType[static_cast<int>(fen[index])];
      int square = Utilities::RankFileToSquare(rank, file);
      board.setBitboards(pieceType, square);
      board.addToPieceList(pieceType, square);
      board.setPieceOnSquare(pieceType, square);
      board.hashPiece(pieceType, square);
    }

    ++index, ++file;
  }

  return (index + 1);
}

UINT64 FENParser::parseSideToMove(Board& board, const std::string& fen, UINT64 index) {
  board.setSideToMove((fen[index] == 'w' ? WHITE : BLACK));
  if (board.getSideToMove() == BLACK) {
    board.hashSide();
  }
  return (index + 2);
}

UINT64 FENParser::parseCastlingRights(Board& board, const std::string& fen, UINT64 index) {
  while (fen[index] != ' ') {
    if (fen[index] == 'K') board.addCastlingPermission(WHITE_KING_SIDE_CASTLE);
    else if (fen[index] == 'Q') board.addCastlingPermission(WHITE_QUEEN_SIDE_CASTLE);
    else if (fen[index] == 'k') board.addCastlingPermission(BLACK_KING_SIDE_CASTLE);
    else if (fen[index] == 'q') board.addCastlingPermission(BLACK_QUEEN_SIDE_CASTLE);
    ++index;
  }
  board.hashCastlePermission();
  return (index + 1);
}

UINT64 FENParser::parseEnPassantSquare(Board& board, const std::string& fen, UINT64 index) {
  if (fen[index] == '-') {
    return index + 2;
  }

  int file = fen[index++] - 'a';
  int rank = fen[index++] - '1';

  board.setEnPassantSquare(Utilities::RankFileToSquare(rank, file));
  board.hashEnPassantSquare();
  return (index + 1);
}

void FENParser::parseHalfMoveAndFullMoveClock(Board& board, const std::string& fen, UINT64 index) {

  if (index >= fen.size() || fen[index] == '-') {
    return;
  }

  int index2 = fen.find_first_of(' ', index);
  int halfMove = std::stoi(&fen[index]);
  int fullMove = std::stoi(&fen[index2]);

  board.setNumberOfHalfMovesSinceLastPawnCaptureAdvance(halfMove);
  board.setNumberOfHalfMoves(fullMove / 2 + (fullMove % 2));
}

void FENParser::parseFEN(Board& board, const std::string& fen) {
  UINT64 index = parseRanks(board, fen);
  index = parseSideToMove(board, fen, index);
  index = parseCastlingRights(board, fen, index);
  index = parseEnPassantSquare(board, fen, index);
  parseHalfMoveAndFullMoveClock(board, fen, index);
}
