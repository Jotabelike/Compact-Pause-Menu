#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

struct MyCustomTitle : Modify<MyCustomTitle, PauseLayer> {

    std::string getDifficultyFrameName(GJGameLevel* level) {
        if (level->m_levelID > 0 && level->m_levelID < 128 && level->m_levelType != GJLevelType::Editor) {
            switch (level->m_levelID) {
            case 1: case 2: return "difficulty_01_btn_001.png";
            case 3: case 4: return "difficulty_02_btn_001.png";
            case 5: case 6: return "difficulty_03_btn_001.png";
            case 7: case 8: case 9: case 17: case 19: return "difficulty_04_btn_001.png";
            case 10: case 11: case 12: case 13: case 15: case 16: case 21: case 22: return "difficulty_05_btn_001.png";
            case 14: case 18: case 20: return "difficulty_06_btn_001.png";
            default: return "difficulty_04_btn_001.png";
            }
        }
        if (level->m_autoLevel) return "difficulty_auto_btn_001.png";
        if (level->m_demon > 0) {
            switch (level->m_demonDifficulty) {
            case 3: return "difficulty_07_btn_001.png";
            case 4: return "difficulty_08_btn_001.png";
            case 0: return "difficulty_06_btn_001.png";
            case 5: return "difficulty_09_btn_001.png";
            case 6: return "difficulty_10_btn_001.png";
            default: return "difficulty_06_btn_001.png";
            }
        }
        int difficulty = level->getAverageDifficulty();
        switch (difficulty) {
        case 0: return "difficulty_00_btn_001.png";
        case 1: return "difficulty_01_btn_001.png";
        case 2: return "difficulty_02_btn_001.png";
        case 3: return "difficulty_03_btn_001.png";
        case 4: return "difficulty_04_btn_001.png";
        case 5: return "difficulty_05_btn_001.png";
        default: return "difficulty_00_btn_001.png";
        }
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto layerSize = this->getContentSize();
        auto title = static_cast<CCLabelBMFont*>(this->getChildByID("level-name"));

        if (!title) return;
        auto playLayer = PlayLayer::get();
        if (!playLayer || !playLayer->m_level) return;
        auto level = playLayer->m_level;


        float faceX = layerSize.width * 0.08f;
        float faceY = layerSize.height - 37.f;

        float textX = layerSize.width * 0.127f;
        float titleY = layerSize.height - 27.0f;
        float creatorY = layerSize.height - 42.0f;

        auto diffSprite = GJDifficultySprite::create(0, GJDifficultyName::Short);
        std::string frameName = getDifficultyFrameName(level);

        if (auto frame = CCSpriteFrameCache::get()->spriteFrameByName(frameName.c_str())) {
            diffSprite->setDisplayFrame(frame);
        }

        int featureState = 0;
        if (level->m_featured > 0) featureState = 1;
        if (level->m_isEpic > 0) featureState = level->m_isEpic + 1;
        if (featureState > 0) diffSprite->updateFeatureState(static_cast<GJFeatureState>(featureState));

        diffSprite->setID("difficulty-face"_spr);
        diffSprite->setScale(0.8f);
        diffSprite->setAnchorPoint({ 0.5f, 0.5f });
        diffSprite->setPosition({ faceX, faceY });
        this->addChild(diffSprite);

        title->setScale(0.7f);
        title->setAnchorPoint({ 0.f, 0.5f });
        title->setAlignment(kCCTextAlignmentLeft);
        title->setPosition({ textX, titleY });

        std::string creatorName = level->m_creatorName;
        if (level->m_levelID < 128 && level->m_levelID > 0) creatorName = "RobTop";
        else if (creatorName.empty()) creatorName = "Unknown";

        auto creatorLabel = CCLabelBMFont::create(
            fmt::format("By {}", creatorName).c_str(),
            "goldFont.fnt"
        );
        creatorLabel->setID("creator-name-label"_spr);
        creatorLabel->setScale(0.4f);
        creatorLabel->setAnchorPoint({ 0.f, 0.5f });
        creatorLabel->setPosition({ textX, creatorY });

        this->addChild(creatorLabel);
    }
};