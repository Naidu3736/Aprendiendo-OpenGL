#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#pragma once

#include "./sprite_sheet.hpp"
#include "../../glm/glm.hpp"
#include <vector>
#include <functional>

namespace engine {
    namespace graphics {
        class Animation {
            SpriteSheet& sprite;
            std::vector<glm::uvec2> frames;
            float frameDuration;
            float currentTime;
            GLuint currentFrame;
            bool looping;
            bool playing;

            std::function<void()> onComplete;
            std::function<void()> onFrameChange;

            Animation (SpriteSheet& sheet, float duration, bool loop = true);

            void addFrame(GLuint x, GLuint y);
            void addFrameRange(GLuint startX, GLuint startY, GLuint endX, GLuint endY);
            void addAllFrame();

            void update(float deltaTime);
            void play();
            void pause();
            void stop();
            void reset();

            glm::vec4 getCurrentUV() const;
            GLuint getCurrentFrameIndex() const { return currentFrame; };
            GLuint getTotalFrames() const { return (GLuint)frames.size(); };

            void setFrameDuration(float durarion);
            void setLooping(bool loop);
            void setOnComplete(std::function<void()> callback) { onComplete = callback; }
            void setOnFrameChange(std::function<void()> callback) { onFrameChange = callback; }

            bool isPlaying() const { return playing; }
            bool isFinished() const { return (!looping && currentFrame >= frames.size() - 1); };
        };
    }
}

#endif // ANIMATION_HPP