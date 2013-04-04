#ifndef I_LOCK_H
#define I_LOCK_H

class ILock {
  public:
  virtual bool isLocked() = 0;
};

#endif
