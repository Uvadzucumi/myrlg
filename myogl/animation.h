#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <SDL/SDL.h>

namespace MyOGL{

    class CAnimation {
        private:
            int    CurrentFrame;
            int     FrameInc;

            double    FrameRate; //in seconds
            double    OldTime;

        public:
            int    FramesCount;
            bool    Oscillate;

            CAnimation();
            bool OnAnimate(double DeltaTime);

            void SetFrameRate(double Rate);
            void SetCurrentFrame(int Frame);
            int GetCurrentFrame();
    };

}
#endif // ANIMATION_H_INCLUDED
