#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

std::string formatStatNumber(int number) {
    if (number >= 1000000) return fmt::format("{:.1f}M", number / 1000000.0);
    else if (number >= 10000) return fmt::format("{:.0f}k", number / 1000.0);
    else if (number >= 1000) return fmt::format("{:.1f}k", number / 1000.0);
    return std::to_string(number);
}

std::string getLengthText(int lengthEnum) {
    switch (lengthEnum) {
    case 0: return "Tiny";
    case 1: return "Short";
    case 2: return "Medium";
    case 3: return "Long";
    case 4: return "XL";
    case 5: return "Plat";
    default: return "NA";
    }
}

int getMainLevelLength(int levelID) {
    switch (levelID) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 15:
    case 16:
    case 17:
    case 19:
    case 21:
    case 22:
        return 3;
    case 14:
    case 18:
    case 20:
        return 4;
    default:
        return -1;
    }
}

class $modify(MyLevelStats, PauseLayer) {

    void addStatToContainer(CCNode * container,
        std::string iconFrame,
        std::string text,
        float xPos, float iconScale = 0.6f)
    {
        auto sprite = CCSprite::createWithSpriteFrameName(iconFrame.c_str());
        sprite->setScale(iconScale);
        sprite->setPosition({ xPos, 0.0f });
        container->addChild(sprite);

        auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        label->setScale(0.35f);
        label->setAnchorPoint({ 0.0f, 0.5f });
        label->setPosition({ xPos + 10.0f, 0.0f });
        container->addChild(label);
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto level = PlayLayer::get()->m_level;
        if (!level) return;

        auto layerSize = this->getContentSize();
        float generalScale = 0.70f;
        float itemSpacing = 53.0f;

        float totalSpan = itemSpacing * 3.0f + 30.0f;
        float positionX = (layerSize.width / 2) - (totalSpan * generalScale / 2);
        float positionY = layerSize.height * 0.05f;

        auto statsContainer = CCNode::create();
        statsContainer->setPosition({ positionX, positionY });
        statsContainer->setScale(generalScale);
        statsContainer->setID("stats-container"_spr);
        this->addChild(statsContainer);

        bool isPlatformer = level->isPlatformer();
        std::string starIcon = isPlatformer ? "GJ_moonsIcon_001.png" : "GJ_starsIcon_001.png";

        addStatToContainer(
            statsContainer,
            starIcon,
            std::to_string(level->m_stars),
            0.0f
        );

        addStatToContainer(
            statsContainer,
            "GJ_downloadsIcon_001.png",
            formatStatNumber(level->m_downloads),
            itemSpacing * 1.0f
        );

        addStatToContainer(
            statsContainer,
            "GJ_likesIcon_001.png",
            formatStatNumber(level->m_likes),
            itemSpacing * 2.0f
        );


        int levelID = level->m_levelID.value();
        bool isMainLevel = (level->m_levelType == GJLevelType::Main)
            || (levelID > 0 && levelID < 128 && level->m_levelType != GJLevelType::Editor);

        int lengthValue = level->m_levelLength;
        if (isMainLevel) {
            int mainLength = getMainLevelLength(levelID);
            if (mainLength >= 0) {
                lengthValue = mainLength;
            }
        }

        addStatToContainer(
            statsContainer,
            "GJ_timeIcon_001.png",
            getLengthText(lengthValue),
            itemSpacing * 3.0f,
            0.5f
        );
    }
};