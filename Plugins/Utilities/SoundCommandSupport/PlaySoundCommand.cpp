#include "PlaySoundCommand.h"

PlaySoundCommand::PlaySoundCommand(ISound* sound) {
  cSound = sound;
}

std::string PlaySoundCommand::getCommandName() {
  return "Play " + cSound->getName();
}

void PlaySoundCommand::execute() {
  cSound->play();
}

