#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/GJDropDownLayer.hpp>

using namespace geode::prelude;

class $modify(MySideMenu, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();

        float posX = winSize.width * 0.56f;
        float posY = winSize.height * 0.31f;
        auto myMenu = CCMenu::create();
        myMenu->setID("my-custom-side-menu");
        myMenu->setPosition({ posX, posY });
        myMenu->setContentSize({ 260.f, 50.f });

        auto sprAch = CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png");
        if (sprAch) {
            sprAch->setScale(0.7f);
            auto btnAch = CCMenuItemSpriteExtra::create(
                sprAch,
                this,
                menu_selector(MySideMenu::onAchievements)
            );
            btnAch->setID("my-achievements-btn");
            myMenu->addChild(btnAch);
        }

        auto sprOpt = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        if (sprOpt) {
            sprOpt->setScale(0.7f);
            auto btnOpt = CCMenuItemSpriteExtra::create(
                sprOpt,
                this,
                menu_selector(MySideMenu::onOptions)
            );
            btnOpt->setID("my-options-btn");
            myMenu->addChild(btnOpt);
        }

        myMenu->setLayout(
            AxisLayout::create(Axis::Row)
            ->setGap(10.f)
            ->setAutoScale(false)
        );
        myMenu->updateLayout();

        this->addChild(myMenu);
    }

    void onAchievements(CCObject*) {
        auto layer = AchievementsLayer::create();
        CCDirector::get()->getRunningScene()->addChild(layer, 1000);
        layer->showLayer(false);
    }

    void onOptions(CCObject*) {
        auto layer = OptionsLayer::create();
        CCDirector::get()->getRunningScene()->addChild(layer, 1000);
        layer->showLayer(false);
    }
};