#include "DigitalInput.h"

DigitalInput::DigitalInput() {
  cActivatedScript = NULL;
  cInput = new bool;
  *cInput = false;
}

void DigitalInput::setup(DOMNodeWrapper* node, CommandDirectory* commandRegistry) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "OnStart") {
      cActivatedScript = commandRegistry->getScript(mNode);
    }
  }
}

void DigitalInput::configure(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Mapping") {
      std::string mType = mNode->getAttribute("type");
      std::string mValue = mNode->getAttribute("value");
      if (mType == "keyDown") {
        SDLKey mKey = getSDLKey(mValue);
        cKeys.push_back(mKey);
      } else {
        // TODO: Throw something
      }
    }
  }
}

SDLKey DigitalInput::getSDLKey(const std::string& name) {
  if (name == "1") {return SDLK_1;}
  if (name == "2") {return SDLK_2;}
  if (name == "3") {return SDLK_3;}
  if (name == "4") {return SDLK_4;}
  if (name == "5") {return SDLK_5;}
  if (name == "6") {return SDLK_6;}
  if (name == "7") {return SDLK_7;}
  if (name == "8") {return SDLK_8;}
  if (name == "9") {return SDLK_9;}
  if (name == "0") {return SDLK_0;}
  if (name == "[") {return SDLK_LEFTBRACKET;}
  if (name == "]") {return SDLK_RIGHTBRACKET;}
  if (name == "a") {return SDLK_a;}
  if (name == "b") {return SDLK_b;}
  if (name == "c") {return SDLK_c;}
  if (name == "d") {return SDLK_d;}
  if (name == "e") {return SDLK_e;}
  if (name == "f") {return SDLK_f;}
  if (name == "g") {return SDLK_g;}
  if (name == "h") {return SDLK_h;}
  if (name == "i") {return SDLK_i;}
  if (name == "j") {return SDLK_j;}
  if (name == "k") {return SDLK_k;}
  if (name == "l") {return SDLK_l;}
  if (name == "m") {return SDLK_m;}
  if (name == "n") {return SDLK_n;}
  if (name == "o") {return SDLK_o;}
  if (name == "p") {return SDLK_p;}
  if (name == "q") {return SDLK_q;}
  if (name == "r") {return SDLK_r;}
  if (name == "s") {return SDLK_s;}
  if (name == "t") {return SDLK_t;}
  if (name == "u") {return SDLK_u;}
  if (name == "v")            {return SDLK_v;}
  if (name == "w")            {return SDLK_w;}
  if (name == "x")            {return SDLK_x;}
  if (name == "y")            {return SDLK_y;}
  if (name == "z")            {return SDLK_z;}
  if (name == "F1")           {return SDLK_F1;}
  if (name == "F2")           {return SDLK_F2;}
  if (name == "F3")           {return SDLK_F3;}
  if (name == "F4")           {return SDLK_F4;}
  if (name == "F5")           {return SDLK_F5;}
  if (name == "F6")           {return SDLK_F6;}
  if (name == "F7")           {return SDLK_F7;}
  if (name == "F8")           {return SDLK_F8;}
  if (name == "F9")           {return SDLK_F9;}
  if (name == "F10")          {return SDLK_F10;}
  if (name == "F11")          {return SDLK_F11;}
  if (name == "F12")          {return SDLK_F12;}
  if (name == "F13")          {return SDLK_F13;}
  if (name == "F14")          {return SDLK_F14;}
  if (name == "F15")          {return SDLK_F15;}
  if (name == "ArrowUp")      {return SDLK_UP;}
  if (name == "ArrowLeft")    {return SDLK_LEFT;}
  if (name == "ArrowRight")   {return SDLK_RIGHT;}
  if (name == "ArrowDown")    {return SDLK_DOWN;}
  if (name == "LeftControl")  {return SDLK_LCTRL;}
  if (name == "RightControl") {return SDLK_RCTRL;}
  if (name == "Space")        {return SDLK_SPACE;}
  std::cout << "Unrecognised key name: \"" << name << "\"" << std::endl;
  exit(1);
}

bool DigitalInput::isMapped(SDLKey& key) {
  for (unsigned int i = 0; i < cKeys.size(); i++) {
    if (cKeys[i] == key) {
      return true;
    }
  }
  return false;
}

bool DigitalInput::keyDown(SDLKey& key) {
  if (isMapped(key)) {
    if (cActiveKeys.empty()) {
      *cInput = true;
      if (cActivatedScript != NULL) {
        cActivatedScript->execute();
      }
    }
    cActiveKeys.insert(key);
    return true;
  }
  return false;
}  

bool DigitalInput::keyUp(SDLKey& key) {
  if (isMapped(key)) {
    cActiveKeys.erase(key);
    if (cActiveKeys.empty()) {
      *cInput = false;
    }
    return true;
  }
  return false;
}

bool DigitalInput::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
    
    case SDL_KEYUP: {
      return keyUp(event.key.keysym.sym);
    }
  }
  return false;
}

bool* DigitalInput::getDigitalInput() {
  std::cout << "Using input (" << cInput << ")" << std::endl;
  return cInput;
}
