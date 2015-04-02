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

	//	nodeQueue[rh.pos].push_back(nodeinfo);	//�ڽ����Ӧ��lane�����node
	//	nodeinfo.index = nodeQueue[rh.pos].size() - 1;
	//dspeed���������м乤�����ٶ�
	double speed = this->song.dSpeed;	//->hard128�٣�ex160�٣�����hard����ȦȦ�ӳ�����������ͷ����Ҫ1�룬��ex����Ϊ128/160��
	float start_t1 = speed * this -> songconfig.rate;//ͷԲ������������ͷ����Ҫ��ʱ��
	float dis = vGameArea[rh.pos].distance(vBornPoint);//�õ���ͷ��ͳ�����ľ���


	float end_t1 = (rh.endTime - rh.beginTime)*this->songconfig.rate;//ͷԲ�������ӳ�ʱ��
	float end_t2 = speed*this->songconfig.rate;//βԲ������ͷ����Ҫ��ʱ��


	Vec2 vGoal = vGameArea[rh.pos];//�Զ���ֱ�����Ա�͵�λ��

	//������sequence������һ��spawn��ͷ��ȦȦ��start_t1��������С�����Ҵ�ԭ���ƶ�����Ӧͷ�񣩣����Ž���ͷ�̶���ͷ�����沢���Ż�������ֶ�����combo�����ӣ���Ȼ����β�˶���ͷ�񣬲��Ż��򶯻���combo���ӣ��������ֶ���
	//
	ScaleTo* st1 = ScaleTo::create(start_t1, 1);	//ȦȦ��С����
	MoveTo* mt1 = MoveTo::create(start_t1, vGameArea[rh.pos]);//ȦȦ��Զ����
	//
	Sequence *sq1 = Sequence::create(
		Spawn::create(st1,mt1,NULL),
		CCCallFuncN::create([=](Ref*sender)
	{
		showPressEffect(rh.pos);
		showScoreEffect(Score::PREJECT);
		//����
		//��������������ԭ��λ��Maingame
		RepeatForever* rpt = RepeatForever::create(Sequence::create(EaseIn::create(FadeOut::create(1), 2), EaseIn::create(FadeIn::create(1), 2), NULL));
		nodeinfo.noodle->runAction(rpt);
	})
		, NULL);
	//������β����Ȧ,�󲿷ֺ�����һ��,���ǳ���ʱ���ͷ�ӳ���end - start
	//
	ScaleTo* st2 = ScaleTo::create(end_t2, 1);	//β��ȦȦ��С����
	MoveTo* mt2 = MoveTo::create(end_t2, vGameArea[rh.pos]);//β��ȦȦ��Զ����
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
	//��ʼ��������
	nodeinfo.head->runAction(sq1);
	nodeinfo.tail->runAction(sq2);

	nodeinfo.noodle->scheduleUpdate();
}//��������	,autoplay�����
void AutoPlay::Deal_with_tap(NodeInfo &nodeinfo, const Rhythm &rh)
{
	cocos2d::log("Auto Tap");
	nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
	nodeinfo.head->setPosition(539, 539);
	nodeinfo.head->setScale(0);
	this->addChild(nodeinfo.head, 10);
	//	nodeQueue[rh.pos].push_back(nodeinfo);	//�ڽ����Ӧ��lane�����node
	//	nodeinfo.index = nodeQueue[rh.pos].size() - 1;
	//dspeed���������м乤�����ٶ�
	double speed = this->song.dSpeed;	//->hard128�٣�ex160�٣�����hard����ȦȦ�ӳ�����������ͷ����Ҫ1�룬��ex����Ϊ128/160��
	float start_t1 = speed * this -> songconfig.rate;//ͷԲ������������ͷ����Ҫ��ʱ��
	float dis = vGameArea[rh.pos].distance(vBornPoint);//�õ���ͷ��ͳ�����ľ���
	float end_t1 = (rh.endTime - rh.beginTime)*this->songconfig.rate;//ͷԲ�������ӳ�ʱ��
	float end_t2 = speed*this->songconfig.rate;//βԲ������ͷ����Ҫ��ʱ��
	Vec2 vGoal = vGameArea[rh.pos];//�Զ���ֱ�����Ա�͵�λ��
	//����
	//������sequence������һ��spawn��ͷ��ȦȦ��start_t1��������С�����Ҵ�ԭ���ƶ�����Ӧͷ�񣩣����Ž���ͷ�̶���ͷ�����沢���Ż�������ֶ�����combo�����ӣ���Ȼ����β�˶���ͷ�񣬲��Ż��򶯻���combo���ӣ��������ֶ���
	//
	ScaleTo* st1 = ScaleTo::create(start_t1, 1);	//ȦȦ��С����
	MoveTo* mt1 = MoveTo::create(start_t1, vGameArea[rh.pos]);//ȦȦ��Զ����
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
	//��ʼ��������
	nodeinfo.head->runAction(sq1);
}//��������	,autoplay����и���
