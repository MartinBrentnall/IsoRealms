/*
 * Copyright 2009 Martin Brentnall
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
 * GNU General Public Linpicense for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef COMPONENT_COLOUR_SELECTOR_H
#define COMPONENT_COLOUR_SELECTOR_H

#include <map>
#include <vector>
#include <xercesc/framework/psvi/XSConstants.hpp>

#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>

#include "../../Configuration.h"
#include "../../GUI/Dialog.h"
#include "../../IComponentContainer.h"
#include "../../ScreenConfiguration.h"

class ComponentColourSelector:public RectangularComponent {
  private:
  static float MIN_CHANNEL_VALUE;
  static float MAX_CHANNEL_VALUE;
    
  class ChannelRenderer:public ISliderRenderer {
    private:
    ComponentColourSelector* cParent;
    float* cRedStart;
    float* cRedEnd;
    float* cGreenStart;
    float* cGreenEnd;
    float* cBlueStart;
    float* cBlueEnd;
    
    public:
    ChannelRenderer(ComponentColourSelector*, float*, float*, float*, float*, float*, float*);
    void render(SliderComponent*);
  };
    
  class HueRenderer:public ISliderRenderer {
    private:
    ComponentColourSelector* cParent;
    
    public:
    HueRenderer(ComponentColourSelector*);      
    void render(SliderComponent*);
  };
  
  class SaturationRenderer:public ISliderRenderer {
    private:
    ComponentColourSelector* cParent;

    public:
    SaturationRenderer(ComponentColourSelector*);
    void render(SliderComponent*);
  };
  
  class LightnessRenderer:public ISliderRenderer {
    private:
    ComponentColourSelector* cParent;
    
    public:
    LightnessRenderer(ComponentColourSelector*);
    void render(SliderComponent*);
  };
  
  class AlphaRenderer:public ISliderRenderer {
    private:
    ComponentColourSelector* cParent;
    
    public:
    AlphaRenderer(ComponentColourSelector*);      
    void render(SliderComponent*);
  };
  
  class PreviewRenderer:public IPanelRenderer {
    private:
    ComponentColourSelector* cParent;
    
    public:
    PreviewRenderer(ComponentColourSelector*);
    void render(PanelComponent*);
  };
  
  class SliderListener:public IValueListener<float> {
    private:
    ComponentColourSelector* cParent;
    float* cChannel;
      
    public:
    SliderListener(ComponentColourSelector*, float*);
    
    /************************************\
     * Implements IValueListener<float> *
    \************************************/
    void valueChanged(IValueComponent<float>*, float);
  };
  
  class HueListener:public IValueListener<float> {
    private:
    ComponentColourSelector* cParent;
    float* cChannel;
      
    public:
    HueListener(ComponentColourSelector*, float*);
    
    /************************************\
     * Implements IValueListener<float> *
    \************************************/
    void valueChanged(IValueComponent<float>*, float);
  };
  
  class StringListener:public IValueListener<std::string> {
    private:
    ComponentColourSelector* cParent;
    float* cChannel;
    
    public:
    StringListener(ComponentColourSelector*, float*);
    
    /*****************************************\
     * Implements IValueListener<std::string *
    \*****************************************/
    void valueChanged(IValueComponent<std::string>*, std::string);
  };
    
  class StringListenerHSL:public IValueListener<std::string> {
    private:
    ComponentColourSelector* cParent;
    float* cChannel;
    
    public:
    StringListenerHSL(ComponentColourSelector*, float*);
    
    /******************************************\
     * Implements IValueListener<std::string> *
    \******************************************/
    void valueChanged(IValueComponent<std::string>*, std::string);
  };
    
  float cRed;
  float cGreen;
  float cBlue;
  float cHue;
  float cSaturation;
  float cLightness;
  float cAlpha;
  Colour* cColour;
  bool cUpdating;
  IResourceAccessor* cResources;
  std::vector<IResourceSelectionListener<IColour>*> cListeners;
    
  void setupComponent();
  void updateColour();
  void updateHSLSliders();
  void updateRGBSliders();
  float getMiddle();
  float getLightness();
  float getDarkness();
  void renderBackground(IRectangularComponent*);
  void calculateHue(float&, float&, float&);
    
  public:
  ComponentColourSelector(Colour*, IResourceAccessor*);
  
  void setColour(Colour*);
  
  void addColourSelectionListener(IResourceSelectionListener<IColour>* listener);
  void removeColourSelectionListener(IResourceSelectionListener<IColour>* listener);
  
  void refresh();
};

#endif
