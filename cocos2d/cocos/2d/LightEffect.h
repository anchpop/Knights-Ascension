#pragma once


#include "2d/Effect.h"

class LightEffect : public Effect
{
public:
    static LightEffect* create();
    
    void setLightCutoffRadius(float value);
    void setLightHalfRadius(float value);
    
    void prepareForRender(cocos2d::Sprite *sprite, cocos2d::Texture2D *normalmap);
    
    cocos2d::Color3B getLightColor() const { return _lightColor; }
    cocos2d::Color3B getAmbientLightColor() const { return _ambientLightColor; }

protected:
    bool init();

    cocos2d::Vec3 _lightPos;
    cocos2d::Color3B _lightColor;
    cocos2d::Color3B _ambientLightColor;
    float _lightCutoffRadius;
    float _lightHalfRadius;
    float _brightness;
};

#pragma message ("light object is declaired")