#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp> 
#include <Geode/binding/RewardsPage.hpp>

using namespace geode::prelude;

class $modify(DailyChest, PauseLayer) {
    struct Fields {
        RewardsPage* m_miniRewards = nullptr;
        bool m_rewardOpen = false;
    };

    void customSetup() {
        PauseLayer::customSetup();
        auto layerSize = this->getContentSize();
        bool is4x3 = (layerSize.width / layerSize.height) < 1.5f;
        float posX = (layerSize.width * (is4x3 ? 0.59f : 0.56f)) + 8.f;
        float posY = layerSize.height * 0.31f;
        float chestPosY = posY + 7.f;

        auto bgBox = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
        bgBox->setColor({ 0, 0, 0 });
        bgBox->setOpacity(110);
        bgBox->setContentSize({ 104.f, 38.f });
        bgBox->setPosition({ posX, chestPosY - 5.f });
        this->addChild(bgBox);

        auto rewardsPage = RewardsPage::create();
        rewardsPage->setID("mini-rewards-menu"_spr);
        rewardsPage->setTouchEnabled(false);
        rewardsPage->setKeyboardEnabled(false);
        rewardsPage->setKeypadEnabled(false);
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(rewardsPage);
        rewardsPage->setOpacity(0);

        if (auto mainLayer = static_cast<CCLayer*>(rewardsPage->getChildren()->objectAtIndex(0))) {
            for (int i = 0; i < mainLayer->getChildrenCount(); i++) {
                auto node = static_cast<CCNode*>(mainLayer->getChildren()->objectAtIndex(i));
                std::string nodeID = node->getID();

                if (nodeID == "main-menu") {
                    node->setVisible(true);
                    for (int j = 0; j < node->getChildrenCount(); j++) {
                        auto btn = static_cast<CCNode*>(node->getChildren()->objectAtIndex(j));
                        std::string btnID = btn->getID();
                        if (btnID == "close-button" || btnID == "info-button" || btnID == "free-button") {
                            btn->setVisible(false);
                        }
                    }
                }
                else if (nodeID == "left-label" || nodeID == "right-label") {
                    node->setVisible(true);
                }
                else {
                    node->setVisible(false);
                }
            }

            if (auto historyBtn = rewardsPage->getChildByIDRecursive("cvolton.betterinfo/history-button")) {
                historyBtn->setVisible(false);
            }
        }

        rewardsPage->ignoreAnchorPointForPosition(false);
        rewardsPage->setAnchorPoint({ 0.5f, 0.5f });
        rewardsPage->setPosition({ posX, chestPosY });
        rewardsPage->setScale(0.288f);
        CCDirector::sharedDirector()->getActionManager()->resumeTarget(rewardsPage);
        this->addChild(rewardsPage);
        m_fields->m_miniRewards = rewardsPage;
        this->schedule(schedule_selector(DailyChest::checkRewardPopup), 0.05f);
    }

    void checkRewardPopup(float dt) {
        if (!m_fields->m_miniRewards) return;
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;
        RewardUnlockLayer* rewardLayer = nullptr;
        std::function<void(CCNode*)> findRewardLayer = [&](CCNode* node) {
            if (rewardLayer || !node) return;

            if (auto r = dynamic_cast<RewardUnlockLayer*>(node)) {
                rewardLayer = r;
                return;
            }

            if (node->getChildren()) {
                for (int i = 0; i < node->getChildrenCount(); i++) {
                    findRewardLayer(static_cast<CCNode*>(node->getChildren()->objectAtIndex(i)));
                }
            }
            };


        findRewardLayer(m_fields->m_miniRewards);

        if (rewardLayer) {
            rewardLayer->retain();
            rewardLayer->removeFromParentAndCleanup(false);
            scene->addChild(rewardLayer, 100);
            rewardLayer->release();
            rewardLayer->setScale(1.0f);
            rewardLayer->setPosition({ 0.f, 0.f });
            m_fields->m_rewardOpen = true;
            m_fields->m_miniRewards->setVisible(false);
        }
        else {
            bool foundInScene = false;
            if (scene->getChildren()) {
                for (int i = 0; i < scene->getChildrenCount(); i++) {
                    auto child = static_cast<CCNode*>(scene->getChildren()->objectAtIndex(i));
                    if (dynamic_cast<RewardUnlockLayer*>(child)) {
                        foundInScene = true;
                        break;
                    }
                }
            }

            if (foundInScene && !m_fields->m_rewardOpen) {
                m_fields->m_rewardOpen = true;
                m_fields->m_miniRewards->setScale(0.288f);
                m_fields->m_miniRewards->setVisible(false);
            }
            else if (!foundInScene && m_fields->m_rewardOpen) {
                m_fields->m_rewardOpen = false;
                m_fields->m_miniRewards->setVisible(true);
            }
        }
    }
};