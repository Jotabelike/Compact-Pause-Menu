#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(timmer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();

        if (auto timeLabel = static_cast<CCLabelBMFont*>(this->getChildByID("play-time"))) {
            float posX = winSize.width * 0.23f;
            float posY = (winSize.height / 2) + 80.f;

            timeLabel->setPosition({ posX, posY });
            timeLabel->setAnchorPoint({ 0.5f, 0.5f });
            timeLabel->setScale(0.7f);
        }
    }
};