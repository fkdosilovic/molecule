#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

typedef uint64_t U64;

static const int kSquares = 64;

static const char kRookMagics[] = "magics/rook_magics.magic";
static const char kRookMasks[] = "magics/rook_masks.magic";
static const char kRookShifts[] = "magics/rook_shifts.magic";
static const char kRookOffsets[] = "magics/rook_offsets.magic";
static const char kRookAttackTable[] = "magics/rook_attack_table.magic";

static const char kBishopMagics[] = "magics/bishop_magics.magic";
static const char kBishopMasks[] = "magics/bishop_masks.magic";
static const char kBishopShifts[] = "magics/bishop_shifts.magic";
static const char kBishopOffsets[] = "magics/bishop_offsets.magic";
static const char kBishopAttackTable[] = "magics/bishop_attack_table.magic";

#endif
