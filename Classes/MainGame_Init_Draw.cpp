#include "MainGame.h"
#include <algorithm>
#include <sstream>
#include <math.h>
#include "audio\include\AudioEngine.h"
#include "AndroidAudio.h"
#include "Autoplay.h"
#include "Global_timeline_varibles.h"
USING_NS_CC;
using namespace ui;



Scene* MainGame::createScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = MainGame::create(songinfo, song, songfig, GAMEMODE_NORMAL);
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}
Scene* MainGame::createScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = MainGame::create(songinfo, song, songfig, GameMode);
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene983 
	return scene;
}
void MainGame::showPressEffect(int pos)
{
	//����note֮����ʾ����Ч
	auto spRing = Sprite::create("ring.png");
	spRing->setPosition(vGameArea[pos]);
	spRing->setScale(0.4);
	auto sqRing = Sequence::create(Spawn::create(FadeOut::create(0.2), ScaleTo::create(0.1, 1), NULL),
		CCCallFuncN::create([=](Ref *sender){this->removeChild(spRing); }), NULL);;
	spRing->runAction(sqRing);
	this->addChild(spRing, 5);

	auto spStar = Sprite::create("star.png");
	spStar->setPosition(vGameArea[pos]);
	auto sqStar = Sequence::create(DelayTime::create(0.1), FadeOut::create(0.1),
		CCCallFuncN::create([=](Ref *sender){this->removeChild(spStar); }), NULL);
	spStar->runAction(sqStar);
	this->addChild(spStar, 5);


}
// on "init" you need to initialize your instance
bool MainGame::init(const SongInfo &songinfo, const Song &song, const SongConfig &songfig)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	log("Anzer init begin");
	memset(queueHead, 0, sizeof(queueHead));
	memset(queueTail, -1, sizeof(queueTail));
	this->song = song;
	this->songinfo = songinfo;
	this->songconfig = songfig;
	Init_Background();
	Init_Spr_Score_cb();
	if(!(mGameMode & GAMEMODE_AUTO))	//�Զ������ǲ���ʼ��������  
	{
		Init_TouchLayer();
	}
	//��ͣ��ť
	btStop = ui::Button::create("stopbutton.png");
	btStop->setPosition(Vec2(1050, 692));
	btStop->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
			this->StopSence(); 
	});
	addChild(btStop, 0);
	//����ѭ����������
	this->schedule(schedule_selector(MainGame::update), 1/60);
	//
	//��¼����������ɵ�ʱ�䣬��Ϊ���ֿ�ʼʱ��
	SystemTime::Music_start = SystemTime::getSystemTime();
	SystemTime::Pause_time = 0;
	//
	return true;
}
void MainGame::Init_Background()
{
	//��������
	auto spBackGround = Sprite::create(songinfo.sBackgroundPath,Rect(0,0,1080,720));
	spBackGround->setPosition(540,360);
	addChild(spBackGround, 0);
	//����ͷ��
	for (int i = 0; i < 9; i++)
	{
		auto sptmp = Sprite::create(songinfo.sPressButtonPath[i]);
		sptmp->setPosition(vGameArea[i]);
		addChild(sptmp, 5);
	}
	//Բ�������� ��������
	auto spRing1 = Sprite::create("ring2.png");
	auto spRing2 = Sprite::create("ring2.png");
	auto spRing3 = Sprite::create("ring2.png");
	auto spRing4 = Sprite::create("ring2.png");
	spRing1->setPosition(vBornPoint);
	spRing2->setPosition(vBornPoint);
	spRing3->setPosition(vBornPoint);
	spRing4->setPosition(vBornPoint);
	spRing1->setOpacity(0);
	spRing2->setOpacity(0);
	spRing3->setOpacity(0);
	spRing4->setOpacity(0);
	//������ȦȦ�������
	auto sqRing1 = RepeatForever::create(Sequence::create(
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(1.4),
		NULL));
	auto sqRing2 = RepeatForever::create(Sequence::create(
		DelayTime::create(0.2),
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(1.2),
		NULL));
	auto sqRing3 = RepeatForever::create(Sequence::create(
		DelayTime::create(0.4),
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(1.0),
		NULL));
	auto sqRing4 = RepeatForever::create(Sequence::create(
		DelayTime::create(0.6),
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(0.8),
		NULL));
	spRing1->runAction(sqRing1);
	spRing2->runAction(sqRing2);
	spRing3->runAction(sqRing3);
	spRing4->runAction(sqRing4);
	addChild(spRing1, 9);
	addChild(spRing2, 9);
	addChild(spRing3, 9);
	addChild(spRing4, 9);
	auto spOnfuku = Sprite::create("onfuku.png");	//���ֱ�־��ȦȦ�������
	spOnfuku->setPosition(vBornPoint);
	spOnfuku->setScale(0.8);
	auto sqOnfuku = RepeatForever::create(Sequence::create(EaseIn::create(ScaleTo::create(1, 1), 2), EaseIn::create(ScaleTo::create(1, 0.8), 2), NULL));
	spOnfuku->runAction(sqOnfuku);
	addChild(spOnfuku, 10);
	//������ȦȦ�����趨��ϣ����Ŷ���
}
void MainGame::Init_Spr_Score_cb()
{
	//���־��鴴��
	spPerfect = Sprite::create("perfect.png");
	spGreat = Sprite::create("great.png");
	spGood = Sprite::create("good.png");
	spBad = Sprite::create("bad.png");
	spMiss = Sprite::create("miss.png");
	spPerfect->setVisible(false);
	spGreat->setVisible(false);
	spGood->setVisible(false);
	spBad->setVisible(false);
	spMiss->setVisible(false);
	spPerfect->setPosition(Vec2(540, 360));
	spGreat->setPosition(Vec2(540, 360));
	spGood->setPosition(Vec2(540, 360));
	spBad->setPosition(Vec2(540, 360));
	spMiss->setPosition(Vec2(540, 360));

	addChild(spPerfect,30);
	addChild(spGreat, 30);
	addChild(spGood, 30);
	addChild(spBad, 30);
	addChild(spMiss, 30);


	//combo��ʾ
	lbCombo = Label::createWithSystemFont("combo", "Arial", 30);
	lbCombo->setPosition(Vec2(570,415));
	lbCombo->setVisible(false);
	addChild(lbCombo);
	lbComboCnt = Label::createWithSystemFont("141", "Arial", 55);
	lbComboCnt->setPosition(Vec2(490, 415));
	lbComboCnt->setAlignment(TextHAlignment::RIGHT);
	addChild(lbComboCnt);
	lbComboCnt->setVisible(false);;
}
void MainGame::Init_TouchLayer()
{
	//����������
	touchLayer = Layer::create();
	touchLayer->setAnchorPoint(Vec2::ZERO);
	touchLayer->setPosition(Vec2::ZERO);
	touchLayer->setContentSize(Size(1080,720));
	//touchLayer->setOpacity(0);
	addChild(touchLayer, 20);
	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(MainGame::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(MainGame::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(MainGame::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, touchLayer);
}
void MainGame::showScoreEffect(Score score)
{
	//	static  int cnt = 0;
	//	log("Anzer showScore-%d begin", cnt);
	spPerfect->stopAllActions();
	spGreat->stopAllActions();
	spGood->stopAllActions();
	spBad->stopAllActions();
	spMiss->stopAllActions();
	spPerfect->setVisible(false);
	spGreat->setVisible(false);
	spGood->setVisible(false);
	spBad->setVisible(false);
	spMiss->setVisible(false);
	std::stringstream buf;
	switch (score)
	{
	case Score::PREJECT:
		spPerfect->setVisible(true);
		spPerfect->setOpacity(255);
		spPerfect->setScale(0.4);
		spPerfect->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(true);
		lbCombo->setVisible(true);

		lbComboCnt->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		lbCombo->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		curCombo++;
		maxCombo = std::max(maxCombo, curCombo);
		buf << curCombo;
		lbComboCnt->setString(buf.str());
		cntPerfect++;
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectPerfect();
#else
		experimental::AudioEngine::play2d("perfect.ogg");
#endif

		break;
	case Score::GREAT:
		spGreat->setVisible(true);
		spGreat->setOpacity(255);
		spGreat->setScale(0.4);
		spGreat->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(true);
		lbCombo->setVisible(true);

		lbComboCnt->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		lbCombo->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));

		curCombo++;
		maxCombo = std::max(maxCombo, curCombo);
		buf << curCombo;
		lbComboCnt->setString(buf.str());
		cntGreat++;
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectGreat();
#else
		experimental::AudioEngine::play2d("great.ogg");
#endif

		break;
	case Score::GOOD:
		spGood->setVisible(true);
		spGood->setOpacity(255);
		spGood->setScale(0.4);
		spGood->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));

		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
		curCombo = 0;
		cntGood++;
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectGood();
#else
		experimental::AudioEngine::play2d("good.ogg");
#endif

		break;
	case Score::BAD:
		spBad->setVisible(true);
		spBad->setOpacity(255);
		spBad->setScale(0.4);
		spBad->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
		curCombo = 0;
		cntBad++;
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectGood();
#else
		experimental::AudioEngine::play2d("good.ogg");
#endif

		break;
	case Score::MISS:
		spMiss->setVisible(true);
		spMiss->setOpacity(255);
		spMiss->setScale(0.4);
		spMiss->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectMiss();
#else
		experimental::AudioEngine::play2d("miss.ogg");
#endif

		curCombo = 0;
		cntMiss++;
		break;
	default:
		break;
	}
	//	log("Anzer  showScore-%d end", cnt++);
}
void MainGame::Deal_with_long(NodeInfo &nodeinfo, const Rhythm &rh)
{
	nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
	nodeinfo.head->setPosition(539, 539);
	nodeinfo.head->setScale(0);//��Բ����ͷԲ��

	nodeinfo.tail = Sprite::create("r1.png");
	nodeinfo.tail->setPosition(539, 539);
	nodeinfo.tail->setScale(0);

	nodeinfo.noodle = CustomDrawNode::create();//������
	nodeinfo.noodle->setPosition(Vec2::ZERO);
	nodeinfo.noodle->setAnchorPoint(Vec2::ZERO);
	nodeinfo.noodle->head = nodeinfo.head;
	nodeinfo.noodle->tail = nodeinfo.tail;
	nodeinfo.noodle->pattern = true;
	nodeinfo.noodle->tm = 0;
	nodeinfo.noodle->atm = rh.endTime - rh.beginTime;//����ʱ��
	addChild(nodeinfo.head, 10);
	addChild(nodeinfo.noodle, 8);
	addChild(nodeinfo.tail, 10);
	nodeinfo.number = curRhythm;
	nodeQueue[rh.pos].push_back(nodeinfo);	//�ڽ����Ӧ��lane�����node
	queueTail[rh.pos] = nodeQueue[rh.pos].size() - 1;	//������еĶ�β
	nodeinfo.index = nodeQueue[rh.pos].size() - 1;


	
	//dspeed���������м乤�����ٶ�
	double speed = songconfig.rate;	//->hard128�٣�ex160�٣�����hard����ȦȦ�ӳ�����������ͷ����Ҫ1�룬��ex����Ϊ128/160��
	float start_t1 = songconfig.rate;//ͷԲ������������ͷ����Ҫ��ʱ��
	float dis = vGameArea[rh.pos].distance(vBornPoint);//�õ���ͷ��ͳ�����ľ���

	float start_t2 = (songconfig.baddis / dis) * songconfig.rate;;//�ж�Ϊmiss��Ҫ��ʱ��

	float end_t1 = (rh.endTime - rh.beginTime);//ͷԲ�����ֵ�βԲ�����֣��м���ʱ��
	float end_t2 = songconfig.rate;//βԲ������ͷ����Ҫ��ʱ��

	//����miss�㣬����
	float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (songconfig.rate);
	float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (songconfig.rate);
	//�����ٶȺϳɹ�ʽ
	float end_t3 = (songconfig.baddis / dis)*end_t2;
	//end_t3:Bad_margin X end_t2 / DISTANCE_ORI_TARGET ��֮�Ǹ����ʣ������ٶȾ��ܵõ��ж�missʱ��������....(��ô�鷳�����������ð�β���ǻ�ɹ�ȥ�ģ����г�miss )
	Vec2 vGoal =Vec2 (vGameArea[rh.pos].x + speedx*end_t3, vGameArea[rh.pos].y + speedy*end_t3 );//missʱ��λ��
	//����
	//������sequence������һ��spawn��ͷ��ȦȦ��start_t1��������С�����Ҵ�ԭ���ƶ�����Ӧͷ�񣩣�������һ�����������ж��Ƿ�miss����һ��sequenceΪ���Σ���Ϊ���壬����ִ������Ժ�ɱ���Ѿ��ų���node����
	Sequence *sq1 = Sequence::create(
		Spawn::create(ScaleTo::create(start_t1, 1), MoveTo::create(start_t1, vGameArea[rh.pos]), NULL),					DelayTime::create(start_t2)
		, CCCallFuncN::create([=](Ref*sender)
	{
		nodeinfo.head->setVisible(false);
		nodeinfo.tail->setVisible(false);
		nodeinfo.noodle->setVisible(false);
		Score result = nodeQueue[rh.pos][nodeinfo.index].result;
		//nodequeue������9��λ�ã�����9��ͷ����ֵ�λ�ã��м�����NodeInfo����ÿ��������ȦȦ����Ϣ���˴�ʹ����ԭJSON�Ľṹ����9�������note��9��vector�����ˡ��������� -0 0ʲô��������ʵ����ȡ����Ӧ��note��
		if (result == Score::NONE)
		{	
			nodeQueue[rh.pos][nodeinfo.index].result = Score::MISS;
			showScoreEffect(Score::MISS);
			//���û���κ���������miss��
		}
	})
		, NULL);
	//������β����Ȧ,�󲿷ֺ�����һ��,���ǳ���ʱ���ͷ�ӳ���end - start
	Sequence *sq2 = Sequence::create(
		DelayTime::create(end_t1), Spawn::create(ScaleTo::create(end_t2, 1), MoveTo::create(end_t2, vGameArea[rh.pos]), NULL), MoveTo::create(end_t3, vGoal)
		, CCCallFuncN::create([=](Ref*sender)
	{
		nodeinfo.head->setVisible(false);
		nodeinfo.tail->setVisible(false);
		nodeinfo.noodle->setVisible(false);

		Score result = nodeQueue[rh.pos][nodeinfo.index].result_tail;

		if (result == Score::NONE)
		{
			nodeQueue[rh.pos][nodeinfo.index].result_tail = Score::MISS;
			showScoreEffect(Score::MISS);
		}
	})
		, DelayTime::create(10), CCCallFuncN::create([=](Ref*sender)
	{
		this->removeChild(nodeinfo.head);
		this->removeChild(nodeinfo.tail);
		this->removeChild(nodeinfo.noodle);
	})

		, NULL);
	//��ʼ��������
	nodeinfo.head->runAction(sq1);
	nodeinfo.tail->runAction(sq2);
	nodeinfo.noodle->scheduleUpdate();
}
void MainGame::Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh)
{
	nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
	nodeinfo.head->setPosition(539, 539);
	nodeinfo.head->setScale(0);
	nodeinfo.number = curRhythm;
	this->addChild(nodeinfo.head, 10);
	nodeQueue[rh.pos].push_back(nodeinfo);
	nodeinfo.index = nodeQueue[rh.pos].size() - 1;
	queueTail[rh.pos] = nodeQueue[rh.pos].size() - 1;	//������еĶ�β

	//�����song.speed������ʾ�ٶȣ���ϸ������
	double speed = songconfig.rate;

	float t1 = songconfig.rate;
	float dis = vGameArea[rh.pos].distance(vBornPoint);
	float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (songconfig.rate);
	float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (songconfig.rate);
	float t2 = (songconfig.baddis/ dis)*t1;
	//Vec2 vGoal =Vec2(vGameArea[rh.pos].x + speedx*t2, vGameArea[rh.pos].y+speedy*t2);//missʱ��λ��


	/*auto ac = Sequence::create(
		Spawn::create(ScaleTo::create(t1, 1), MoveTo::create(t1, vGameArea[rh.pos]), NULL),
		MoveTo::create(t2, vGoal),
		CCCallFuncN::create([=](Ref *sender)
	{
		nodeinfo.head->setVisible(false);

		Score result = nodeQueue[rh.pos][nodeinfo.index].result;

		if (result == Score::NONE)
			nodeQueue[rh.pos][nodeinfo.index].result = Score::MISS;
		if (result == Score::NONE)
			showScoreEffect(Score::MISS);


	}), DelayTime::create(10), CCCallFuncN::create([=](Ref*sender)
	{
		this->removeChild(nodeinfo.head);
	})

		, NULL);
	nodeinfo.head->runAction(ac);*/
}




