#include "animation.h"

using namespace MyOGL;

CAnimation::CAnimation() {
    CurrentFrame    = 0;
    FramesCount     = 0;
    FrameInc        = 1;

    FrameRate       = 0.1; //seconds
    OldTime         = 0;

    Oscillate       = false;
}

// return true if frame changed
bool CAnimation::OnAnimate(double DeltaTime){
    OldTime+=DeltaTime;
    if(OldTime<FrameRate){
        return false;
    }
    OldTime-=FrameRate;
    CurrentFrame += FrameInc;
    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= FramesCount-1) {
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= FramesCount) {
            CurrentFrame = 0;
        }
    }
    return true;
}

void CAnimation::SetFrameRate(double Rate) {
    FrameRate = Rate;
}

void CAnimation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame > FramesCount) return;
    CurrentFrame = Frame;
}

int CAnimation::GetCurrentFrame() {
    return CurrentFrame;
}
