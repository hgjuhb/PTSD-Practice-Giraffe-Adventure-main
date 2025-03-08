#include "AppUtil.hpp"
#include "App.hpp"
#include "../PTSD/lib/sdl2_mixer/src/codecs/stb_vorbis/stb_vorbis.h"

#include "Util/Logger.hpp"

/**
 * @brief The function to validate the tasks.
 * @warning Do not modify this function.
 * @note See README.md for the task details.
 */
void App::ValidTask() {
    switch (m_Phase) {
        case Phase::LOBBY:
            Reset();
            SetLevel(1);
            m_PRM->NextPhase();
            m_Phase = Phase::FIRST_LEVEL;
            break;

        case Phase::FIRST_LEVEL:
            Reset();
            SetLevel(2);
            m_PRM->NextPhase();
            m_Phase = Phase::SECOND_LEVEL;
            break;

        case Phase::SECOND_LEVEL:
            m_PRM->NextPhase();
        m_Phase = Phase::SECOND_LEVEL;
        break;
    }
}

void App::Reset() {
    Level_Coordinates = {};
    Level_Balloons = {};
    if (!m_Counters.empty()) {
        m_Root.RemoveChild(m_Counters[0]);
        m_Root.RemoveChild(m_Counters[1]);
        m_Root.RemoveChild(m_Counters[2]);
        m_Root.RemoveChild(m_Counters[0]->GetCounterText());
        m_Root.RemoveChild(m_Counters[1]->GetCounterText());
        m_Root.RemoveChild(m_Counters[2]->GetCounterText());
        m_Counters = {};
    }
    if (!m_Monkeys.empty()) {
        for (auto& monkeyPtr : m_Monkeys) {
            m_Root.RemoveChild(monkeyPtr);
            m_Root.RemoveChild(monkeyPtr -> GetRange());
        }
        m_Monkeys = {};
    }
}

void App::SetLevel(int level) {
    Level_Coordinates = m_PRM -> GetCoordinates(level-1);
    Level_Balloons = m_PRM -> GetBalloons(level-1);

    m_Counters.push_back(std::make_shared<Heart>(20, 20));
    m_Root.AddChild(m_Counters[0]);
    m_Root.AddChild(m_Counters[0]->GetCounterText());

    m_Counters.push_back(std::make_shared<Money>(0,99999));
    m_Root.AddChild(m_Counters[1]);
    m_Root.AddChild(m_Counters[1]->GetCounterText());

    m_Counters.push_back(std::make_shared<Round>(1,10));
    m_Root.AddChild(m_Counters[2]);
    m_Root.AddChild(m_Counters[2]->GetCounterText());

    // 设置按钮位置的基础坐标和间隔
    float startX = 0.0f;
    float startY = 360.0f;
    float buttonSpacing = 100.0f;  // 按钮之间的间隔
    
    // 创建飞镖猴子按钮
    auto m_DartMonkeyButton = std::make_shared<DartMonkeyButton>(glm::vec2(startX, startY));
    m_DragButtons.push_back(m_DartMonkeyButton);    
    m_Root.AddChild(m_DartMonkeyButton);
    
    // 创建钉子猴子按钮
    auto m_NailMonkeyButton = std::make_shared<NailMonkeyButton>(glm::vec2(startX, startY-buttonSpacing));
    m_DragButtons.push_back(m_NailMonkeyButton);    
    m_Root.AddChild(m_NailMonkeyButton);
    
    // 创建狙击手猴子按钮
    auto m_SniperMonkeyButton = std::make_shared<SniperMonkeyButton>(glm::vec2(startX, startY-buttonSpacing*2));
    m_DragButtons.push_back(m_SniperMonkeyButton);    
    m_Root.AddChild(m_SniperMonkeyButton);
    
    // 创建回旋镖猴子按钮
    auto m_BoomerangMonkeyButton = std::make_shared<BoomerangMonkeyButton>(glm::vec2(startX, startY-buttonSpacing*3));
    m_DragButtons.push_back(m_BoomerangMonkeyButton);    
    m_Root.AddChild(m_BoomerangMonkeyButton);
    
    // 创建忍者猴子按钮
    auto m_NinjaMonkeyButton = std::make_shared<NinjaMonkeyButton>(glm::vec2(startX, startY-buttonSpacing*4));
    m_DragButtons.push_back(m_NinjaMonkeyButton);    
    m_Root.AddChild(m_NinjaMonkeyButton);

    auto m_CannonButton = std::make_shared<CannonButton>(glm::vec2(startX, startY-buttonSpacing*5));
    m_DragButtons.push_back(m_CannonButton);    
    m_Root.AddChild(m_CannonButton);
}
