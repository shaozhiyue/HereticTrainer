#include "MainGame.h"
#include "SelectSong.h"
#include <algorithm>
#include <sstream>
#include <math.h>
#include "audio\include\AudioEngine.h"
#include "AndroidAudio.h"
USING_NS_CC;
void MainGame::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation(); //�õ���ǰ����������
		for (int i = 0; i < 9; i++)
		{
			if (checkTouch(i, location))
			{
				//��������λ���ж�������
				while (queueHead[i] < nodeQueue[i].size() && nodeQueue[i][queueHead[i]].result != Score::NONE) queueHead[i]++;
				if (queueHead[i] >= nodeQueue[i].size())
				{
					continue;
				}
				auto tmp = nodeQueue[i][queueHead[i]];
				auto sppos = tmp.head->getPosition();
				float dis = vGameArea[i].distance(sppos);
				if (dis>songconfig.baddis)
				{
					continue;
				}
				Score score = Score::NONE;
				if (dis < songconfig.perfectdis)
				{
					score = (Score::PREJECT);
				}
				else if (dis < songconfig.greatdis)
				{
					score = (Score::GREAT);
				}
				else if (dis < songconfig.gooddis)
				{
					score = (Score::GOOD);
				}
				else
				{
					score = (Score::BAD);
				}
				nodeQueue[i][queueHead[i]].result = score;
				queueHead[i]++;
				showPressEffect(i);
				if (tmp.type&RHYTHMTYPE_LONG)
				{
					log("press %d\n", i);
					//�����α���ס��ֹͣ���������õ�ͷ��
					table.insert(std::make_pair(touch, i));
					tmp.head->stopAllActions();
					tmp.head->setPosition(vGameArea[i]);
					tmp.head->setScale(1);
					tmp.noodle->runAction(RepeatForever::create
						(Sequence::create(EaseIn::create(FadeOut::create(1), 2), EaseIn::create(FadeIn::create(1), 2), NULL)));
				}
				else tmp.head->setVisible(false);
				showScoreEffect(score);
			}
		}
	}
}

void MainGame::update(float dt)
{
	curTime += dt;
	if ((song.dDuration <= curTime) && curRhythm >=song.lstRhythm.size())//�����Ƿ��Ѿ��������ʱ����
	{

		this->unscheduleUpdate();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Stop();
#else
		experimental::AudioEngine::stopAll(); 
#endif
		ResultScene();
		return;
	}
	while (curRhythm <song.lstRhythm.size() && (song.lstRhythm[curRhythm].beginTime - songconfig.rate) <= curTime)
	{
		born(song.lstRhythm[curRhythm]);
		curRhythm++;
	}
}
void MainGame::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{

}
void MainGame::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	for (auto touch : touches)
	{
		auto iter = table.find(touch);
		if (iter != table.end())
		{
			Score score;
			auto tmp = (nodeQueue[iter->second])[queueHead[iter->second]-1];
			score = tmp.result_tail;
			if (score == Score::NONE)
			{
				auto sppos = tmp.tail->getPosition();
				float dis = vGameArea[iter->second].distance(sppos);
				if (dis<songconfig.perfectdis)
				{
					score = (Score::PREJECT);
				}
				else if (dis < songconfig.greatdis)
				{
					score = (Score::GREAT);
				}
				else if (dis <songconfig.gooddis)
				{
					score = (Score::GOOD);
				}
				else if (dis<songconfig.baddis)
				{
					score = (Score::BAD);
				}
				else score = Score::MISS;
				nodeQueue[iter->second][queueHead[iter->second] - 1].result_tail = score;
			}
			if (nodeQueue[iter->second][queueHead[iter->second] - 1].result_tail != Score::NONE)
			{
				tmp.head->setVisible(false);
				tmp.tail->setVisible(false);
				tmp.noodle->setVisible(false);
				showPressEffect(iter->second);
				showScoreEffect(nodeQueue[iter->second][queueHead[iter->second] - 1].result_tail);
			}
			table.erase(iter);
		}
	}
}

void MainGame::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
	onTouchesEnded(touches, event);
}

void MainGame::born(const Rhythm &rh)
{
	NodeInfo nodeinfo;
	nodeinfo.type = rh.type;
	if (rh.type&RHYTHMTYPE_LONG)//����Բ���������ɣ�
	{
		Deal_with_long(nodeinfo, rh);
	}
	else
	{
		Deal_with_tap(nodeinfo,rh);
	}
	//	log("Anzer  born-%d end", debug_cnt++);
}
bool MainGame::checkTouch(int pos, const Vec2 &touchLocation)//�����ȡ���ж���������Բ��+��������
{
	float dis = vGameArea[pos].distance(vBornPoint);	//ͷ�񵽳��������
	float rectx = vGameArea[pos].x - this->songconfig.touchwidth;
	float recty = vGameArea[pos].y - this->songconfig.touchheight;	//λ�ù̶���ͷ�����Ͻ�
	float rectwidth = this->songconfig.touchwidth * 2;
	float rectheight = this->songconfig.touchheight * 2;			//������סͷ��
	Rect checkRect = Rect(rectx, recty, rectwidth , rectheight);
	auto point=touchLocation.rotateByAngle(vGameArea[pos], atan((vBornPoint.x - vGameArea[pos].x) / (vBornPoint.y - vGameArea[pos].y)));	 //�������ͷ������ߣ���ˮƽ�߼н�
	return (checkRect.containsPoint(point) && touchLocation.distance(vGameArea[pos]) < this->songconfig.touchdis);

}
