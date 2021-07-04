#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include "constants.h"
#include "board.h"
#include "makemove.h"
#include <cstring>

class TT {

public:
    TT () {
      tableSize = 1 << 21;
      table = new TTNode[tableSize];

      for (UINT32 i = 0; i < tableSize; ++i) {
        std::memset(&table[i], 0x0, sizeof(TTNode));
      }
    }

    ~TT() {
      delete [] table;
    }

    void add(Board& board, UINT32 move, INT32 score, TTFlags flag, UINT32 depth) {
      TTNode* node = &table[board.getPositionKey() % tableSize];

      // if score > MATE -> score += pos.getply
      // else if score < -MATE -> score -= pos.getply

        node->positionKey = board.getPositionKey();
        node->move = move;
        node->score = score;
        node->flag = flag;
        node->depth = depth;
    }

    INT32 get(Board& board, UINT32* move, INT32 alpha, INT32 beta, UINT32 depth) const {
        const TTNode* node = &table[board.getPositionKey() % tableSize];

        if (node->positionKey == board.getPositionKey()) {
            *move = node->move;
            if (node->depth >= depth) {
                if (node->flag == HFEXACT) return node->score;
                if (node->flag == HFALPHA && node->score <= alpha) return alpha;
                if (node->flag == HFBETA && node->score >= beta) return beta;
            }
        }

        return -INFINITE - 5;
    }

    UINT32 probe(const Board& board) const {
        const TTNode* node = &table[board.getPositionKey() % tableSize];

        if (node->positionKey == board.getPositionKey()) {
            return node->move;
        }

        return 0;
    }

    void clear() {
    }

    UINT32 getPvLine(Board& board, const UINT32 depth);

private:
    TTNode* table{nullptr};
    UINT32 tableSize{0};
};

#endif // TRANSPOSITION_TABLE_H