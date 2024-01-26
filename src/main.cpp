#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

CCSprite* jumpscare_sprite = NULL;
CCSprite* background = NULL;

class $modify(PlayerObject) {
	TodoReturn playerDestroyed(bool p0) {
    	PlayerObject::playerDestroyed(p0);

		// probability check
		auto chance = Mod::get()->getSettingValue<double>("chance");
		if (rand()/(RAND_MAX+1.0) > chance/100) return;

		// thanks nicknamegg
		const auto runningScene = CCDirector::get()->getRunningScene();
		auto winSize = CCDirector::get()->getWinSize();

		if (!runningScene->getChildByID("jumpscare")) {
			jumpscare_sprite = CCSprite::create("jumpscare.png"_spr);
			jumpscare_sprite->setID("jumpscare");

			jumpscare_sprite->setPosition({winSize.width / 2, winSize.height / 2});
			runningScene->addChild(jumpscare_sprite, 9);
		}

		if (!runningScene->getChildByID("jumpscare-background")) {
			background = CCSprite::create("background.png"_spr);
			background->setID("jumpscare-background");
			// scale to screen
			background->setScaleX(winSize.width / background->getContentSize().width);
			background->setScaleY(winSize.height / background->getContentSize().height);

			background->setPosition({winSize.width / 2, winSize.height / 2});
			runningScene->addChild(background, 8);
		}

		//scale is to make it fit to screen but
		//i dont even fucking know anymore c++ is so fucking stupid
		auto yscale = winSize.height / jumpscare_sprite->getContentSize().height;
		auto xscale = winSize.width / jumpscare_sprite->getContentSize().width;
		float scale;
		if (xscale < yscale) scale = xscale;
		else scale = yscale;

		background->setOpacity(255);
		jumpscare_sprite->setScale(scale);

		jumpscare_sprite->setOpacity(255);
		jumpscare_sprite->setScale(1);
		jumpscare_sprite->runAction(CCScaleBy::create(0.2, scale))->setTag(1);	
		jumpscare_sprite->runAction(CCBlink::create(0.5, 10))->setTag(2);

		// does not fucking work
		FMODAudioEngine::sharedEngine()->playEffect("jumpscareAudio.ogg"_spr);
	}
};

class $modify(PlayLayer) {
	TodoReturn startMusic() {
		PlayLayer::startMusic();
		const auto runningScene = CCDirector::get()->getRunningScene();
		// only set opacity if the sprite is already in the scene
		if (runningScene->getChildByID("jumpscare")) {
			jumpscare_sprite->setOpacity(0);
			background->setOpacity(0);	
		}
	}
};

// class $modify(PauseLayer) {
// 	bool init(bool p0) {
// 		if (!PauseLayer::init(p0)) return false;

		

// 		return true;
// 	}

// };



// int range = max - min + 1;
// int num = rand() % range + min;
// rand with range
