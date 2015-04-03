#include "MusicPlayer.h"
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 bool Prepare(const char* Filename)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "PrepareMusic", "(Ljava/lang/String;)Z"))
	{
		jstring fname = t.env->NewStringUTF(Filename);
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID, fname);
	}
}

 bool PlayMusic()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "PlayMusic", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
}
bool Seekto(int millisec)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "SeekTo", "(I)Z"))
	{
		jint mil = (jint)millisec;
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID, mil);
	}
}
bool PlayDuration(int start, int end)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "PlayMusic_Duration", "(II)Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID, start, end);
	}
}
 bool Resume()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "Continue_Play", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
}
 bool Stop()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "Stop", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
}
 bool Pause()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "Pause", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
}
#endif