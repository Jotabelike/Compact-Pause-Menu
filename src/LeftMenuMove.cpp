#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(PagedPauseMenu, PauseLayer) {
    // Definimos campos personalizados para guardar variables (Estado del menú)
    struct Fields {
        CCMenu* m_innerMenu = nullptr;       // El menú que contiene los botones
        CCMenuItemSpriteExtra* m_prevBtn = nullptr; // Flecha izquierda
        CCMenuItemSpriteExtra* m_nextBtn = nullptr; // Flecha derecha
        int m_pageIndex = 0;                 // Página actual (0, 1, 2...)
        const int ITEMS_PER_PAGE = 5;        // Cuantos botones mostrar por página
    };

    // Función para cambiar de página (flechas)
    void onPage(CCObject * sender) {
        if (sender->getTag() == -1) { // Izquierda
            if (m_fields->m_pageIndex > 0)
                m_fields->m_pageIndex--;
        }
        else { // Derecha
            m_fields->m_pageIndex++;
        }
        this->updatePageVisibility();
    }

    // Lógica principal: Decide qué botones mostrar y oculta/muestra flechas
    void updatePageVisibility() {
        auto menu = m_fields->m_innerMenu;
        if (!menu) return;

        auto children = menu->getChildren();
        if (!children) return;

        int totalItems = children->count();
        int maxPages = (totalItems + m_fields->ITEMS_PER_PAGE - 1) / m_fields->ITEMS_PER_PAGE;

        // 1. Gestionar visibilidad de las FLECHAS
        bool multiPage = totalItems > m_fields->ITEMS_PER_PAGE;

        if (m_fields->m_prevBtn) {
            // Visible si es multipagina Y no estamos en la primera página
            m_fields->m_prevBtn->setVisible(multiPage && m_fields->m_pageIndex > 0);
        }
        if (m_fields->m_nextBtn) {
            // Visible si es multipagina Y no estamos en la última página
            m_fields->m_nextBtn->setVisible(multiPage && m_fields->m_pageIndex < maxPages - 1);
        }

        // 2. Gestionar visibilidad de los BOTONES
        int startIndex = m_fields->m_pageIndex * m_fields->ITEMS_PER_PAGE;
        int endIndex = startIndex + m_fields->ITEMS_PER_PAGE;

        for (int i = 0; i < totalItems; i++) {
            auto node = static_cast<CCNode*>(children->objectAtIndex(i));
            if (node) {
                // Solo visible si está dentro del rango de la página actual
                bool shouldShow = (i >= startIndex && i < endIndex);
                node->setVisible(shouldShow);
            }
        }

        // 3. Forzar al layout a reacomodarse con los visibles
        menu->updateLayout();
    }

    // Función para robar los botones (como hicimos antes)
    void moverBotones() {
        auto oldMenu = this->getChildByID("left-button-menu");
        auto innerMenu = m_fields->m_innerMenu;

        if (oldMenu && innerMenu) {
            // Mudar todos los botones
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

            // Una vez mudados, calculamos la paginación
            this->updatePageVisibility();
        }
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::get()->getWinSize();

        // --- 1. Contenedor Principal (Flecha < Menú > Flecha) ---
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

        // --- 2. Crear Flecha Izquierda ---
        auto sprPrev = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        sprPrev->setScale(0.7f);
        auto btnPrev = CCMenuItemSpriteExtra::create(
            sprPrev, this, menu_selector(PagedPauseMenu::onPage)
        );
        btnPrev->setTag(-1); // -1 significa "Ir atrás"
        containerMenu->addChild(btnPrev);
        m_fields->m_prevBtn = btnPrev;

        // --- 3. Crear Menú Interno (Donde van los botones) ---
        auto innerMenu = CCMenu::create();
        innerMenu->setID("inner-button-menu");
        innerMenu->setContentSize({ 230.f, 60.f });
        innerMenu->ignoreAnchorPointForPosition(false);

        // Layout para los botones internos
        auto innerLayout = RowLayout::create();
        innerLayout->setGap(5.f);
        innerLayout->setAxisAlignment(AxisAlignment::Center);
        innerLayout->setCrossAxisAlignment(AxisAlignment::Center);
        innerMenu->setLayout(innerLayout);

        containerMenu->addChild(innerMenu);
        m_fields->m_innerMenu = innerMenu;

        // --- 4. Crear Flecha Derecha ---
        auto sprNext = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        sprNext->setScale(0.7f);
        sprNext->setFlipX(true); // Voltear flecha
        auto btnNext = CCMenuItemSpriteExtra::create(
            sprNext, this, menu_selector(PagedPauseMenu::onPage)
        );
        btnNext->setTag(1); // 1 significa "Ir adelante"
        containerMenu->addChild(btnNext);
        m_fields->m_nextBtn = btnNext;

        // Actualizamos el layout del contenedor principal para acomodar Flechas y Menú
        containerMenu->updateLayout();

        // --- 5. Programar la mudanza ---
        Loader::get()->queueInMainThread([this] {
            this->moverBotones();
            });
    }
};