#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__
#include<queue>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"CustomDrawNode.h"
#include"DataManager.h"
#include<unordered_map>
#include "MusicPlayer.h"
using cocos2d::Vec2;
enum class Score{ PREJECT = 1, GREAT,GOOD,BAD,MISS,NONE };
#define GAMEMODE_NORMAL 0x0	//LL的一般模式，没有什么奇怪的
#define GAMEMODE_NIGHT 0x1 //黑夜模式（note不再出现在起点，而是运动一段时间之后突然现身，减少玩家反映时间，增大难度）
#define GAMEMODE_AUTO 0x2 //自动模式，专治观测反人类多押面条观赏谱

const Vec2 vGameArea[9]= { Vec2(89, 539), Vec2(124, 364), Vec2(220, 219), Vec2(367, 122), Vec2(539, 88)
		, Vec2(712, 122), Vec2(858, 219), Vec2(954, 364), Vec2(989, 539) };

	const Vec2 vBornPoint = Vec2(539, 539);
	const double iRadius = 62;
struct NodeInfo
{
	cocos2d::Sprite* head;
	cocos2d::Sprite* tail;
	CustomDrawNode* noodle;
	int type;	//类型
	int index;	//node在当前人物头像（lane）的note列中的位置
	Score result;
	Score result_tail;
	NodeInfo()
	{	
		head = NULL;
		tail=NULL;
		noodle = NULL;
		type=0;
		index=0;
		result = Score::NONE;
		result_tail = Score::NONE;
	}

};

class MainGame : public cocos2d::Layer
{
public:
	//保存一些对象的指针方便操作
	cocos2d::EventListenerTouchAllAtOnce* listener;
	cocos2d::Layer *touchLayer;
	cocos2d::Sprite *spPerfect;
	cocos2d::Sprite *spGreat;
	cocos2d::Sprite *spGood;
	cocos2d::Sprite *spBad;
	cocos2d::Sprite *spMiss;
	cocos2d::Label* lbCombo;
	cocos2d::Label*  lbComboCnt;
	cocos2d::ui::Button* btStop;

	int curRhythm;
	int curRhythm_auto;
	double curTime;
	//记录
	int maxCombo;
	int curCombo;
	int cntPerfect;
	int cntGreat;
	int cntGood;
	int cntBad;
	int cntMiss;
	//游戏的额外模式
	int mGameMode;
	Song song;//当前的歌曲数据
	SongInfo songinfo;
	SongConfig songconfig;


	std::vector<NodeInfo> nodeQueue[9];	//nodequeue即1-9号位上出现的note的信息
	int queueHead[9];
	std::unordered_map<cocos2d::Touch*, int > table;//保存某次触摸是属于哪一个道的圆环的
	std::unordered_map<int, Rhythm> table_auto;//自动谱，长条音符保持
	MainGame()
	{
		curRhythm_auto = 0;
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

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(const SongInfo &songinfo,const Song &song, const SongConfig &songfig);
	static cocos2d::Scene* createScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode);
	bool init(const SongInfo& songinfo, const Song &song, const SongConfig &songfig);
	static MainGame* create(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode)
	{  
		MainGame *pRet = new MainGame(); 
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
	virtual void Deal_with_long(NodeInfo &nodeinfo, const Rhythm &rh);//创建长条	,autoplay类进行覆盖
	virtual void Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh);//创建单键	,autoplay类进行覆盖
	void Init_Spr_Score_cb(); //创造combo和分数评价的spr
	void Init_TouchLayer();//创建触摸层
	void Init_Background();//背景图绘制
	void born(const Rhythm &rh);//c产生圆
	void AutoPlay(float t);	//自动打谱

	//触摸事件
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void Calcscore_Touchbegin(Vec2* Coord, Rhythm r);
	void Calcscore_touchend();
	//帧更新
	void update(float dt);
	//显示点击动画
	void showPressEffect(int pos);
	//显示评分动画
	void showScoreEffect(Score score);
	//暂停界面
	void  StopSence();
	//结果界面
	void  ResultScene();
	//测试是否触摸了某一道
	bool checkTouch(int pos, const Vec2 &touchLocation);

};
#endif 
