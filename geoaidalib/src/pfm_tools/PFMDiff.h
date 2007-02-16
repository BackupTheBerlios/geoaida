#ifndef PFMDiff_h
#define PFMDiff_h

#include "PFMArray.h"

class PFMDiff : public PFMArray
{
 public:
  enum {
    X_DIR,
    Y_DIR
  };
  PFMDiff(PFMArray &pic, int direction);
};

#endif
