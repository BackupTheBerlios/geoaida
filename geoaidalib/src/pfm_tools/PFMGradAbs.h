#ifndef PFMGradAbs_h
#define PFMGradAbs_h

#include "PFMArray.h"

class PFMGradAbs : public PFMArray
{
 public:
  enum {
    X_DIR,
    Y_DIR
  };
  PFMGradAbs(PFMArray &pic);
};

#endif
