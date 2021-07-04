#ifndef MOLECULE_BOARD_H
#define MOLECULE_BOARD_H

#include "../dependency/magic-bits/slider_attacks.h"
#include "atomicdatatypes.h"
#include "constants.h"
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>

class Board {

private:
  SliderAttacks sliderAttacks;
  UINT32 board[BOARD_SIZE];  // for each square contains a piece type on it

  /* used to speed up removal from piece list */
  UINT32 squareToPieceListIndex[BOARD_SIZE];

  UINT64 bitboards[PIECE_TYPE_SIZE];

  UINT64 sideToMove{WHITE}; 
  UINT64 enPassantSquare{BOARD_SIZE};
  UINT64 castlePermission{0ULL};
  UINT64 numberOfHalfMoves{0ULL};
  UINT64 numberOfHalfMovesSinceLastPawnCaptureAdvance{0ULL};
  UINT64 positionKey{0ULL};

  void printBitboard(const UINT64* bitboard, const UINT32 bitboardType) const;
  std::vector<Move> history;

  UINT32 searchHistory[PIECE_TYPE_SIZE][BOARD_SIZE];
  UINT32 searchKillers[2][MAX_DEPTH];
  UINT32 pvArray[MAX_DEPTH];

public:
  UINT64 pieceList[PIECE_TYPE_SIZE][20]; // pieceList[piece][0] -> size of current piece list

  /* Constructors and destructor */
  Board(const std::string& fen);
  ~Board();

  void addToPieceList(const UINT32 piece, const UINT32 square) {
    pieceList[piece][pieceList[piece][0]] = square;
    squareToPieceListIndex[square] = pieceList[piece][0];
    pieceList[piece][0]++;
  }

  void removeFromPieceList(const UINT32 piece, const UINT32 square) {
    UINT32 index = squareToPieceListIndex[square];
    squareToPieceListIndex[square] = 0;
    std::swap(pieceList[piece][index], pieceList[piece][pieceList[piece][0] - 1]);
    squareToPieceListIndex[pieceList[piece][index]] = index;
    pieceList[piece][0]--;
  }

  void setUpBoard(const std::string& fen);

  void addToKillers(const UINT32 move) {
    searchKillers[1][numberOfHalfMoves] = searchKillers[0][numberOfHalfMoves];
    searchKillers[0][numberOfHalfMoves] = move;
  }

  UINT32 getKillers(UINT32 index) const {
    return searchKillers[index][numberOfHalfMoves];
  }

  /* Getters */
  void print() const;

  UINT64 getPositionKey() const {
    return positionKey;
  }

  UINT64 getSideToMove() const {
    return sideToMove;
  }

  UINT64 getCastlingPermission() const {
    return castlePermission;
  }

  UINT64 getEnPassantSquare() const {
    return enPassantSquare;
  }

  UINT64 getNumberOfHalfMovesSinceLastPawnCaptureAdvance() const {
    return numberOfHalfMovesSinceLastPawnCaptureAdvance;
  }

  UINT64 getBitboard(const UINT32 bitboardType) const {
    return bitboards[bitboardType];
  }

  UINT64 getMovingSideBitboard() const {
    return bitboards[sideToMove];
  }

  UINT32 getPieceFromSquare(const UINT32 square) const {
    return board[square];
  }

  UINT64 getNumberOfHalfMoves() const {
    return numberOfHalfMoves;
  }

  bool hasCastlingRights(const UINT32 perm) const {
    return (castlePermission & perm);
  }

  bool isRepetition() const {
    if (!history.size()) {
      return false;
    }

    for (UINT32 i = history.size() - numberOfHalfMovesSinceLastPawnCaptureAdvance; i < history.size() - 1; ++i) {
      if (history[i].positionKey == positionKey) {
        return true;
      }
    }
    return false;
  }

  void printRegularBitboard(const int bitboardType) const;
  UINT64 getAttacksTo(UINT64 bitboard, const UINT32 square) const;

  void getMoveFromHistory(Move& move) {
    if (!history.size()) {
      return;
    }

    move = history[history.size() - 1];
    history.pop_back();
  }

  /* Setters */
  void setBitboards(const UINT64 pieceType, UINT64 value) {
    bitboards[pieceType] |= PowersOfTwo[value];
  }

  void setSideToMove(const UINT64 side) {
    sideToMove = side;
  }

  void clearCastlingPermission(const UINT32 perm) {
    castlePermission &= perm;
  }

  void addCastlingPermission(const UINT32 perm) {
    castlePermission |= perm;
  }

  void setCastlingPermission(const UINT32 perm) {
    castlePermission = perm;
  }

  void setEnPassantSquare(const UINT64 square) {
    enPassantSquare = square;
  }

  void setNumberOfHalfMoves(const UINT64 numberOfMoves) {
    numberOfHalfMoves = numberOfMoves;
  }

  void setNumberOfHalfMovesSinceLastPawnCaptureAdvance(const UINT64 value) {
    numberOfHalfMovesSinceLastPawnCaptureAdvance = value;
  }

  void setPieceOnSquare(const UINT32 piece, const UINT32 square) {
    board[square] = piece;
  }

  void updateBitboard(const UINT32 boardType, const UINT32 square) {
    bitboards[boardType] ^= PowersOfTwo[square];
    bitboards[PieceTypeToColour[boardType]] ^= PowersOfTwo[square];
    bitboards[BOTH] ^= PowersOfTwo[square];
  }

  void increaseFiftyMoveCounter() {
    ++numberOfHalfMovesSinceLastPawnCaptureAdvance;
  }

  void resetFiftyMoveCounter() {
    numberOfHalfMovesSinceLastPawnCaptureAdvance = 0;
  }

  void increasePly() {
    ++numberOfHalfMoves;
  }

  void switchSides() {
    sideToMove = (sideToMove == WHITE ? BLACK : WHITE);
  }

  void addMoveToHistoryArray(const UINT32 move) {
    history.push_back(Move{move, castlePermission, enPassantSquare, numberOfHalfMovesSinceLastPawnCaptureAdvance, positionKey});
  }

  void decreasePly() {
    --numberOfHalfMoves;
  }

  void hashPiece(const UINT32 pieceType, const UINT32 square) {
    positionKey ^= PieceHashKeys[pieceType][square];
  }

  void hashEnPassantSquare() {
    positionKey ^= EnPassantKeys[enPassantSquare];
  }

  void hashCastlePermission() {
    positionKey ^= CastlePermissionKeys[castlePermission];
  }

  void hashSide() {
    positionKey ^= SideToMoveKey;
  }

  void clearSearchHistory() {
    memset(searchHistory, 0x0, sizeof(searchHistory));
  }

  void clearSearchKillers() {
    memset(searchKillers, 0x0, sizeof(searchKillers));
  }

  UINT32 getMovingSideKing() const {
    return (sideToMove - 1);
  }

  void addToPVArray(const UINT32 index, const UINT32 move) {
    pvArray[index] = move;
  }


  UINT32 getFromPVArray(UINT32 index = 0) const {
    return pvArray[index];
  }

  UINT32 getHistory(const UINT32 piece, const UINT32 square) const {
    return searchHistory[piece][square];
  }

  void addToHistory(const UINT32 piece, const UINT32 square, const UINT32 increase) {
    searchHistory[piece][square] += increase;
  }

};

#endif // MOLECULE_BOARD_H
