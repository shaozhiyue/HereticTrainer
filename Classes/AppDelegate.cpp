#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include"MainGame.h"
#include "SelectSong.h"
#include"audio\include\AudioEngine.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni.h"
#include "MusicPlayer.h"
#include"AndroidAudio.h"
#endif
#include <stdio.h>
//#include "build\platforms\android-8\arch-arm\usr\include\android\log.h"
//#define TAG "HereticTrainer_init"
//#define LOGD(��) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
USING_NS_CC;
MainGame* thisgame;
AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() 
{
}
//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	thisgame = nullptr;
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("HereticTrainer");
        director->setOpenGLView(glview);
	}
	std::vector<std::string> searchPaths;

	//Google ���Ƽ�Ӳ����/sdcard������android.os,Environment.getExternalStorageDirectory()��ȡ�ⲿ�洢Ŀ¼
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	JniMethodInfo getPath;
	jstring mstr;
	char mstrcpy[2000];
	if (JniHelper::getStaticMethodInfo(getPath, "org/cocos2dx/cpp/MusicPlay", "getMyStorage", "()Ljava/lang/String;"))
		{
			mstr = (jstring)(getPath.env->CallStaticObjectMethod(getPath.classID, getPath.methodID));
			strcpy(mstrcpy,getPath.env->GetStringUTFChars(mstr, false));
			CCLOG("ExtStoragePath = %s",mstrcpy);
			searchPaths.push_back(std::string(mstrcpy));
		}	
		searchPaths.push_back(std::string("/storage/extSdCard/HereticTrainer"));	//�ȴ�Google����API
		initAudioEngine();
#endif	//�õ��ⲿ�������Ŀ¼

#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)

	searchPaths.push_back("Resources/");
	searchPaths.push_back("../HereticTrainer/");
#endif
	FileUtils::getInstance()->setSearchPaths(searchPaths);
    // turn on display FPS
	
    director->setDisplayStats(false);
	//glview->setFrameSize(1280,720);
	glview->setDesignResolutionSize(1080, 720, ResolutionPolicy::SHOW_ALL);//�ֱ���ƥ��
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	
	if (!UserDefault::getInstance()->getBoolForKey("ok"))
	{
		
		UserDefault::getInstance()->setBoolForKey("ok", true);
		UserDefault::getInstance()->setDoubleForKey("rate",1.0);
		UserDefault::getInstance()->setDoubleForKey("badtime_ms", 180);
		UserDefault::getInstance()->setDoubleForKey("goodtime_ms", 120);
		UserDefault::getInstance()->setDoubleForKey("greattime_ms", 65);
		UserDefault::getInstance()->setDoubleForKey("perfecttime_ms", 30);
		UserDefault::getInstance()->setDoubleForKey("touchdis", 142);
		UserDefault::getInstance()->setDoubleForKey("touchwidth", 80);
		UserDefault::getInstance()->setDoubleForKey("touchheight",140);
		UserDefault::getInstance()->flush();
		
	}
    // create a scene. it's an autorelease object

	auto scene = SelectSong::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	if(!thisgame -> paused)
	{
		thisgame -> StopSence();
		// if you use SimpleAudioEngine, it must be pause
		#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			Pause();
		#else
			experimental::AudioEngine::pauseAll(); 
		#endif
	}
	Director::getInstance()->stopAnimation();
	
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
}
