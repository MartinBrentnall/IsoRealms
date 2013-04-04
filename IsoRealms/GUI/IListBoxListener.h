#ifndef I_LIST_BOX_LISTENER_H
#define I_LIST_BOX_LISTENER_H

#include <string>

class ListBox;

class IListBoxListener {
  public:
  virtual void assertSelection(ListBox*, const std::string&) = 0;
};

#endif
