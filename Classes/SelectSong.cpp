#include "SelectSong.h"
#include<math.h>
#include<sstream>
#include"MainGame.h"
#include "ui/CocosGUI.h"
#include "SettingScene.h"
#include"RandomSong.h"
#include <locale>
#include "AutoPlay.h"

USING_NS_CC;

Scene* SelectSong::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SelectSong::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SelectSong::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto sprite = Sprite::create("view_bk_start.png");
	sprite->setPosition(Vec2(540,360));
	this->addChild(sprite, 0);






	songlist = DataManager::getSongInfoList();


	createSprite(0);
	auto btLeft = ui::Button::create("left.png");
	btLeft->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED&&curPos > 0)
		{ 
			this->removeSprite();
			this->createSprite(curPos - 1); 
			
		}
	});
	btLeft->setPosition(Vec2(253, 720 - 247));
	auto btRight= ui::Button::create("right.png");
	btRight->addTouchEventListener(
		[=](Ref *pSender, ui::Widget::TouchEventType type)
		{
		if (type == ui::Widget::TouchEventType::ENDED&&curPos <songlist.size()-1)
		{ 
			this->removeSprite();
			this->createSprite(curPos + 1);
		}
	});
	btRight->setPosition(Vec2(854, 720 - 247));
	
	addChild(btLeft,10);
	addChild(btRight,10);




	auto btNormal = ui::Button::create("button_normal.png");
	btNormal->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			StartGame(160);
		}
	});
	btNormal->setPosition(Vec2(561, 720 - 626));
	btNormal->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btNormal, 13);

	auto btFast = ui::Button::create("button_fast.png");
	btFast->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			StartGame(200);
		}
	});
	btFast->setPosition(Vec2(909, 720 -626));
	btFast->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btFast, 13);

	auto btSlow = ui::Button::create("botton_slow.png");
	btSlow->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			StartGame(128);
		}
	});
	btSlow->setPosition(Vec2(210, 720 - 626));
	btSlow->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btSlow, 13);

	auto btReturn = ui::Button::create("button_return.png");
	btReturn->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->end();


		}
	});
	btReturn->setPosition(Vec2(51, 720 - 42));
	btReturn->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btReturn, 13);





	auto btSetting = ui::Button::create("button_setting.png");
	btSetting->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto scene = SettingScene::scene();
			Director::getInstance()->pushScene(scene);
		}
	});
	btSetting->setPosition(Vec2(1047, 720 - 34));
	btSetting->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btSetting, 13);

	//
	cbRandom_new = ui::CheckBox::create(
		"selecter_random_nor.png", 
		"selecter_random_nor.png",
		"selecter_random_press.png",
		"selecter_random_nor.png", 
		"selecter_random_press.png");
	cbRandom_new->setPosition(Vec2(780, 720 - 520));
	cbRandom_new->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(cbRandom_new, 13);
	
	auto lbRandom_new = Label::createWithSystemFont("使用新随机，否则旧版", "Arial", 24);
	lbRandom_new->setPosition(Vec2(940, 720 - 520));
	addChild(lbRandom_new, 13);
	//

	cbRandom_enable = ui::CheckBox::create(
		"selecter_random_nor.png", 
		"selecter_random_nor.png",
		"selecter_random_press.png",
		"selecter_random_nor.png", 
		"selecter_random_press.png");
	cbRandom_enable->setPosition(Vec2(380, 720 - 520));
	cbRandom_enable->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(cbRandom_enable, 13);
	
	auto lbRandom_enable= Label::createWithSystemFont("启用随机谱面", "Arial", 24);
	lbRandom_enable->setPosition(Vec2(480, 720 - 520));
	addChild(lbRandom_enable, 13);

	//Auto
	AutoPlay_enable = ui::CheckBox::create(
		"selecter_random_nor.png", 
		"selecter_random_nor.png",
		"selecter_random_press.png",
		"selecter_random_nor.png", 
		"selecter_random_press.png");
	AutoPlay_enable->setPosition(Vec2(190, 720 - 520));
	AutoPlay_enable->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(AutoPlay_enable, 13);
	
	auto AutoPlay= Label::createWithSystemFont("启用Auto", "Arial", 24);
	AutoPlay->setPosition(Vec2(220, 720 - 520));
	addChild(AutoPlay, 13);


	return true;
}
void SelectSong::StartGame(int Speed)
{
			
			SongConfig config;
			config.baddis = UserDefault::getInstance()->getDoubleForKey("baddis");
			config.gooddis = UserDefault::getInstance()->getDoubleForKey("gooddis");
			config.greatdis = UserDefault::getInstance()->getDoubleForKey("greatdis");
			config.perfectdis = UserDefault::getInstance()->getDoubleForKey("perfectdis");
			config.rate = Speed / 128;
			Song song = DataManager::loadDataFile(songlist[curPos].sDataPath, songlist[curPos]);
			bool enable_ran = cbRandom_enable->getSelectedState();
			bool enable_ran_new = cbRandom_new->getSelectedState();
			if (enable_ran && enable_ran_new)
				song = Randomize(song, RANDOM_NEW);
			else if(enable_ran && !enable_ran_new)
				song = Randomize(song, RANDOM_OLD);
			
			//log("^%s\n", songlist[curPos].sBackgroundPath.c_str());
			if (song.bUsable)
			{
				Scene* scene;
				if(!AutoPlay_enable->getSelectedState())	
					scene = MainGame::createScene(songlist[curPos],song,config);
				else
					scene = AutoPlay::createAutoScene(songlist[curPos],song,config,GAMEMODE_AUTO);
				Director::getInstance()->pushScene(scene);
			}
			

}

void SelectSong::createSprite(int pos)
{
	curPos = pos;
	
	std::string sSpritePath = songlist[pos].sTitlePath;
	


	lbName = Label::create(songlist[pos].sDisplayName, "Arial", 42);
	lbName->setPosition(Vec2(553, 720 - 480));
	lbName->setOpacity(0);
	lbName->runAction(FadeIn::create(0.5));
	spTitle = Sprite::create(sSpritePath);;
	spTitle->setOpacity(0);
	spTitle->setPosition(Vec2(553, 720 - 247));
	spTitle->runAction(FadeIn::create(0.5));
	spTitle->setContentSize(Size(300, 300));
	addChild(lbName,5);
	addChild(spTitle, 5);
}
void SelectSong::removeSprite()
{
	auto curSprite = this->spTitle;
	auto curLabel = this->lbName;
	
	curSprite->runAction(Sequence::create(FadeOut::create(0.5), CCCallFunc::create([=](){curSprite->removeFromParent(); }), NULL));
	curLabel->runAction(Sequence::create(FadeOut::create(0.5), CCCallFunc::create([=](){curLabel->removeFromParent(); }), NULL));
}
