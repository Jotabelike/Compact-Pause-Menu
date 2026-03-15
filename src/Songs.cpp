#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/binding/CustomSongWidget.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;
const int MY_WIDGET_TAG = 7777;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();
        auto level = PlayLayer::get()->m_level;

        SongInfoObject* songInfo = nullptr;
        if (level->m_songID == 0) {
            songInfo = SongInfoObject::create(level->m_audioTrack);
        }
        else {
            songInfo = SongInfoObject::create(level->m_songID);
        }

        if (songInfo) {
            songInfo->retain();
        }

        bool isRobtop = (level->m_songID == 0);

        auto songWidget = CustomSongWidget::create(
            songInfo, nullptr, true, true, true, isRobtop, false, false, 0
        );

        if (songWidget) {
            // Position relative to winSize - left-center area
            songWidget->setPosition({ winSize.width * 0.255f, winSize.height * 0.328f });
            songWidget->setScale(0.47f);

            songWidget->setTag(MY_WIDGET_TAG);
            songWidget->setID("custom-songs-widget");

            this->addChild(songWidget);

            if (auto menu = songWidget->getChildByID("buttons-menu")) {
                if (auto oldBtn = menu->getChildByID("more-button")) {
                    oldBtn->setVisible(false);
                    oldBtn->setPosition({ -99999.0f, -99999.0f });
                }
            }

            auto btnSprite = ButtonSprite::create("More", 40, true, "bigFont.fnt", "GJ_button_02.png", 30.0f, 0.7f);

            auto myNewBtn = CCMenuItemSpriteExtra::create(
                btnSprite,
                songWidget,
                menu_selector(CustomSongWidget::onMore)
            );

            auto myMenu = CCMenu::create();
            myMenu->addChild(myNewBtn);
            myMenu->setPosition({ 122.0f, 58.0f });

            songWidget->addChild(myMenu);
        }
    }
};

class $modify(FixedSongWidget, CustomSongWidget) {

    void deleteSong() {
        CustomSongWidget::deleteSong();

        if (auto engine = FMODAudioEngine::sharedEngine()) {
            engine->stopAllMusic(true);
        }

        this->updateSongInfo();
    }

    void onExit() {
        if (m_songInfoObject) {
            m_songInfoObject->release();
            m_songInfoObject = nullptr;
        }
        CustomSongWidget::onExit();
    }

    void updateSongInfo() {
        CustomSongWidget::updateSongInfo();

        if (this->getTag() == MY_WIDGET_TAG) {
            if (auto menu = this->getChildByID("buttons-menu")) {
                if (auto oldBtn = menu->getChildByID("more-button")) {
                    oldBtn->setVisible(false);
                    oldBtn->setPosition({ -99999.0f, -99999.0f });
                }
            }
        }
    }
};