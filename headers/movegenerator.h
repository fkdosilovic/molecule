#ifndef MOLECULE_MOVEGENERATOR_H
#define MOLECULE_MOVEGENERATOR_H

#include "../dependency/magic-bits/slider_attacks.h"
#include "board.h"
#include <vector>

class MoveGenerator {

public:
  static void generateMoves(const Board& board, std::vector<MoveScore>& result);
  static void initializeMvvLvaTable();
  static void initializeGenerateMovesFunctionsArray();

  static void setFromSquare(UINT32& move, const UINT32 fromSquare) {
    move |= fromSquare;
  }

  static void setToSquare(UINT32& move, const UINT32 toSquare) {
    move |= (toSquare << 6);
  }

  static void setCapturedPiece(UINT32& move, const UINT32 capturedPiece) {
    move |= (capturedPiece << 12);
  }

  static void setIsEnPassantMove(UINT32& move, const bool isEnPassantMove) {
    move |= (isEnPassantMove << 16);
  }

  static void setIsPawnStart(UINT32& move, const bool isPawnStart) {
    move |= (isPawnStart << 17);
  }

  static void setPromotedPiece(UINT32& move, const UINT32 promotedPiece) {
    move |= (promotedPiece << 18);
  }

  static void setIsCastleMove(UINT32& move, const bool isCastleMove) {
    move |= (isCastleMove << 22);
  }

  static void setScore(UINT32& move, const UINT32 score) {
    move |= (score << 23);
  }

  static UINT32 createMove(const UINT32 from = 0, const UINT32 to = 0,
                           const UINT32 capturedPiece = 0,
                           const bool isEnPassantMove = 0, const bool isPawnStart = 0,
                           const UINT32 promotedPiece = 0, const bool isCastleMove = 0,
                           const UINT32 score = 0);

  static UINT32 getFromSquare(const UINT32 move) {
    return move & 0x3F;
  }

  static UINT32 getToSquare(const UINT32 move) {
    return (move >> 6) & 0x3F;
  }

  static UINT32 getCapturedPiece(const UINT32 move) {
    return (move >> 12) & 0xF;
  }

  static UINT32 getIsEnPassantMove(const UINT32 move) {
    return (move & 0x10000);
  }

  static UINT32 getIsPawnStart(const UINT32 move) {
    return (move & 0x20000);
  }

  static UINT32 getPromotedPiece(const UINT32 move) {
    return (move >> 18) & 0xF;
  }

  static UINT32 getIsCastleMove(const UINT32 move) {
    return (move & 0x400000);
  }

  static UINT32 getScore(const UINT32 move) {
    return (move & 0xF800000);
  }

  static void printMove(const UINT32 move);

private:
  static void addMove(const Board& board, UINT32 move, std::vector<MoveScore>& result);
  static void generateKingMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v);
  static void generateQueenMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v);
  static void generateRookMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v);
  static void generateKnightMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v);
  static void generateBishopMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v);
  static void generatePawnMoves(const Board& board, const UINT32 from, std::vector<MoveScore>& v);
};

#endif // MOLECULE_MOVEGENERATOR_H
