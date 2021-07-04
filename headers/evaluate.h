#ifndef MOLECULE_EVALUATE_H
#define MOLECULE_EVALUATE_H

#include "constants.h"
#include "evaluation_constants.h"
#include "board.h"

class Evaluate {

private:
  static INT32 basicEvaluation(const Board& board);

public:
  static INT32 evaluatePosition(const Board& board);

};

#endif // MOLECULE_EVALUATE_H
