#include "engine/graphics/animation.hpp"
#include <iostream>

using namespace engine::graphics;

Animation::Animation(SpriteSheet& sheet, float duration, bool loop)
                    : sprite(sheet), frameDuration(duration), looping(loop)
{

}

Animation::