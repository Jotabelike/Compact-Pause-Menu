#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ChallengeNode.hpp>

using namespace geode::prelude;
class $modify(MyQuestPauseLayer, PauseLayer) {


    void disableTouchRecursive(CCNode * node) {
        if (!node) return;

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

        if (auto mainLayer = static_cast<CCLayer*>(originalPage->getChildren()->objectAtIndex(0))) {
            for (int i = 0; i < mainLayer->getChildrenCount(); i++) {
                auto node = static_cast<CCNode*>(mainLayer->getChildren()->objectAtIndex(i));
                if (typeinfo_cast<ChallengeNode*>(node) || typeinfo_cast<CCLabelBMFont*>(node)) {
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
            float savedScale = pageNode->getScale();
            CCPoint savedPos = pageNode->getPosition();

            pageNode->setScale(1.0f);
            pageNode->setPosition({ 0.f, 0.f });

            auto engine = FMODAudioEngine::sharedEngine();
            float savedVolume = engine->getEffectsVolume();
            engine->setEffectsVolume(0.0f);
            ChallengeNode::onClaimReward(sender);
            engine->setEffectsVolume(savedVolume);
            pageNode->setScale(savedScale);
            pageNode->setPosition(savedPos);
        }
        else {
            ChallengeNode::onClaimReward(sender);
        }
    }
};