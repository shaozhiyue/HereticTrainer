#include "Global_timeline_varibles.h"
#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <sys/time.h>
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif
double SystemTime::getSystemTime()
{  
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	struct timeval tv;                //��ȡһ��ʱ��ṹ  
	gettimeofday(&tv, NULL);   //��ȡ��ǰʱ��  
	double t = tv.tv_sec;    
	t += ((double)tv.tv_usec)/1000000;
	cocos2d::log("Time: %f", t);
	return  t;  
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	double t = sys.wHour * 3600;
	t += sys.wMinute * 60;
	t += sys.wSecond;
	t += ((double)sys.wMilliseconds) / 1000;
	cocos2d::log("Time: %f", t);
	return t;
#endif
}  
double SystemTime::Music_start = 0;
double SystemTime::Pause_time = 0;