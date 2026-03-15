#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

struct MyCustomBar : Modify<MyCustomBar, PauseLayer> {

    void moverGrupo(const char* labelID, const char* percentID, const char* barID, float x, float y) {

        auto label = static_cast<CCLabelBMFont*>(this->getChildByID(labelID));
        auto percent = this->getChildByID(percentID);
        auto bar = static_cast<CCSprite*>(this->getChildByID(barID));

        if (!label || !percent || !bar) return;

        bar->setScaleX(0.35f);
        bar->setScaleY(0.5f);
        label->setScale(0.225f);
        percent->setScale(0.35f);

        bar->setPosition({ x, y });
        percent->setPosition({ x, y });

        float anchoBarra = bar->getContentSize().width * bar->getScaleX();
        float bordeIzquierdo = x - (anchoBarra / 2);

        label->setAnchorPoint({ 0.f, 0.5f });
        label->setPosition({ bordeIzquierdo, y + 10.f });
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();

        // Positions relative to winSize instead of hardcoded
        float posX = winSize.width * 0.23f;
        float alturaNormal = (winSize.height / 2) + 80.f;
        float distancia = 25.f;
        float alturaPractica = alturaNormal - distancia;

        moverGrupo("normal-mode-label",
            "normal-progress-label",
            "normal-progress-bar",
            posX,
            alturaNormal);
        moverGrupo("practice-mode-label",
            "practice-progress-label",
            "practice-progress-bar",
            posX,
            alturaPractica);

        auto playLayer = PlayLayer::get();
        if (playLayer && playLayer->m_level) {

            if (auto normalLabel = this->getChildByID("normal-mode-label")) {

                int attempts = playLayer->m_level->m_attempts;
                auto attemptsLabel = CCLabelBMFont::create(
                    fmt::format("Attempts: {}", attempts).c_str(),
                    "goldFont.fnt"
                );

                attemptsLabel->setID("attempts-label");
                attemptsLabel->setScale(0.3f);
                attemptsLabel->setAnchorPoint({ 0.f, 0.5f });

                float labelNormalWidth = normalLabel->getScaledContentSize().width;
                float startX = normalLabel->getPositionX();
                float startY = normalLabel->getPositionY();

                attemptsLabel->setPosition({ startX + labelNormalWidth + 3.f, startY });

                this->addChild(attemptsLabel);
            }
        }
    }
};