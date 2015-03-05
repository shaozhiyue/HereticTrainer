#include "AutoPlay.h"
USING_NS_CC;
Scene* AutoPlay::createAutoScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig, int GameMode)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = AutoPlay::createAuto(songinfo, song, songfig, GameMode);
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}
void AutoPlay::Deal_with_long(NodeInfo &nodeinfo, const Rhythm &rh)
{
	cocos2d::log("Auto Long");
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

	//	nodeQueue[rh.pos].push_back(nodeinfo);	//在节奏对应的lane加入该node
	//	nodeinfo.index = nodeQueue[rh.pos].size() - 1;
	//dspeed是真正在中间工作的速度
	double speed = songconfig.rate;	//->hard128速，ex160速，假设hard里面圈圈从出生到运行至头像需要1秒，则ex里面为128/160秒
	float start_t1 = songconfig.rate;//头圆环出生到到达头像处需要的时间
	float dis = vGameArea[rh.pos].distance(vBornPoint);//该道的头像和出生点的距离


	float end_t1 = (rh.endTime - rh.beginTime);//头圆环出现到尾圆环出现，中间差的时间
	float end_t2 = songconfig.rate;//尾圆环到达头像处需要的时间


	Vec2 vGoal = vGameArea[rh.pos];//自动谱直接算成员和的位置

	//长条的sequence：创建一个spawn（头部圈圈用start_t1到正常大小，并且从原点移动到对应头像），接着将条头固定于头像上面并播放击打和评分动画（combo不增加），然后条尾运动到头像，播放击打动画，combo增加，播放评分动画
	//
	ScaleTo* st1 = ScaleTo::create(start_t1, 1);	//圈圈从小到大
	MoveTo* mt1 = MoveTo::create(start_t1, vGameArea[rh.pos]);//圈圈从远到近
	//
	Sequence *sq1 = Sequence::create(
		Spawn::create(st1,mt1,NULL),
		CCCallFuncN::create([=](Ref*sender)
	{
		showPressEffect(rh.pos);
		showScoreEffect(Score::PREJECT);
		//动作
		//长条的条体绘出：原来位于Maingame
		RepeatForever* rpt = RepeatForever::create(Sequence::create(EaseIn::create(FadeOut::create(1), 2), EaseIn::create(FadeIn::create(1), 2), NULL));
		nodeinfo.noodle->runAction(rpt);
	})
		, NULL);
	//长条的尾部光圈,大部分和上面一样,但是出现时间比头延迟了end - start
	//
	ScaleTo* st2 = ScaleTo::create(end_t2, 1);	//尾巴圈圈从小到大
	MoveTo* mt2 = MoveTo::create(end_t2, vGameArea[rh.pos]);//尾巴圈圈从远到近
	//
	Sequence *sq2 = Sequence::create(
		DelayTime::create(end_t1), Spawn::create(st2,mt2, NULL)
		, CCCallFuncN::create([=](Ref*sender)
	{
		nodeinfo.head->setVisible(false);
		nodeinfo.tail->setVisible(false);
		nodeinfo.noodle->setVisible(false);
		showPressEffect(rh.pos);
		showScoreEffect(Score::PREJECT);
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
}//创建长条	,autoplay类进行
void AutoPlay::Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh)
{
	cocos2d::log("Auto Tap");
	nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
	nodeinfo.head->setPosition(539, 539);
	nodeinfo.head->setScale(0);
	this->addChild(nodeinfo.head, 10);
	//	nodeQueue[rh.pos].push_back(nodeinfo);	//在节奏对应的lane加入该node
	//	nodeinfo.index = nodeQueue[rh.pos].size() - 1;
	//dspeed是真正在中间工作的速度
	double speed = songconfig.rate;	//->hard128速，ex160速，假设hard里面圈圈从出生到运行至头像需要1秒，则ex里面为128/160秒
	float start_t1 = songconfig.rate;//头圆环出生到到达头像处需要的时间
	float dis = vGameArea[rh.pos].distance(vBornPoint);//该道的头像和出生点的距离
	Vec2 vGoal = vGameArea[rh.pos];//自动谱直接算成员和的位置
	//动作
	//长条的sequence：创建一个spawn（头部圈圈用start_t1到正常大小，并且从原点移动到对应头像），接着将条头固定于头像上面并播放击打和评分动画（combo不增加），然后条尾运动到头像，播放击打动画，combo增加，播放评分动画
	//
	ScaleTo* st1 = ScaleTo::create(start_t1, 1);	//圈圈从小到大
	MoveTo* mt1 = MoveTo::create(start_t1, vGameArea[rh.pos]);//圈圈从远到近
	//
	Sequence *sq1 = Sequence::create(
		Spawn::create(st1,mt1,NULL),
		CCCallFuncN::create([=](Ref*sender)
	{
		showPressEffect(rh.pos);
		showScoreEffect(Score::PREJECT);
		this->removeChild(nodeinfo.head);
	})
		, NULL);
	//开始动作运行
	nodeinfo.head->runAction(sq1);
}//创建单键	,autoplay类进行覆盖
