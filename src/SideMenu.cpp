#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/GJDropDownLayer.hpp>

using namespace geode::prelude;

class $modify(MySideMenu, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();

        auto layerSize = this->getContentSize();
        bool is4x3 = (layerSize.width / layerSize.height) < 1.5f;

        float posX = layerSize.width * (is4x3 ? 0.59f : 0.56f);  
        float posY = layerSize.height * 0.31f;

        auto myMenu = CCMenu::create();

        myMenu->setID("my-custom-side-menu"_spr);
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
            btnAch->setID("my-achievements-btn"_spr);
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
            btnOpt->setID("my-options-btn"_spr);
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
        auto scene = CCDirector::get()->getRunningScene();
        scene->addChild(layer, scene->getHighestChildZ() + 1);
        layer->showLayer(false);
    }

    void onOptions(CCObject*) {
        auto layer = OptionsLayer::create();
        auto scene = CCDirector::get()->getRunningScene();
        scene->addChild(layer, scene->getHighestChildZ() + 1);
        layer->showLayer(false);
    }
};