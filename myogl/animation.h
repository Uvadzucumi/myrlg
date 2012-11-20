#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <SDL/SDL.h>

namespace MyOGL{

    class CAnimation {
        private:
            int    CurrentFrame;
            unsigned int     FrameInc;

        private:
            double    FrameRate; //in seconds
            double    OldTime;

        public:
            int    MaxFrames;
            bool    Oscillate;

        public:
            CAnimation();
            void OnAnimate(double DeltaTime);

        public:
            void SetFrameRate(double Rate);
            void SetCurrentFrame(int Frame);
            unsigned int GetCurrentFrame();
    };

}
#endif // ANIMATION_H_INCLUDED
