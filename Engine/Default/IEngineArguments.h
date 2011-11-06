#ifndef I_ENGINE_ARGUMENTS_H
#define I_ENGINE_ARGUMENTS_H

class IEngineArguments {
  public:
  virtual void set(const std::string&) = 0;
  virtual std::string get() = 0;
};

#endif
