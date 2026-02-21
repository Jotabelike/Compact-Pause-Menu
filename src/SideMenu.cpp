#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/GJDropDownLayer.hpp> 

using namespace geode::prelude;

class $modify(MySideMenu, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();

      
        float posX = winSize.width - 250.f;
        float posY = winSize.height - 220.f;
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

     
        auto sprHome = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
        if (sprHome) {
            sprHome->setScale(0.73f);  
            auto btnHome = CCMenuItemSpriteExtra::create(
                sprHome,
                this,
                menu_selector(MySideMenu::onHome)
            );
            btnHome->setID("my-home-btn");
            myMenu->addChild(btnHome);
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

   
    void onHome(CCObject*) {
        geode::createQuickPopup(
            "Exit to Menu",           
            "Are you sure you want to <cr>exit</c> to the main menu?", 
            "Cancel", "Exit",          
            [](auto, bool btn2) {
                if (btn2) {
                    if (PlayLayer::get()) PlayLayer::get()->onQuit();
                    geode::Loader::get()->queueInMainThread([]() {
                        CCDirector::sharedDirector()->replaceScene(MenuLayer::scene(false));
                    });
                }
            }
        );
    }
};