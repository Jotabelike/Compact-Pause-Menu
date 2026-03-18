#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>

using namespace geode::prelude;

struct ParticleHandler : Modify<ParticleHandler, CCParticleSystemQuad> {
    void draw() {
        bool mostrarParticulas = Mod::get()->getSavedValue<bool>("show-particles", true);

        if (mostrarParticulas) {
            CCParticleSystemQuad::draw();
        }
    }
};

struct ShowParticlesOption : Modify<ShowParticlesOption, PauseLayer> {

    void onParticlesToggle(CCObject* sender) {
        bool estadoActual = Mod::get()->getSavedValue<bool>("show-particles", true);
        bool nuevoEstado = !estadoActual;
        Mod::get()->setSavedValue("show-particles", nuevoEstado);
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto menu = CCMenu::create();
        menu->setPosition({ winSize.width * 0.15f, winSize.height / 2 - 10.f });
        menu->setTouchPriority(-510);
        this->addChild(menu);

        bool estado = Mod::get()->getSavedValue<bool>("show-particles", true);

        auto toggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ShowParticlesOption::onParticlesToggle),
            0.50f
        );

        toggle->toggle(estado);
        toggle->setPosition({ 0.f, 0.f });
        menu->addChild(toggle);

        auto label = CCLabelBMFont::create("Show Particles", "bigFont.fnt");
        label->setScale(0.25f);
        label->setAnchorPoint({ 0.f, 0.5f });
        label->setPosition({ 15.f, 0.f });
        menu->addChild(label);
    }
};