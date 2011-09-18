#include "ImageTextureSet.h"

ImageTextureSet::ImageTextureSet() {
  // TODO: Should only init once!
  int mFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  int mInit = IMG_Init(mFlags);
  if ((mInit & mFlags) != mFlags) {
    std::cout << "Failed to initialise SDL_Image" << std::endl;
	// TODO: Throw here
  }
}

ImageTextureProxy* ImageTextureSet::getTextureProxy(const std::string& name) {
  ImageTextureProxy* mTexture = cTextures[name];
  if (mTexture == NULL) {
    mTexture = new ImageTextureProxy();
    cTextures[name] = mTexture;
  }
  return mTexture;
}

ISpindizzyTexture* ImageTextureSet::getTexture(const std::string& name) {
  return getTextureProxy(name);
}

void ImageTextureSet::loadTexture(const std::string& filename, const std::string& id) {
  std::string mResource = System::getResource("Images/" + filename);
  SDL_Surface* mSurface = IMG_Load(mResource.c_str());
  GLuint mTextureID;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if (mSurface->format->Amask) {
    glTexImage2D(GL_TEXTURE_2D, 0, 4, mSurface->w, mSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mSurface->pixels);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, 3, mSurface->w, mSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, mSurface->pixels);
  }
  SDL_FreeSurface(mSurface);
  ImageTextureProxy* mTextureProxy = getTextureProxy(id);
  mTextureProxy->setTexture(new ImageTexture(mTextureID));
}

void ImageTextureSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mFilename = mNode->getAttribute("file");
      std::string mID = mNode->getAttribute("id");
      loadTexture(mFilename, mID);
    } else {
      // TODO: Throw here
    }
  }
}

ImageTextureSet::~ImageTextureSet() {
  for (std::map<std::string, ImageTextureProxy*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  // TODO: Should only be called when *all* plugins/instances have finished using SDL_Image
  IMG_Quit();
}

extern "C" IPlugin* create() {
  return new ImageTextureSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}