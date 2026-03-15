#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyTimerMod, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();

        if (auto timeLabel = static_cast<CCLabelBMFont*>(this->getChildByID("play-time"))) {
            timeLabel->setPosition({
                winSize.width * 0.24f,
                winSize.height - 52.f
                });

            timeLabel->setAnchorPoint({ 0.5f, 0.5f });
            timeLabel->setScale(0.7f);
        }
    }
};