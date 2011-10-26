#ifndef IMAGE_TEXTURE_SET_H
#define IMAGE_TEXTURE_SET_H

#include <GL/gl.h>
#include <map>
#include <SDL_image.h>
#include <string>

#include "../ISpindizzyTextureSet.h"

#include "ImageTexture.h"
#include "ImageTextureProxy.h"

class ImageTextureSet:public ISpindizzyTextureSet {
  private:
  std::map<std::string, ImageTextureProxy*> cTextures;
  
  void loadTexture(const std::string&, const std::string&);
  ImageTextureProxy* getTextureProxy(const std::string&);
  
  public:
  ImageTextureSet();
  
  /**************************\
   * Implements ITextureSet *
  \**************************/
  ISpindizzyTexture* getTexture(const std::string&);

  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);

  ~ImageTextureSet();
};

#endif

