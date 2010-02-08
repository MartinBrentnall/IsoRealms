#include "TextLabelComponent.h"

IFont* TextLabelComponent::cFont = NULL;

void TextLabelComponent::setFont(IFont* font) {
  cFont = font;
}

TextLabelComponent::TextLabelComponent(std::string label) {
  cLabel = label;
}

void TextLabelComponent::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
/*  float mRight = getRight();
  float mTop = getTop();*/
  glColor3f(1.0f, 1.0f, 1.0f);
  cFont->print(mLeft + 0.01f, mBottom + 0.01f, 0.02f, 0, cLabel.c_str());
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
  return cFont->getWidth(0.02f, cLabel.c_str()) + 0.02f;
}

float TextLabelComponent::getHeight() {
  return 0.05f;
}

// TODO: Consider override contains() method to prevent this component from taking focus?
