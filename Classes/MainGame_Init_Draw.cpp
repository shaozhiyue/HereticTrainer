#include "MainGame.h"
#include <algorithm>
#include <sstream>
#include <math.h>
#include "audio\include\AudioEngine.h"
#include "AndroidAudio.h"
#include "Autoplay.h"
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
	//击打note之后显示的特效
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
	//播放音乐
	//log("Anzer init end");
	if (songconfig.bPlayMusic)
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if(Prepare(songinfo.sMusicPath.c_str()))
		{
			PlayMusic();
		}
#else
		experimental::AudioEngine::play2d(songinfo.sMusicPath);
#endif
	//log("Anzer init end2");
	memset(queueHead, 0, sizeof(queueHead));
	this->song = song;
	this->songinfo = songinfo;
	this->songconfig = songfig;
	Init_Background();
	Init_Spr_Score_cb();
	if(!(mGameMode & GAMEMODE_AUTO))	//自动打谱是不初始化触摸的  
	{
		Init_TouchLayer();
	}
	//暂停按钮
	btStop = ui::Button::create("stopbutton.png");
	btStop->setPosition(Vec2(1050, 692));
	btStop->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
			this->StopSence(); 
	});
	addChild(btStop, 0);
	//开启循环制造旋律
	this->schedule(schedule_selector(MainGame::update), 1/60);
	return true;
}
void MainGame::Init_Background()
{
	//创建背景
	auto spBackGround = Sprite::create(songinfo.sBackgroundPath,Rect(0,0,1080,720));
	spBackGround->setPosition(540,360);
	addChild(spBackGround, 0);
	//创建头像
	for (int i = 0; i < 9; i++)
	{
		auto sptmp = Sprite::create(songinfo.sPressButtonPath[i]);
		sptmp->setPosition(vGameArea[i]);
		addChild(sptmp, 5);
	}
	//圆环出生点 音符动画
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
	//波动的圈圈创建完毕
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
	auto spOnfuku = Sprite::create("onfuku.png");	//音乐标志，圈圈从这里出
	spOnfuku->setPosition(vBornPoint);
	spOnfuku->setScale(0.8);
	auto sqOnfuku = RepeatForever::create(Sequence::create(EaseIn::create(ScaleTo::create(1, 1), 2), EaseIn::create(ScaleTo::create(1, 0.8), 2), NULL));
	spOnfuku->runAction(sqOnfuku);
	addChild(spOnfuku, 10);
	//波动的圈圈动作设定完毕，播放动画
}
void MainGame::Init_Spr_Score_cb()
{
	//评分精灵创建
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


	//combo显示
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
	//创建触摸层
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
	nodeinfo.head->setScale(0);//长圆环的头圆环

	nodeinfo.tail = Sprite::create("r1.png");
	nodeinfo.tail->setPosition(539, 539);
	nodeinfo.tail->setScale(0);

	nodeinfo.noodle = CustomDrawNode::create();//画梯形
	nodeinfo.noodle->setPosition(Vec2::ZERO);
	nodeinfo.noodle->setAnchorPoint(Vec2::ZERO);
	nodeinfo.noodle->head = nodeinfo.head;
	nodeinfo.noodle->tail = nodeinfo.tail;
	nodeinfo.noodle->pattern = true;
	nodeinfo.noodle->tm = 0;
	nodeinfo.noodle->atm = rh.endTime - rh.beginTime;//持续时间
	addChild(nodeinfo.head, 10);
	addChild(nodeinfo.noodle, 8);
	addChild(nodeinfo.tail, 10);
	nodeQueue[rh.pos].push_back(nodeinfo);	//在节奏对应的lane加入该node
	nodeinfo.index = nodeQueue[rh.pos].size() - 1;


	
	//dspeed是真正在中间工作的速度
	double speed = songconfig.rate;	//->hard128速，ex160速，假设hard里面圈圈从出生到运行至头像需要1秒，则ex里面为128/160秒
	float start_t1 = songconfig.rate;//头圆环出生到到达头像处需要的时间
	float dis = vGameArea[rh.pos].distance(vBornPoint);//该道的头像和出生点的距离

	float start_t2 = (songconfig.baddis / dis) * songconfig.rate;;//判定为miss需要的时间

	float end_t1 = (rh.endTime - rh.beginTime);//头圆环出现到尾圆环出现，中间差的时间
	float end_t2 = songconfig.rate;//尾圆环到达头像处需要的时间

	//计算miss点，坐标
	float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (songconfig.rate);
	float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (songconfig.rate);
	//利用速度合成公式
	float end_t3 = (songconfig.baddis / dis)*end_t2;
	//end_t3:Bad_margin X end_t2 / DISTANCE_ORI_TARGET 总之是个比率，乘以速度就能得到判定miss时额外坐标....(这么麻烦？。。。。好吧尾巴是会飞过去的，会判出miss )
	Vec2 vGoal =Vec2 (vGameArea[rh.pos].x + speedx*end_t3, vGameArea[rh.pos].y + speedy*end_t3 );//miss时的位置
	//动作
	//长条的sequence：创建一个spawn（头部圈圈用start_t1到正常大小，并且从原点移动到对应头像），最后调用一个匿名函数判断是否miss，下一个sequence为梯形，即为条体，条体执行完毕以后即杀掉已经放出的node对象
	Sequence *sq1 = Sequence::create(
		Spawn::create(ScaleTo::create(start_t1, 1), MoveTo::create(start_t1, vGameArea[rh.pos]), NULL),					DelayTime::create(start_t2)
		, CCCallFuncN::create([=](Ref*sender)
	{
		nodeinfo.head->setVisible(false);
		nodeinfo.tail->setVisible(false);
		nodeinfo.noodle->setVisible(false);
		Score result = nodeQueue[rh.pos][nodeinfo.index].result;
		//nodequeue里面有9个位置，代表9个头像出现的位置，中间存的是NodeInfo，即每个到来的圈圈的信息，此处使用了原JSON的结构，将9个轨道的note分9个vector储存了。。。。。 -0 0什么鬼！这里其实就是取出对应的note啦
		if (result == Score::NONE)
		{	
			nodeQueue[rh.pos][nodeinfo.index].result = Score::MISS;
			showScoreEffect(Score::MISS);
			//如果没有任何评分则算miss了
		}
	})
		, NULL);
	//长条的尾部光圈,大部分和上面一样,但是出现时间比头延迟了end - start
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
	//开始动作运行
	nodeinfo.head->runAction(sq1);
	nodeinfo.tail->runAction(sq2);
	nodeinfo.noodle->scheduleUpdate();
}
void MainGame::Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh)
{
	nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
	nodeinfo.head->setPosition(539, 539);
	nodeinfo.head->setScale(0);
	this->addChild(nodeinfo.head, 10);



	//这里的song.speed并不表示速度，详细见定义
	double speed = songconfig.rate;

	float t1 = songconfig.rate;
	float dis = vGameArea[rh.pos].distance(vBornPoint);
	float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (songconfig.rate);
	float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (songconfig.rate);
	float t2 = (songconfig.baddis/ dis)*t1;
	Vec2 vGoal =Vec2(vGameArea[rh.pos].x + speedx*t2, vGameArea[rh.pos].y+speedy*t2);//miss时的位置
	nodeQueue[rh.pos].push_back(nodeinfo);
	nodeinfo.index = nodeQueue[rh.pos].size() - 1;

	auto ac = Sequence::create(
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
	nodeinfo.head->runAction(ac);
}




