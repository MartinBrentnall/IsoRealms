#include "SpindizzyLiftProperties.h"

SpindizzyLiftProperties::SpindizzyLiftProperties() {
  reset();
}

unsigned int SpindizzyLiftProperties::getTopDelay() {
  return cTopDelay;
}

unsigned int SpindizzyLiftProperties::getBottomDelay() {
  return cBottomDelay;
}

unsigned int SpindizzyLiftProperties::getUpSpeed() {
  return cUpSpeed;
}

unsigned int SpindizzyLiftProperties::getDownSpeed() {
  return cDownSpeed;
}

void SpindizzyLiftProperties::setTopDelay(unsigned int topDelay) {
  cTopDelay = topDelay;
}

void SpindizzyLiftProperties::setBottomDelay(unsigned int bottomDelay) {
  cBottomDelay = bottomDelay;
}

void SpindizzyLiftProperties::setUpSpeed(unsigned int upSpeed) {
  cUpSpeed = upSpeed;
}

void SpindizzyLiftProperties::setDownSpeed(unsigned int downSpeed) {
  cDownSpeed = downSpeed;
}

void SpindizzyLiftProperties::setup(DOMNodeWrapper* node) {
  cTopDelay    = node->getIntegerAttribute("topDelay");
  cBottomDelay = node->getIntegerAttribute("bottomDelay");
  cUpSpeed     = node->getIntegerAttribute("upSpeed");
  cDownSpeed   = node->getIntegerAttribute("downSpeed");
}

void SpindizzyLiftProperties::reset() {
  cTopDelay    = 1000;
  cBottomDelay = 1000;
  cUpSpeed     = 1000;
  cDownSpeed   = 1000;
}


