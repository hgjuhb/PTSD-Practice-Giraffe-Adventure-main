#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "PhaseResourceManger.hpp"
#include "Balloon.hpp"
#include "Attack.hpp"
#include "Counter.hpp"
#include "Monkey.hpp"
#include "DragButton.hpp"
#include "Range.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();
    void Reset();
    void SetLevel(int level);

private:
    enum class Phase {
        LOBBY,
        FIRST_LEVEL,
        SECOND_LEVEL,
    };


    State m_CurrentState = State::START;
    Phase m_Phase = Phase::LOBBY;
    std::vector<glm::vec2> Level_Coordinates;
    std::vector<std::vector<int>> Level_Balloons;
    Util::Renderer m_Root;
    std::vector<std::shared_ptr<Counter>> m_Counters;
    std::vector<std::shared_ptr<Balloon>> m_Balloons;
    std::vector<std::shared_ptr<Attack>> m_Attacks;
    std::vector<std::shared_ptr<Monkey>> m_Monkeys;
    std::shared_ptr<PhaseResourceManger> m_PRM;
    std::vector<std::shared_ptr<DragButton>> m_DragButtons;
    std::shared_ptr<Monkey> m_DragMonkey;
    std::shared_ptr<Monkey> m_testMonkey;

    bool m_EnterDown = false;
};

#endif
