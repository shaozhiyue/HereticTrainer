#include "MainGame.h"
#include "SelectSong.h"
USING_NS_CC;
using namespace ui;

void  MainGame::ResultScene()
{
	//屏幕截图作为背景
	RenderTexture *renderTexture = RenderTexture::create(1080, 720);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	Director::getInstance()->getRenderer()->render();

	//创建结果场景
	auto scResult = Scene::create();
	auto lyResult = Layer::create();
	scResult->addChild(lyResult);
	auto spBackGround = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	spBackGround->setPosition(540, 360);
	spBackGround->setFlippedY(true);
	lyResult->addChild(spBackGround, 0);


	auto spResult = Sprite::create("result.png");
	spResult->setPosition(540, 360);
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			Director::getInstance()->replaceScene(SelectSong::createScene());
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spResult);
	lyResult->addChild(spResult, 1);
	//写上得分等数据
	TTFConfig ttfConfig("fonts/arial.ttf", 42, GlyphCollection::DYNAMIC, nullptr, false);
	auto lbPerfect = Label::createWithTTF(ttfConfig, String::createWithFormat("%d",cntPerfect)->_string);
	lbPerfect->setColor(Color3B(255, 102, 153));
	lbPerfect->setPosition(Vec2(141, 720 - 586));
	lbPerfect->setAdditionalKerning(20);
	lyResult->addChild(lbPerfect, 2);

	auto lbGreat = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntGreat)->_string);
	lbGreat->setColor(Color3B(255, 102, 153));
	lbGreat->setPosition(Vec2(346, 720 - 586));
	lbGreat->setAdditionalKerning(20);
	lyResult->addChild(lbGreat, 2);

	auto lbGood = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntGood)->_string);
	lbGood->setColor(Color3B(255, 102, 153));
	lbGood->setPosition(Vec2(546, 720 - 586));
	lbGood->setAdditionalKerning(20);
	lyResult->addChild(lbGood, 2);

	auto lbBad = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntBad)->_string);
	lbBad->setColor(Color3B(255, 102, 153));
	lbBad->setPosition(Vec2(737, 720 - 586));
	lbBad->setAdditionalKerning(20);
	lyResult->addChild(lbBad, 2);

	auto lbMiss = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntMiss)->_string);
	lbMiss->setColor(Color3B(255, 102, 153));
	lbMiss->setPosition(Vec2(940, 720 - 586));
	lbMiss->setAdditionalKerning(20);
	lyResult->addChild(lbMiss, 2);



	auto lbScore = Label::createWithTTF(ttfConfig, "4444444");
	lbScore->setColor(Color3B(255, 102, 153));
	lbScore->setPosition(Vec2(655, 720 - 332));
	lbScore->setAdditionalKerning(20);
	lyResult->addChild(lbScore, 2);

	auto lbHScore = Label::createWithTTF(ttfConfig, "252525252");
	lbHScore->setColor(Color3B(255, 102, 153));
	lbHScore->setPosition(Vec2(655, 720 - 376));
	lbHScore->setAdditionalKerning(20);
	lyResult->addChild(lbHScore, 2);

	auto lbCombo = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", maxCombo)->_string);
	lbCombo->setColor(Color3B(255, 102, 153));
	lbCombo->setPosition(Vec2(655, 720 - 458));
	lbCombo->setAdditionalKerning(20);
	lyResult->addChild(lbCombo, 2);



	Director::getInstance()->replaceScene(scResult);

}

void  MainGame::StopSence()
{
	//	TextureCache::getInstance()->removeUnusedTextures();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Pause();
#else
	experimental::AudioEngine::pauseAll(); 
#endif
	RenderTexture *renderTexture = RenderTexture::create(1080, 720);
	renderTexture->begin();
	//renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	this->getParent()->visit();
	renderTexture->end();
	Director::getInstance()->getRenderer()->render();
	auto scStop = Scene::create();
	//TextureCache::getInstance()->removeUnusedTextures();

	auto lyStop = Layer::create();
	scStop->addChild(lyStop);

	auto spBackGround = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	spBackGround->setPosition(540, 360);
	spBackGround->setFlippedY(true);
	//spBackGround->setScale(0.5);
	lyStop->addChild(spBackGround, 0);
	auto spStop = Sprite::create("stopscene.png");
	spStop->setPosition(540, 360);
	lyStop->addChild(spStop, 1);
	auto btContinue = ui::Button::create("button2.png");
	//按钮继续的事件函数
	btContinue->setPosition(Vec2(376, 194));
	btContinue->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->popScene();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Resume();
#else
		experimental::AudioEngine::resumeAll(); 
#endif
	} });
	//
	lyStop->addChild(btContinue, 2);
	//放弃按钮的事件函数
	auto btReturn = ui::Button::create("button1.png");
	btReturn->setPosition(Vec2(704, 194));
	btReturn->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type){if (type == ui::Widget::TouchEventType::ENDED) {
		_eventDispatcher->removeEventListener(this->listener);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Stop();
#else
		experimental::AudioEngine::stopAll(); 
#endif
		Director::getInstance()->popScene(); 

		Director::getInstance()->popScene();

	} });
	//
	lyStop->addChild(btReturn, 2);
	Director::getInstance()->pushScene(scStop);

}