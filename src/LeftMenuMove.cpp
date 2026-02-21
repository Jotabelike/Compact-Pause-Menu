#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(PagedPauseMenu, PauseLayer) {
    
    struct Fields {
        CCMenu* m_innerMenu = nullptr;       
        CCMenuItemSpriteExtra* m_prevBtn = nullptr;  
        CCMenuItemSpriteExtra* m_nextBtn = nullptr;  
        int m_pageIndex = 0;                 
        const int ITEMS_PER_PAGE = 5;       
    };

  
    void onPage(CCObject * sender) {
        if (sender->getTag() == -1) {  
            if (m_fields->m_pageIndex > 0)
                m_fields->m_pageIndex--;
        }
        else {  
            m_fields->m_pageIndex++;
        }
        this->updatePageVisibility();
    }

   
    void updatePageVisibility() {
        auto menu = m_fields->m_innerMenu;
        if (!menu) return;

        auto children = menu->getChildren();
        if (!children) return;

        int totalItems = children->count();
        int maxPages = (totalItems + m_fields->ITEMS_PER_PAGE - 1) / m_fields->ITEMS_PER_PAGE;
        bool multiPage = totalItems > m_fields->ITEMS_PER_PAGE;

        if (m_fields->m_prevBtn) {
            m_fields->m_prevBtn->setVisible(multiPage && m_fields->m_pageIndex > 0);
        }
        if (m_fields->m_nextBtn) {
       
            m_fields->m_nextBtn->setVisible(multiPage && m_fields->m_pageIndex < maxPages - 1);
        }

     
        int startIndex = m_fields->m_pageIndex * m_fields->ITEMS_PER_PAGE;
        int endIndex = startIndex + m_fields->ITEMS_PER_PAGE;

        for (int i = 0; i < totalItems; i++) {
            auto node = static_cast<CCNode*>(children->objectAtIndex(i));
            if (node) {
              
                bool shouldShow = (i >= startIndex && i < endIndex);
                node->setVisible(shouldShow);
            }
        }
 
        menu->updateLayout();
    }

   
    void moverBotones() {
        auto oldMenu = this->getChildByID("left-button-menu");
        auto innerMenu = m_fields->m_innerMenu;

        if (oldMenu && innerMenu) {
            while (oldMenu->getChildrenCount() > 0) {
                auto node = static_cast<CCNode*>(oldMenu->getChildren()->objectAtIndex(0));
                if (node) {
                    node->retain();
                    node->removeFromParent();
                    innerMenu->addChild(node);
                    node->release();
                }
                else {
                    break;
                }
            }

            oldMenu->setVisible(false);

       
            this->updatePageVisibility();
        }
    }

    void customSetup() {
        PauseLayer::customSetup();
        auto winSize = CCDirector::get()->getWinSize();

     
        auto containerMenu = CCMenu::create();
        containerMenu->setID("bottom-menu-container");
        containerMenu->setPosition({ winSize.width / 2, 45.f });
        containerMenu->setContentSize({ 500.f, 60.f });
        containerMenu->ignoreAnchorPointForPosition(false);
        containerMenu->setLayout(
            RowLayout::create()
            ->setGap(10.f)
            ->setAxisAlignment(AxisAlignment::Center)
        );
        this->addChild(containerMenu);

       
        auto sprPrev = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        sprPrev->setScale(0.7f);
        auto btnPrev = CCMenuItemSpriteExtra::create(
            sprPrev, this, menu_selector(PagedPauseMenu::onPage)
        );
        btnPrev->setTag(-1);  
        containerMenu->addChild(btnPrev);
        m_fields->m_prevBtn = btnPrev;

     
        auto innerMenu = CCMenu::create();
        innerMenu->setID("inner-button-menu");
        innerMenu->setContentSize({ 230.f, 60.f });
        innerMenu->ignoreAnchorPointForPosition(false);
 
        auto innerLayout = RowLayout::create();
        innerLayout->setGap(5.f);
        innerLayout->ignoreInvisibleChildren(true);
        innerLayout->setAxisAlignment(AxisAlignment::Center);
        innerLayout->setCrossAxisAlignment(AxisAlignment::Center);
        innerMenu->setLayout(innerLayout);
        containerMenu->addChild(innerMenu);
        m_fields->m_innerMenu = innerMenu;

    
        auto sprNext = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        sprNext->setScale(0.7f);
        sprNext->setFlipX(true); 
        auto btnNext = CCMenuItemSpriteExtra::create(
            sprNext, this, menu_selector(PagedPauseMenu::onPage)
        );
        btnNext->setTag(1);  
        containerMenu->addChild(btnNext);
        m_fields->m_nextBtn = btnNext;
        containerMenu->updateLayout();

       
        Loader::get()->queueInMainThread([this] {
            this->moverBotones();
            });
    }
};