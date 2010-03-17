#ifndef I_TEXT_COMPONENT_H
#define I_TEXT_COMPONENT_H

#include <string>

class ITextComponent {
  public:
  virtual void setText(const std::string&) = 0;
};

#endif
