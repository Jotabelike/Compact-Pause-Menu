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
    default: return "NA";
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

        auto winSize = CCDirector::get()->getWinSize();
        float generalScale = 0.70f;
        float itemSpacing = 53.0f;

       
        float totalSpan = itemSpacing * 3.0f + 30.0f;   
        float positionX = (winSize.width / 2) - (totalSpan * generalScale / 2);
        float positionY = winSize.height * 0.05f;

        auto statsContainer = CCNode::create();
        statsContainer->setPosition({ positionX, positionY });
        statsContainer->setScale(generalScale);
        statsContainer->setID("stats-container");
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

        addStatToContainer(
            statsContainer,
            "GJ_timeIcon_001.png",
            getLengthText(level->m_levelLength),
            itemSpacing * 3.0f,
            0.5f
        );
    }
};