/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SpindizzyZoneThemeTexture.h" 

SpindizzyZoneThemeTexture::SpindizzyZoneThemeTexture(IIconAnimator* animator) {
  cAnimator = animator;
}

void SpindizzyZoneThemeTexture::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cTexture = nullptr;
}

Icon<ITexture>* SpindizzyZoneThemeTexture::getResourceIcon(IResourceBrowser<ITexture>* browser) {
  return new IconThemeTexture(cAnimator, browser, this);
}

void SpindizzyZoneThemeTexture::set(ITexture* texture) {
  cTexture = texture;
}

ITexture* SpindizzyZoneThemeTexture::getTexture() {
  return cTexture->getTexture();
}

void SpindizzyZoneThemeTexture::set() {
  if (cTexture != nullptr) {
    cTexture->set();
  }
}

void SpindizzyZoneThemeTexture::hintInUse(bool inUse) {
  if (cTexture != nullptr) {
    cTexture->hintInUse(inUse);
  }
}

