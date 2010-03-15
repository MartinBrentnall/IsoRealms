#ifndef SDL_MIXER_SUPPORT_H
#define SDL_MIXER_SUPPORT_H

#include "../ISoundSupport.h"

#include "ISoundRegistry.h"

class SDLMixerSupport:public ISoundSupport,
                      public ISoundRegistry {
  private:
  std::vector<ISound*> cSounds;
  std::vector<ISoundSupportListener*> cListeners;  

  public:
  SDLMixerSupport();

  /*****************************\
   * Ipmlements ISoundRegistry *
  \*****************************/
  void addSound(ISound*);
  void removeSound(ISound*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  /****************************\
   * Implements ISoundSupport *
  \****************************/
  std::vector<ISound*> getSounds();
  void addSoundSupportListener(ISoundSupportListener*);
  void removeSoundSupportListener(ISoundSupportListener*);
};

#endif
