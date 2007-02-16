#ifndef PFMGradAngle_h
#define PFMGradAngle_h

#include "PFMArray.h"

class PFMGradAngle : public PFMArray
{
 public:
  enum {
    X_DIR,
    Y_DIR
  };
  PFMGradAngle(PFMArray &pic);
};

#endif
