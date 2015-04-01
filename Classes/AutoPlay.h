#ifndef __AUTOPLAY_MAINGAME_H__
#define __AUTOPLAY_MAINGAME_H__
#include "MainGame.h"
#include "cocos2d.h"
class AutoPlay:MainGame
{
public:
	AutoPlay()
	{
		Frames = 0;
		curRhythm = 0;
		curTime = 0;
		maxCombo = 0;
		curCombo = 0;
		cntPerfect = 0;
		cntGreat = 0;
		cntGood = 0;
		cntBad = 0;
		cntMiss = 0;
		mGameMode = GAMEMODE_NORMAL;
	}
	static cocos2d::Scene* createAutoScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode);
	static AutoPlay* createAuto(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode)
	{  
		cocos2d::log("Auto Play Start\n");
		AutoPlay *pRet = new AutoPlay(); 
		pRet->mGameMode = GameMode;
		if (pRet && pRet->init(songinfo,song,songfig))
		{ 
			pRet->autorelease(); 
			return pRet; 
		} 
		else 
		{ 
			delete pRet; 
			pRet = NULL; 
			return NULL; 
		} 
	}
	void Deal_with_long(NodeInfo &nodeinfo, const Rhythm &rh);//创建长条	,autoplay类进行
	void Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh);//创建单键	,autoplay类进行覆盖
};
#endif