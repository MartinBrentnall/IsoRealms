#ifndef I_RESOURCE_BROWSER_H
#define I_RESOURCE_BROWSER_H

template <class T> class Icon;

template <class T> class IResourceBrowser {
  public:
  virtual void editResource(T*) = 0;
  virtual void removeResource(T*) = 0;
  virtual void setSelected(Icon<T>*) = 0;
  virtual std::string getResourceLocation(T*) = 0;
};

#endif
