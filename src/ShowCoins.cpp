#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyCoins, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto layerSize = this->getContentSize();
        auto playLayer = PlayLayer::get();

        if (!playLayer || !playLayer->m_level) return;
        auto level = playLayer->m_level;

        auto coinMenu = CCMenu::create();
        coinMenu->setID("coins-menu"_spr);

        coinMenu->setPosition({ layerSize.width * 0.12f, layerSize.height - 63.f });
        coinMenu->setAnchorPoint({ 0.f, 0.2f });

        coinMenu->setLayout(
            RowLayout::create()
            ->setGap(0.2f)
            ->setAxisAlignment(AxisAlignment::Start)
        );
        coinMenu->setContentSize({ 90.f, 20.f });

        int levelIDInt = level->m_levelID.value();
        std::string levelIDStr = std::to_string(levelIDInt);
        bool isMainLevel = (level->m_levelType == GJLevelType::Main) || (levelIDInt < 128 && levelIDInt > 0 && level->m_levelType != GJLevelType::Editor);
        std::string slotFrameName = isMainLevel ? "secretCoin_b_01_001.png" : "secretCoin_2_b_01_001.png";
        std::string coinFrameName = isMainLevel ? "secretCoinUI_001.png" : "secretCoinUI2_001.png";

        for (int i = 0; i < 3; i++) {
            if (level->m_coins <= i) break;
            auto coinSlot = CCSprite::createWithSpriteFrameName(slotFrameName.c_str());
            coinSlot->setScale(0.3f);

            auto coinSprite = CCSprite::createWithSpriteFrameName(coinFrameName.c_str());
            coinSprite->setScale(0.3f);

            if (!isMainLevel && !level->m_coinsVerified.value()) {
                coinSlot->setColor({ 255, 175, 75 });
                coinSprite->setColor({ 255, 175, 75 });
            }

            bool hasCollectedSaved = false;
            bool hasCollectedNow = false;

            if (isMainLevel) {
                auto key = fmt::format("{}_{}", levelIDInt, i + 1);
                hasCollectedSaved = GameStatsManager::sharedState()->hasSecretCoin(key.c_str());
            }
            else {
                auto key = fmt::format("{}_{}", levelIDInt, i + 1);
                hasCollectedSaved = GameStatsManager::sharedState()->hasUserCoin(key.c_str());
            }

            CCNode* container = CCNode::create();
            container->setContentSize(coinSlot->getContentSize() * 0.6f);
            coinSlot->setPosition(container->getContentSize() / 2);
            coinSprite->setPosition(container->getContentSize() / 2);
            container->addChild(coinSlot);

            if (hasCollectedSaved) {
                container->addChild(coinSprite);
            }
            else {
                if (playLayer->m_collectedItems->objectForKey(std::to_string(i + 1))) {
                    hasCollectedNow = true;
                }

                if (hasCollectedNow) {
                    container->addChild(coinSprite);
                    auto fadeOut = CCFadeOut::create(0.8f);
                    auto fadeIn = CCFadeIn::create(0.8f);
                    auto seq = CCSequence::create(fadeOut, fadeIn, nullptr);
                    coinSprite->runAction(CCRepeatForever::create(seq));
                }
            }

            coinMenu->addChild(container);
        }

        coinMenu->updateLayout();
        this->addChild(coinMenu);
    }
};