#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ChallengeNode.hpp>
#include <Geode/binding/CurrencyRewardLayer.hpp> 

using namespace geode::prelude;

class $modify(MyQuestPauseLayer, PauseLayer) {
    struct Fields {
        ChallengesPage* m_miniQuests = nullptr;
        bool m_rewardOpen = false;
    };

    void disableTouchRecursive(CCNode * node) {
        if (!node) return;

        if (typeinfo_cast<ChallengeNode*>(node)) {
            return;
        }

        if (auto layer = typeinfo_cast<CCLayer*>(node)) {
            layer->setTouchEnabled(false);
        }
        if (auto menu = typeinfo_cast<CCMenu*>(node)) {
            menu->setEnabled(false);
            menu->setTouchEnabled(false);
        }

        auto children = node->getChildren();
        if (!children) return;
        for (int i = 0; i < children->count(); i++) {
            auto child = static_cast<CCNode*>(children->objectAtIndex(i));
            disableTouchRecursive(child);
        }
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto layerSize = this->getContentSize();
        auto originalPage = ChallengesPage::create();
        originalPage->setID("mini-quest-menu"_spr);

        originalPage->setTouchEnabled(false);
        originalPage->setKeyboardEnabled(false);
        originalPage->setKeypadEnabled(false);
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(originalPage);
        originalPage->setOpacity(0);
        originalPage->updateTimers(0.0f);

        if (auto mainLayer = static_cast<CCLayer*>(originalPage->getChildren()->objectAtIndex(0))) {
            int challengeCount = 0;

            for (int i = 0; i < mainLayer->getChildrenCount(); i++) {
                auto node = static_cast<CCNode*>(mainLayer->getChildren()->objectAtIndex(i));

                if (auto challenge = typeinfo_cast<ChallengeNode*>(node)) {
                    if (challengeCount < 3) {
                        challenge->setVisible(true);
                        CCDirector::sharedDirector()->getActionManager()->resumeTarget(challenge);
                        challengeCount++;
                    }
                    else {
                        challenge->setVisible(false);
                    }
                }
                else if (typeinfo_cast<CCLabelBMFont*>(node)) {
                    node->setVisible(true);
                }
                else {
                    node->setVisible(false);
                }
            }
            disableTouchRecursive(mainLayer);
        }

        originalPage->ignoreAnchorPointForPosition(false);
        originalPage->setAnchorPoint({ 0.5f, 0.5f });
        float myScale = 0.6f;
        bool is4x3 = (layerSize.width / layerSize.height) < 1.5f;
        float posX = layerSize.width * (is4x3 ? 0.61f : 0.57f);
        CCPoint myPos = { posX, layerSize.height * 0.63f };
        originalPage->setScale(myScale);
        originalPage->setPosition(myPos);
        this->addChild(originalPage);
        m_fields->m_miniQuests = originalPage;
        this->schedule(schedule_selector(MyQuestPauseLayer::checkQuestRewardPopup), 0.05f);
    }

    void checkQuestRewardPopup(float dt) {
        if (!m_fields->m_miniQuests) return;
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;
        CurrencyRewardLayer* rewardLayer = nullptr;
        std::function<void(CCNode*)> findRewardLayer = [&](CCNode* node) {
            if (rewardLayer || !node) return;

            if (auto r = typeinfo_cast<CurrencyRewardLayer*>(node)) {
                rewardLayer = r;
                return;
            }

            if (node->getChildren()) {
                for (int i = 0; i < node->getChildrenCount(); i++) {
                    findRewardLayer(static_cast<CCNode*>(node->getChildren()->objectAtIndex(i)));
                }
            }
        };

        findRewardLayer(m_fields->m_miniQuests);

        if (rewardLayer) {
            int highestZ = 0;
            if (scene->getChildren()) {
                for (int i = 0; i < scene->getChildrenCount(); i++) {
                    auto child = static_cast<CCNode*>(scene->getChildren()->objectAtIndex(i));
                    if (child->getZOrder() > highestZ) {
                        highestZ = child->getZOrder();
                    }
                }
            }
   
            rewardLayer->retain();
            rewardLayer->removeFromParentAndCleanup(false);   
            scene->addChild(rewardLayer, highestZ + 1);
            rewardLayer->release();
            rewardLayer->setScale(1.0f);
            rewardLayer->setPosition({ 0.f, 0.f });
            m_fields->m_rewardOpen = true;
        }
        else {
            bool foundInScene = false;
            if (scene->getChildren()) {
                for (int i = 0; i < scene->getChildrenCount(); i++) {
                    auto child = static_cast<CCNode*>(scene->getChildren()->objectAtIndex(i));
                    if (typeinfo_cast<CurrencyRewardLayer*>(child)) {
                        foundInScene = true;
                        break;
                    }
                }
            }

            if (foundInScene && !m_fields->m_rewardOpen) {
                m_fields->m_rewardOpen = true;
            }
            else if (!foundInScene && m_fields->m_rewardOpen) {          
                m_fields->m_rewardOpen = false;              
            }
        }
    }
};

class $modify(MyQuestNode, ChallengeNode) {
    void onClaimReward(CCObject * sender) {

        auto parentLayer = this->getParent();
        CCNode* pageNode = parentLayer ? parentLayer->getParent() : nullptr;

        bool isMyMiniMenu = false;
        if (pageNode && pageNode->getScale() < 0.9f) {
            isMyMiniMenu = true;
        }

        if (isMyMiniMenu) {
            ChallengeNode::onClaimReward(sender);
        }
        else {
            ChallengeNode::onClaimReward(sender);
        }
    }
};