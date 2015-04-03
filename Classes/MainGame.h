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
#define GAMEMODE_NORMAL 0x0	//LL��һ��ģʽ��û��ʲô��ֵ�
#define GAMEMODE_NIGHT 0x1 //��ҹģʽ��note���ٳ�������㣬�����˶�һ��ʱ��֮��ͻȻ����������ҷ�ӳʱ�䣬�����Ѷȣ�
#define GAMEMODE_AUTO 0x2 //�Զ�ģʽ��ר�ι۲ⷴ�����Ѻ����������

const Vec2 vGameArea[9]= { Vec2(89, 539), Vec2(124, 364), Vec2(220, 219), Vec2(367, 122), Vec2(539, 88)
		, Vec2(712, 122), Vec2(858, 219), Vec2(954, 364), Vec2(989, 539) };

	const Vec2 vBornPoint = Vec2(539, 539);
	const double iRadius = 62;
struct NodeInfo
{
	cocos2d::Sprite* head;
	cocos2d::Sprite* tail;
	CustomDrawNode* noodle;
	int type;	//����
	int index;	//node�ڵ�ǰ����ͷ��lane����note���е�λ��
	Score result;
	Score result_tail;
	Rhythm* rh;
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
	//����һЩ�����ָ�뷽�����
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
	cocos2d::Label* lbAccuracy;

	int curRhythm;
	int curRhythm_passed;
	int curRhythm_auto;
	double curTime;
	//��¼
	int maxCombo;
	int curCombo;
	int cntPerfect;
	int cntGreat;
	int cntGood;
	int cntBad;
	int cntMiss;
	bool paused;
	//��Ϸ�Ķ���ģʽ
	int mGameMode;
	float AverageAccuracy;	//ƽ��׼ȷ��
	float AverageDelay;		//ƽ���ӳ�
	Song song;//��ǰ�ĸ�������
	SongInfo songinfo;
	SongConfig songconfig;


	std::vector<NodeInfo> nodeQueue[9];	//nodequeue��1-9��λ�ϳ��ֵ�note����Ϣ
	int queueHead[9];
	std::unordered_map<cocos2d::Touch*, int > table;//����ĳ�δ�����������һ������Բ����
	std::unordered_map<int, Rhythm> table_auto;//�Զ��ף�������������
	MainGame()
	{
		curRhythm_auto = 0;
		curRhythm = 0;
		curRhythm_passed = 0;
		curTime = 0;
		maxCombo = 0;
		curCombo = 0;
		cntPerfect = 0;
		cntGreat = 0;
		cntGood = 0;
		cntBad = 0;
		cntMiss = 0;
		mGameMode = GAMEMODE_NORMAL;
		AverageAccuracy = 0;	//ƽ��׼ȷ��
		AverageDelay = 0;
		paused = false;
	}

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(const SongInfo &songinfo,const Song &song, const SongConfig &songfig);
	static cocos2d::Scene* createScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode);
	bool init(const SongInfo& songinfo, const Song &song, const SongConfig &songfig);
	static MainGame* create(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode);
	virtual void Deal_with_long(NodeInfo &nodeinfo, const Rhythm &rh);//��������	,autoplay����и���
	virtual void Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh);//��������	,autoplay����и���
	void Init_Spr_Score_cb(); //����combo�ͷ������۵�spr
	void Init_TouchLayer();//����������
	void Init_Background();//����ͼ����
	void born(const Rhythm &rh);//c����Բ
	void AutoPlay(float t);	//�Զ�����

	//�����¼�
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void Calcscore_Touchbegin(Vec2* Coord, Rhythm r);
	void Calcscore_touchend();
	//֡����
	void update(float dt);
	//��ʾ�������
	void showPressEffect(int pos);
	//��ʾ���ֶ���
	void showScoreEffect(Score score);
	//��ͣ����
	void  StopSence();
	//�������
	void  ResultScene();
	//�����Ƿ�����ĳһ��
	bool checkTouch(int pos, const Vec2 &touchLocation);

};
#endif 
