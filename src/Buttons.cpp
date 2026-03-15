#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

struct MyCustomPause : Modify<MyCustomPause, PauseLayer> {
    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();
        auto oldMenu = this->getChildByID("center-button-menu");

        auto newMenu = CCMenu::create();
        newMenu->setID("my-compact-menu");
        newMenu->setPosition({ winSize.width * 0.08f, winSize.height / 2 - 10.f });
        newMenu->setContentSize({ 60.f, 500.f });
        newMenu->ignoreAnchorPointForPosition(false);
        newMenu->setScale(0.5f);

        std::vector<std::string> buttonsToMove = {
            "exit-button",
            "options-button",
            "edit-button",
            "practice-button",
            "full-restart-button",
            "retry-button",
            "play-button"
        };

        if (oldMenu) {
            for (auto const& id : buttonsToMove) {
                if (auto btn = oldMenu->getChildByID(id)) {
                    btn->retain();
                    btn->removeFromParent();
                    newMenu->addChild(btn);
                    btn->release();

                    btn->setZOrder(0);
                }
            }
            oldMenu->setVisible(false);
        }

        auto layout = AxisLayout::create(Axis::Column);
        layout->setGap(10.f);
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);
        layout->setAutoScale(false);
        newMenu->setLayout(layout);
        newMenu->updateLayout();
        this->addChild(newMenu);
    }
};