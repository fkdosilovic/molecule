#include "../headers/makemove.h"
#include "../headers/movegenerator.h"

inline void MakeMove::movePiece(Board& board, const UINT32 from, const UINT32 to) {
  UINT32 piece = board.getPieceFromSquare(from);

  board.updateBitboard(piece, from);
  board.hashPiece(piece, from);
  board.removeFromPieceList(piece, from);
  board.setPieceOnSquare(NONE, from);

  board.updateBitboard(piece, to);
  board.hashPiece(piece, to);
  board.addToPieceList(piece, to);
  board.setPieceOnSquare(piece, to);

}

inline void MakeMove::clearPiece(Board& board, const UINT32 square) {
  UINT32 piece = board.getPieceFromSquare(square);

  board.hashPiece(piece, square);
  board.removeFromPieceList(piece, square);
  board.setPieceOnSquare(NONE, square);
  board.updateBitboard(piece, square);
}

inline void MakeMove::addPiece(Board& board, const UINT32 square, const UINT32 piece) {
  board.hashPiece(piece, square);
  board.addToPieceList(piece, square);
  board.setPieceOnSquare(piece, square);
  board.updateBitboard(piece, square);
}

bool MakeMove::makeMove(Board& board, const UINT32 move) {
  //std::cout << "move = " << move << '\n';
  UINT32 from = MoveGenerator::getFromSquare(move);
  UINT32 to   = MoveGenerator::getToSquare(move);
  UINT32 capturedPiece = MoveGenerator::getCapturedPiece(move);
  bool enPassantMove = MoveGenerator::getIsEnPassantMove(move);
  bool pawnStart = MoveGenerator::getIsPawnStart(move);
  UINT32 promotedPiece = MoveGenerator::getPromotedPiece(move);
  bool castleMove = MoveGenerator::getIsCastleMove(move);

  UINT32 piece = board.getPieceFromSquare(from);
  UINT32 colour = PieceTypeToColour[piece];

  board.addMoveToHistoryArray(move);

  if (enPassantMove) {
    (colour == WHITE) ? clearPiece(board, to - 8) : clearPiece(board, to + 8);
  } else if (castleMove) {
    switch (to) {
      case C1: movePiece(board, A1, D1); break;
      case C8: movePiece(board, A8, D8); break;
      case G1: movePiece(board, H1, F1); break;
      case G8: movePiece(board, H8, F8); break;
    }
  }

  board.hashEnPassantSquare();            // removes current en passant square from pos. key
  board.hashCastlePermission();        // removes current castling permission from pos. key

  board.clearCastlingPermission(CastlePermission[from]);
  board.clearCastlingPermission(CastlePermission[to]);
  board.setEnPassantSquare(BOARD_SIZE);

  board.hashCastlePermission();        // adds current castle perm. to pos. key
  board.increaseFiftyMoveCounter();

  if (capturedPiece) {
    clearPiece(board, to);
    board.resetFiftyMoveCounter();
  }

  board.increasePly();

  if (piece == WHITE_PAWN || piece == BLACK_PAWN) {
    board.resetFiftyMoveCounter();
    if (pawnStart) {
      (colour == WHITE) ? board.setEnPassantSquare(from + 8) : board.setEnPassantSquare(from - 8);
    }
  }

  board.hashEnPassantSquare();

  movePiece(board, from, to);

  if (promotedPiece) {
    clearPiece(board, to);
    addPiece(board, to, promotedPiece);
  }

  board.switchSides();

  board.hashSide();

  UINT32 kingPiece = (colour == WHITE ? WHITE_KING : BLACK_KING);
  if (capturedPiece == WHITE_KING || capturedPiece == BLACK_KING ||
    (board.getAttacksTo(board.getBitboard(BOTH), Utilities::getSquare(board.getBitboard(kingPiece))) & board.getMovingSideBitboard())) {
    undoMove(board);
    return false;
  }

  return true;
}

void MakeMove::undoMove(Board& board) {
  Move move;
  board.getMoveFromHistory(move);

  board.decreasePly();

  UINT32 from = MoveGenerator::getFromSquare(move.move);
  UINT32 to = MoveGenerator::getToSquare(move.move);

  board.hashEnPassantSquare();     // removes current en passant square
  board.hashCastlePermission(); // removes current castle permission

  board.setCastlingPermission(move.castlePermission);
  board.setNumberOfHalfMovesSinceLastPawnCaptureAdvance(move.fiftyMoveCounter);
  board.setEnPassantSquare(move.enPassantSquare);

  board.hashEnPassantSquare(); // adds old enpassant square to hash key
  board.hashCastlePermission(); // adds old castle perm. to hash key 

  board.switchSides();
  board.hashSide();

  if (MoveGenerator::getIsEnPassantMove(move.move)) {
    if (board.getSideToMove() == WHITE) {
      addPiece(board, to - 8, BLACK_PAWN);
    } else {
      addPiece(board, to + 8, WHITE_PAWN);
    }
  } else if (MoveGenerator::getIsCastleMove(move.move)) {
    switch (to) { // create a table
      case C1: movePiece(board, D1, A1); break;
      case C8: movePiece(board, D8, A8); break;
      case G1: movePiece(board, F1, H1); break;
      case G8: movePiece(board, F8, H8); break;
    }
  }

  movePiece(board, to, from);

  UINT32 capturedPiece = MoveGenerator::getCapturedPiece(move.move);
  if (capturedPiece) {
    addPiece(board, to, capturedPiece); // place under if statement if neccessary
  }

  UINT32 promotedPiece = MoveGenerator::getPromotedPiece(move.move);
  if (promotedPiece) {
    clearPiece(board, from);
    addPiece(board, from, (promotedPiece < WHITE ? WHITE_PAWN : BLACK_PAWN));
  }
}

void MakeMove::makeNullMove(Board& board) {
  board.increasePly();
  board.addMoveToHistoryArray(0);

  if (board.getEnPassantSquare() < BOARD_SIZE) {
    board.hashEnPassantSquare();
  }

  board.setEnPassantSquare(BOARD_SIZE);
  board.switchSides();
  board.hashSide();
}

void MakeMove::undoNullMove(Board& board) {
  board.decreasePly();

  if (board.getEnPassantSquare() < BOARD_SIZE) {
    board.hashEnPassantSquare();
  }

  Move move;
  board.getMoveFromHistory(move);

  board.setCastlingPermission(move.castlePermission);
  board.setNumberOfHalfMovesSinceLastPawnCaptureAdvance(move.fiftyMoveCounter);
  board.setEnPassantSquare(move.enPassantSquare);

  if (board.getEnPassantSquare() < BOARD_SIZE) {
    board.hashEnPassantSquare();
  }

  board.switchSides();
  board.hashSide();
}

bool MakeMove::moveExists(Board& board, const UINT32 move) {

  std::vector<MoveScore> moves;
  MoveGenerator::generateMoves(board, moves);

  for (MoveScore m : moves) {
    if (!makeMove(board, m.move)) {
      continue;
    }

    undoMove(board);

    if (m.move == move) {
      return true;
    }
  }

  return false;
}
