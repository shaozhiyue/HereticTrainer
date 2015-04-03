#ifndef MUSICPLAYER_H_INCLUDED
#define MUSICPLAYER_H_INCLUDED
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
USING_NS_CC;
bool Prepare(const char* Filename);
bool Pause();
bool PlayMusic();
bool PlayDuration(int start, int end);
bool Seekto(int millisec);
bool Resume();
bool Stop();
#endif
#endif // MUSICPLAYER_H_INCLUDED
