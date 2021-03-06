#ifndef MOLECULE_CONSTANTS_H
#define MOLECULE_CONSTANTS_H

#include "atomicdatatypes.h"
#include "../dependency/magic-bits/slider_attacks.h"
#include <mutex>

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define NAME "Molecule v1.0"
#define AUTHOR "Filip Karlo Dosilovic"

constexpr UINT32 BOARD_SIZE             = 64;
constexpr UINT32 ASCII_SIZE             = 256;
constexpr UINT32 PIECE_TYPE_SIZE        = 16;
constexpr UINT32 MAX_DEPTH              = 16;
constexpr  INT32 INFINITE               = 0x3F3F3F3F;
constexpr  INT32 MATE                   = 5000000;
constexpr UINT32 NODE_CHECK             = 4095;

extern std::mutex searchInfoMutex;
extern SliderAttacks sliderAttacks;
extern INT32 MvvLvaTable[PIECE_TYPE_SIZE][PIECE_TYPE_SIZE];

constexpr CHAR PieceTypeToChar[PIECE_TYPE_SIZE] {
   0,
  'P', 'N', 'B', 'R', 'Q', 'K', 0,
  'p', 'n', 'b', 'r', 'q', 'k', 0,
};

constexpr UINT64 MAX_UINT64{18446744073709551615ULL}; // can be removed and use climits !!!

constexpr UINT64 PowersOfTwo[BOARD_SIZE + 1] {
  1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL, 128ULL,
  256ULL, 512ULL, 1024ULL, 2048ULL, 4096ULL, 8192ULL, 16384ULL, 32768ULL,
  65536ULL, 131072ULL, 262144ULL, 524288ULL, 1048576ULL, 2097152ULL, 4194304ULL, 8388608ULL,
  16777216ULL, 33554432ULL, 67108864ULL, 134217728ULL, 268435456ULL, 536870912ULL, 1073741824ULL, 2147483648ULL,
  4294967296ULL, 8589934592ULL, 17179869184ULL, 34359738368ULL, 68719476736ULL, 137438953472ULL, 274877906944ULL, 549755813888ULL,
  1099511627776ULL, 2199023255552ULL, 4398046511104ULL, 8796093022208ULL, 17592186044416ULL, 35184372088832ULL, 70368744177664ULL, 140737488355328ULL,
  281474976710656ULL, 562949953421312ULL, 1125899906842624ULL, 2251799813685248ULL, 4503599627370496ULL, 9007199254740992ULL, 18014398509481984ULL, 36028797018963968ULL,
  72057594037927936ULL, 144115188075855872ULL, 288230376151711744ULL, 576460752303423488ULL, 1152921504606846976ULL, 2305843009213693952ULL, 4611686018427387904ULL, 9223372036854775808ULL, 0ULL
};

constexpr UINT32 CastlePermission[BOARD_SIZE] = {
    14, 15, 15, 15, 12, 15, 15, 13,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    11, 15, 15, 15,  3, 15, 15, 7,
};

constexpr UINT8 CharToPieceType[BOARD_SIZE * 4] {       // NOT changed YET
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0  - 15
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16 - 31
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 32 - 47
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 48 - 63
  0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 2, 0, // 64 - 79
  1, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80 - 95
  0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 9, 0, // 96 - 111
  8, 12, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 112 - 127
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 128 - 143
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 144 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160 - 175
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176 - 191
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 192 - 207
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 208 - 223
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 224 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 240 - 255
};

constexpr UINT64 KnigthAttacks[BOARD_SIZE] {
  0x20400ULL, 0x50800ULL, 0xa1100ULL, 0x142200ULL, 0x284400ULL, 0x508800ULL, 0xa01000ULL, 0x402000ULL,
  0x2040004ULL, 0x5080008ULL, 0xa110011ULL, 0x14220022ULL, 0x28440044ULL, 0x50880088ULL, 0xa0100010ULL, 0x40200020ULL,
  0x204000402ULL, 0x508000805ULL, 0xa1100110aULL, 0x1422002214ULL, 0x2844004428ULL, 0x5088008850ULL, 0xa0100010a0ULL, 0x4020002040ULL,
  0x20400040200ULL, 0x50800080500ULL, 0xa1100110a00ULL, 0x142200221400ULL, 0x284400442800ULL, 0x508800885000ULL, 0xa0100010a000ULL, 0x402000204000ULL,
  0x2040004020000ULL, 0x5080008050000ULL, 0xa1100110a0000ULL, 0x14220022140000ULL, 0x28440044280000ULL, 0x50880088500000ULL, 0xa0100010a00000ULL, 0x40200020400000ULL,
  0x204000402000000ULL, 0x508000805000000ULL, 0xa1100110a000000ULL, 0x1422002214000000ULL, 0x2844004428000000ULL, 0x5088008850000000ULL, 0xa0100010a0000000ULL, 0x4020002040000000ULL,
  0x400040200000000ULL, 0x800080500000000ULL, 0x1100110a00000000ULL, 0x2200221400000000ULL, 0x4400442800000000ULL, 0x8800885000000000ULL, 0x100010a000000000ULL, 0x2000204000000000ULL,
  0x4020000000000ULL, 0x8050000000000ULL, 0x110a0000000000ULL, 0x22140000000000ULL, 0x44280000000000ULL, 0x88500000000000ULL, 0x10a00000000000ULL, 0x20400000000000ULL
};

constexpr UINT64 KingAttacks[BOARD_SIZE] {
  0x302ULL, 0x705ULL, 0xe0aULL, 0x1c14ULL, 0x3828ULL, 0x7050ULL, 0xe0a0ULL, 0xc040ULL,
  0x30203ULL, 0x70507ULL, 0xe0a0eULL, 0x1c141cULL, 0x382838ULL, 0x705070ULL, 0xe0a0e0ULL, 0xc040c0ULL,
  0x3020300ULL, 0x7050700ULL, 0xe0a0e00ULL, 0x1c141c00ULL, 0x38283800ULL, 0x70507000ULL, 0xe0a0e000ULL, 0xc040c000ULL,
  0x302030000ULL, 0x705070000ULL, 0xe0a0e0000ULL, 0x1c141c0000ULL, 0x3828380000ULL, 0x7050700000ULL, 0xe0a0e00000ULL, 0xc040c00000ULL,
  0x30203000000ULL, 0x70507000000ULL, 0xe0a0e000000ULL, 0x1c141c000000ULL, 0x382838000000ULL, 0x705070000000ULL, 0xe0a0e0000000ULL, 0xc040c0000000ULL,
  0x3020300000000ULL, 0x7050700000000ULL, 0xe0a0e00000000ULL, 0x1c141c00000000ULL, 0x38283800000000ULL, 0x70507000000000ULL, 0xe0a0e000000000ULL, 0xc040c000000000ULL,
  0x302030000000000ULL, 0x705070000000000ULL, 0xe0a0e0000000000ULL, 0x1c141c0000000000ULL, 0x3828380000000000ULL, 0x7050700000000000ULL, 0xe0a0e00000000000ULL, 0xc040c00000000000ULL,
  0x203000000000000ULL, 0x507000000000000ULL, 0xa0e000000000000ULL, 0x141c000000000000ULL, 0x2838000000000000ULL, 0x5070000000000000ULL, 0xa0e0000000000000ULL, 0x40c0000000000000ULL
};

constexpr UINT64 WhitePawnAttacks[BOARD_SIZE] {
  512ULL, 1280ULL, 2560ULL, 5120ULL, 10240ULL, 20480ULL, 40960ULL, 16384ULL,
  131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL,
  33554432ULL, 83886080ULL, 167772160ULL, 335544320ULL, 671088640ULL, 1342177280ULL, 2684354560ULL, 1073741824ULL,
  8589934592ULL, 21474836480ULL, 42949672960ULL, 85899345920ULL, 171798691840ULL, 343597383680ULL, 687194767360ULL, 274877906944ULL,
  2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL,
  562949953421312ULL, 1407374883553280ULL, 2814749767106560ULL, 5629499534213120ULL, 11258999068426240ULL, 22517998136852480ULL, 45035996273704960ULL, 18014398509481984ULL,
  144115188075855872ULL, 360287970189639680ULL, 720575940379279360ULL, 1441151880758558720ULL, 2882303761517117440ULL, 5764607523034234880ULL, -6917529027641081856ULL, 4611686018427387904ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL
};

constexpr UINT64 BlackPawnAttacks[BOARD_SIZE] {
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  2ULL, 5ULL, 10ULL, 20ULL, 40ULL, 80ULL, 160ULL, 64ULL,
  512ULL, 1280ULL, 2560ULL, 5120ULL, 10240ULL, 20480ULL, 40960ULL, 16384ULL,
  131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL,
  33554432ULL, 83886080ULL, 167772160ULL, 335544320ULL, 671088640ULL, 1342177280ULL, 2684354560ULL, 1073741824ULL,
  8589934592ULL, 21474836480ULL, 42949672960ULL, 85899345920ULL, 171798691840ULL, 343597383680ULL, 687194767360ULL, 274877906944ULL,
  2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL,
  562949953421312ULL, 1407374883553280ULL, 2814749767106560ULL, 5629499534213120ULL, 11258999068426240ULL, 22517998136852480ULL, 45035996273704960ULL, 18014398509481984ULL
};

constexpr UINT64 WhitePawnEnPassant[BOARD_SIZE] {
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL
};

constexpr UINT64 BlackPawnEnPassant[BOARD_SIZE] {
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
  0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL
};

struct PVNode {
  UINT64 positionKey;
  UINT32 move;
};

struct SearchInfo {
  UINT64 startTime;
  UINT64 endTime;
  UINT32 depthLimit;
  UINT32 timeLimit;
  UINT32 numberOfNodes;
  UINT32 movesToGo;

  bool quit;
  bool stopped;
  bool infinite;
  bool timeSet;

  UINT32 failHigh;
  UINT32 failHighFirst;
};

// Pawn, Knigth, Bishop, Rook, Queen, King
enum PieceType {
  NONE,
  WHITE_PAWN,     WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, // WHITE = 7
  BLACK_PAWN = 8, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, // BLACK = 14
};

enum Colour {
  WHITE = 7, BLACK = 14, BOTH = 15
};

constexpr UINT8 PieceTypeToColour[PIECE_TYPE_SIZE] {
  0,
  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
  BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK
};

enum Squares {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8
};

// castling constants
constexpr UINT32 BLACK_KING_SIDE_CASTLE  = 8;
constexpr UINT32 BLACK_QUEEN_SIDE_CASTLE = 4;
constexpr UINT32 WHITE_KING_SIDE_CASTLE  = 2;
constexpr UINT32 WHITE_QUEEN_SIDE_CASTLE = 1;

struct Move {
  UINT32 move;
  UINT64 castlePermission;
  UINT64 enPassantSquare;
  UINT64 fiftyMoveCounter;
  UINT64 positionKey;

  Move(UINT32 m = 0, UINT64 cp = 0, UINT64 eps = 0, UINT64 fmc = 0, UINT64 pk = 0) {
    move = m;
    castlePermission = cp;
    enPassantSquare = eps;
    fiftyMoveCounter = fmc;
    positionKey = pk;
  }

  Move(const Move& moveRef) {
    move = moveRef.move;
    castlePermission = moveRef.castlePermission;
    enPassantSquare = moveRef.enPassantSquare;
    fiftyMoveCounter = moveRef.fiftyMoveCounter;
    positionKey = moveRef.positionKey;
  }
};

struct MoveScore {
  UINT32 move;
  INT32 score;

  MoveScore(UINT32 m = 0, INT32 s = 0) {
    move = m;
    score = s;
  }

  MoveScore(const MoveScore& ms) {
    move = ms.move;
    score = ms.score;
  }
};

enum TTFlags {
  HFEXACT, HFALPHA, HFBETA
};

struct TTNode {
  UINT64 positionKey;
  UINT32 depth;
  TTFlags flag;
  INT32 score;
  UINT32 move;

  TTNode() {
    positionKey = 0ULL;
    depth = score = move = 0;
  }
};

#include "zobrist_constants.h"

#endif // MOLECULE_CONSTANTS_H
