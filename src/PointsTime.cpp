#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(PointsLabelFix, PauseLayer) {
    void customSetup() {     
        PauseLayer::customSetup();
        auto layerSize = this->getContentSize();   
        if (auto pointsLabel = static_cast<CCLabelBMFont*>(this->getChildByID("points-label"))) {     
            float posX = layerSize.width * 0.23f;         
            float posY = (layerSize.height / 2) + 70.f;
            pointsLabel->setPosition({ posX, posY });
            pointsLabel->setAnchorPoint({ 0.5f, 0.5f });       
            pointsLabel->setScale(0.55f);
        }
    }
};