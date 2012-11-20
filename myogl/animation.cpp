#include "animation.h"

using namespace MyOGL;

CAnimation::CAnimation() {
    CurrentFrame    = 0;
    MaxFrames       = 0;
    FrameInc        = 1;

    FrameRate       = 0.1; //seconds
    OldTime         = 0;

    Oscillate       = false;
}

void CAnimation::OnAnimate(double DeltaTime) {
    OldTime+=DeltaTime;
    if(OldTime<FrameRate){
        return;
    }
    OldTime-=FrameRate;
    CurrentFrame += FrameInc;

    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= MaxFrames) {
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= MaxFrames) {
            CurrentFrame = 0;
        }
    }
    //printf("Frame: %d\n",CurrentFrame);
}

void CAnimation::SetFrameRate(double Rate) {
    FrameRate = Rate;
}

void CAnimation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame >= MaxFrames) return;

    CurrentFrame = Frame;
}

unsigned int CAnimation::GetCurrentFrame() {
    return CurrentFrame;
}
