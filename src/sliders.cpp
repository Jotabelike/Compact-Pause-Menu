#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/Slider.hpp>  

using namespace geode::prelude;

struct MyCustomSliders : Modify<MyCustomSliders, PauseLayer> {
    struct Fields {
        CCLabelBMFont* m_musicPctLabel = nullptr;
        CCLabelBMFont* m_sfxPctLabel = nullptr;
    };

    void customSetup() {
        PauseLayer::customSetup();

        float slidersX = 10.f;  
        float musicaY = 130.f;
        float sfxY = 110.f;

        float labelsX = 95.f;
        float musicaLabelY = 204.f;
        float sfxLabelY = 184.f;

        float musicPctX = 185.f;
        float musicPctY = 195.f;

        float sfxPctX = 185.f;
        float sfxPctY = 175.f;

       
        auto musicSliderNode = this->getChildByID("music-slider");
        auto musicLabel = this->getChildByID("music-label");

        if (musicSliderNode && musicLabel) {
            musicSliderNode->setScale(0.6f);
            musicSliderNode->setPosition({ slidersX, musicaY });

            musicLabel->setScale(0.3f);
            musicLabel->ignoreAnchorPointForPosition(false);
            musicLabel->setAnchorPoint({ 0.5f, 0.5f });
            musicLabel->setPosition({ labelsX, musicaLabelY });

           
            float currentVol = 0.f;
            if (auto slider = static_cast<Slider*>(musicSliderNode)) {
                currentVol = slider->getValue();
            }
            else {
           
                currentVol = GameManager::sharedState()->m_bgVolume;
            }

            auto labelStr = fmt::format("{}%", static_cast<int>(currentVol * 100));

            m_fields->m_musicPctLabel = CCLabelBMFont::create(labelStr.c_str(), "goldFont.fnt");
            m_fields->m_musicPctLabel->setPosition({ musicPctX, musicPctY });
            m_fields->m_musicPctLabel->setScale(0.4f);
            m_fields->m_musicPctLabel->setAnchorPoint({ 0.0f, 0.5f });
            this->addChild(m_fields->m_musicPctLabel);
        }

        
        auto sfxSliderNode = this->getChildByID("sfx-slider");
        auto sfxLabel = this->getChildByID("sfx-label");

        if (sfxSliderNode && sfxLabel) {
            sfxSliderNode->setScale(0.6f);
            sfxSliderNode->setPosition({ slidersX, sfxY });

            sfxLabel->setScale(0.3f);
            sfxLabel->ignoreAnchorPointForPosition(false);
            sfxLabel->setAnchorPoint({ 0.5f, 0.5f });
            sfxLabel->setPosition({ labelsX, sfxLabelY });
 
            float currentVol = 0.f;
            if (auto slider = static_cast<Slider*>(sfxSliderNode)) {
                currentVol = slider->getValue();
            }
            else {
                currentVol = GameManager::sharedState()->m_sfxVolume;
            }

            auto labelStr = fmt::format("{}%", static_cast<int>(currentVol * 100));

            m_fields->m_sfxPctLabel = CCLabelBMFont::create(labelStr.c_str(), "goldFont.fnt");
            m_fields->m_sfxPctLabel->setPosition({ sfxPctX, sfxPctY });
            m_fields->m_sfxPctLabel->setScale(0.4f);
            m_fields->m_sfxPctLabel->setAnchorPoint({ 0.0f, 0.5f });
            this->addChild(m_fields->m_sfxPctLabel);
        }
    }

   
    void musicSliderChanged(CCObject* sender) {
        PauseLayer::musicSliderChanged(sender);

        auto slider = static_cast<Slider*>(this->getChildByID("music-slider"));

        if (m_fields->m_musicPctLabel && slider) {
            float val = slider->getValue();
            int percent = static_cast<int>(val * 100);
            m_fields->m_musicPctLabel->setString(fmt::format("{}%", percent).c_str());
        }
    }

    void sfxSliderChanged(CCObject* sender) {
        PauseLayer::sfxSliderChanged(sender);

        auto slider = static_cast<Slider*>(this->getChildByID("sfx-slider"));

        if (m_fields->m_sfxPctLabel && slider) {
            float val = slider->getValue();
            int percent = static_cast<int>(val * 100);
            m_fields->m_sfxPctLabel->setString(fmt::format("{}%", percent).c_str());
        }
    }
};