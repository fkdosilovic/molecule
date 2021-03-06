#ifndef MOLECULE_ZOBRIST_CONSTANTS_H
#define MOLECULE_ZOBRIST_CONSTANTS_H

extern UINT64 PieceHashKeys[PIECE_TYPE_SIZE][BOARD_SIZE];
extern UINT64 SideToMoveKey;
extern UINT64 CastlePermissionKeys[16];
extern UINT64 EnPassantKeys[BOARD_SIZE + 1];

#endif // MOLECULE_ZOBRIST_CONSTANTS_H
