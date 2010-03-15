#include "TextLabelComponent.h"

TextLabelComponent::TextLabelComponent(std::string label) {
  cLabel = label;
}

void TextLabelComponent::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, 0, cLabel.c_str());
}

void TextLabelComponent::update(int milliseconds) {
}

bool TextLabelComponent::input(SDL_Event& event) {
  return false;
}

bool TextLabelComponent::contains(float, float) {
  return false;
}

float TextLabelComponent::getWidth() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  return mFont->getWidth(mFontSize, cLabel.c_str()) + 0.02f;
}

float TextLabelComponent::getHeight() {
  return 0.05f;
}

// TODO: Consider override contains() method to prevent this component from taking focus?
