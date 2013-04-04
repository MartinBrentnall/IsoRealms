#ifndef I_COMPONENT_SOURCES_H
#define I_COMPONENT_SOURCES_H

class HUDComponentPosition;

class IComponentSources {
  public:
  virtual std::string getSource(HUDComponentPosition*) = 0;
};

#endif
