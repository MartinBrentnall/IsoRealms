#ifndef I_CONFIRMATION_LISTENER_H
#define I_CONFIRMATION_LISTENER_H

#include <IsoRealms/GUI/IRectangularComponent.h>

class IConfirmationListener {
  public:
  virtual void dialogConfirmed(IRectangularComponent*) = 0;
  virtual void dialogCancelled(IRectangularComponent*) = 0;
};

#endif
