#include "App.hpp"
#include <iostream>
#include <algorithm>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <random>

int count = 20;

std::vector<std::shared_ptr<Balloon>> new_balloons;
std::vector<std::shared_ptr<Balloon>> remove_balloons;
std::vector<std::shared_ptr<Attack>> remove_attacks;
std::shared_ptr<Monkey> m_testMonkey;
std::shared_ptr<Balloon> factory(int num, std::vector<glm::vec2> coordinates) {
    switch (num) {
        case 0:
            return std::make_shared<RED>(coordinates);
        case 1:
            return std::make_shared<BLUE>(coordinates);
        case 2:
            return std::make_shared<GREEN>(coordinates);
        case 3:
            return std::make_shared<YELLOW>(coordinates);
        case 4:
            return std::make_shared<PINK>(coordinates);
        case 5:
            return std::make_shared<BLACK>(coordinates);
        case 6:
            return std::make_shared<WHITE>(coordinates);
        case 7:
            return std::make_shared<PURPLE>(coordinates);
        case 8:
            return std::make_shared<ZEBRA>(coordinates);
        case 9:
            return std::make_shared<IRON>(coordinates);
        case 10:
            return std::make_shared<RAINBOW>(coordinates);
        case 11:
            return std::make_shared<CERAMICS>(coordinates);
        case 12:
            return std::make_shared<MOAB>(coordinates);
        case 13:
            return std::make_shared<BFB>(coordinates);
        case 14:
            return std::make_shared<ZOMG>(coordinates);
        case 15:
            return std::make_shared<DDT>(coordinates);
        case 16:
            return std::make_shared<BAD>(coordinates);
        default:
            return std::make_shared<RED>(coordinates); // 默认返回RED类型气球
    }
}

// int getRandomInt(int min, int max){
//     static std::random_device rd;  // 硬體隨機數生成器
//     static std::mt19937 gen(rd()); // 使用 Mersenne Twister PRNG (比 rand() 更好)
//     std::uniform_int_distribution<int> dist(min, max);
//     return dist(gen);
// }

void App::Update() {
    LOG_TRACE("Update");
    



    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && m_Phase != Phase::LOBBY){
        glm::vec2 position = Util::Input::GetCursorPosition ();
        LOG_DEBUG("Mouse position: " + std::to_string(position.x) + ", " + std::to_string(position.y));
    }


    // 更新所有拖拽按钮
    for (auto& dragButtonPtr : m_DragButtons) {
        dragButtonPtr->Update();
    }

    glm::vec2 mousePosition = Util::Input::GetCursorPosition();
    // 处理猴子的拖拽逻辑
    if (m_DragMonkey) {
        // 已经有猴子在拖拽中
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) { //is move to outside will have bug
            // 鼠标按下，更新猴子位置
            m_DragMonkey->SetPosition(mousePosition);
            m_DragMonkey->UpdateRange();
            m_DragMonkey->SetRangeColor(true);
            if (m_DragMonkey->Placeable(Level_Placeable)) {
                for (auto& monkeyPtr : m_Monkeys) {
                    if (monkeyPtr->Touched(*m_DragMonkey)) {
                        m_DragMonkey->SetRangeColor(false);
                        break;
                    }
                }
            }else{
                m_DragMonkey->SetRangeColor(false);
            }
        } else {
            bool allMonkeysAllowPlacement = true;
            
            // 检查所有现有猴子是否允许在此位置放置
            if (m_DragMonkey->Placeable(Level_Placeable)) {
                for (auto& monkeyPtr : m_Monkeys) {
                    if (monkeyPtr->Touched(*m_DragMonkey)) { 
                        allMonkeysAllowPlacement = false;
                        break;
                    }
                }
            }else{
                allMonkeysAllowPlacement = false;
            }
            
            // 只有当所有条件都满足时才放置猴子
            if (allMonkeysAllowPlacement) {
                // 添加到猴子集合
                m_Monkeys.push_back(m_DragMonkey);
                
                // 设置位置并更新范围
                m_DragMonkey->SetPosition(mousePosition);
                m_DragMonkey->UpdateRange();
                m_DragMonkey->SetRangeColor(true);                
                // 确保将猴子添加到场景根节点
                m_Root.AddChild(m_DragMonkey);
            }else{
                m_Root.RemoveChild(m_DragMonkey);
                m_Root.RemoveChild(m_DragMonkey->GetRange());
            }
            
            // 无论是否成功放置，都清除拖拽引用
            m_DragMonkey = nullptr;
        }
    } else {
        // 检查是否有按钮开始拖拽
        for (auto& dragButtonPtr : m_DragButtons) {
            if (dragButtonPtr->IsPointInside(mousePosition) && Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
                // 按钮被拖动，创建新猴子
                m_DragMonkey = dragButtonPtr->ProduceMonkey(mousePosition);
                
                if (m_DragMonkey) {
                    LOG_DEBUG("Created monkey at: " + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y));

                    m_Root.AddChild(m_DragMonkey);
                    m_Root.AddChild(m_DragMonkey->GetRange());
                    
                    break; // 只处理第一个拖动的按钮
                }
            }
        }
    } 
    // cd -= 1;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && m_Phase == Phase::LOBBY){
        glm::vec2 position = Util::Input::GetCursorPosition ();
        if (position.x >= -223 && position.x <= 6 && position.y >= -124 && position.y <= -80) {
            ValidTask();
        }
    }

    if (m_Phase != Phase::LOBBY && count == 0) {
        int round = m_Counters[2] -> GetCurrent()-1;
        if (m_Balloons.empty() && Level_Balloons[round].empty()) {
            m_Counters[2] -> AddValue(1);
        }
        std::shared_ptr<Balloon> m_Balloon;
        if (!Level_Balloons[round].empty()) {
            int num = Level_Balloons[round][0];
            Level_Balloons[round].erase(Level_Balloons[round].begin());
            m_Balloon = factory(num, Level_Coordinates);
            m_Balloons.push_back(m_Balloon);
            m_Root.AddChild(m_Balloon);
        }
    }


    if (count == 0) {
        count = 20;
    }
    count --;

    // #################################################################################################

    for (auto& balloonPtr : m_Balloons) {
        bool underAttack = false;
        for (auto& attackPtr : m_Attacks) {
            if (attackPtr -> IsAlive() && balloonPtr -> IsCollision(attackPtr)){
                underAttack = true;
                balloonPtr -> LoseHealth(attackPtr -> GetPower());
                attackPtr -> LosePenetration();
                if (!attackPtr -> IsAlive()) {
                    remove_attacks.push_back(attackPtr);
                }
            }
            if (!balloonPtr -> IsAlive()) {
                break;
            }
        }
        for (auto& attackPtr : remove_attacks) {
            m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attackPtr), m_Attacks.end());
            m_Root.RemoveChild(attackPtr);
        }
        if (!balloonPtr -> IsAlive()) {
            std::vector<std::shared_ptr<Balloon>> bs = balloonPtr -> Burst();
            new_balloons.insert(new_balloons.end(), bs.begin(), bs.end());
            remove_balloons.push_back(balloonPtr);
            m_Counters[1] -> AddValue(balloonPtr -> GetMoney());
        }
        else if (underAttack) {
            balloonPtr -> Injured();
        }
    }
    for (auto& balloonPtr : remove_balloons) {
        m_Balloons.erase(std::remove(m_Balloons.begin(), m_Balloons.end(), balloonPtr), m_Balloons.end());
        m_Root.RemoveChild(balloonPtr);
    }

    remove_balloons = {};

    for (auto& balloonPtr : m_Balloons) {
        balloonPtr -> Move();
        if (balloonPtr -> IsArrive()) {
            remove_balloons.push_back(balloonPtr);
            m_Counters[0] ->MinusValue(1);
        }
    }

    for (auto& balloonPtr : remove_balloons) {
        m_Balloons.erase(std::remove(m_Balloons.begin(), m_Balloons.end(), balloonPtr), m_Balloons.end());
        m_Root.RemoveChild(balloonPtr);
    }

    for (auto& balloonPtr : new_balloons) {
        m_Balloons.push_back(balloonPtr);
        m_Root.AddChild(balloonPtr);
    }

    remove_balloons = {};
    new_balloons = {};

    std::sort(m_Balloons.begin(), m_Balloons.end(),
        [](const std::shared_ptr<Balloon>& a, const std::shared_ptr<Balloon>& b) {
            return a->GetDistance() > b->GetDistance();
        });

    remove_attacks = {};
    // #################################################################################################

    for (auto& monkeyPtr : m_Monkeys) {
        // add the check of click or not
        monkeyPtr -> CheckRangeVisible();
        if (monkeyPtr -> Countdown()) {
            for (auto& balloonPtr : m_Balloons) {
                if (monkeyPtr -> IsCollision(balloonPtr)) {
                    std::vector<std::shared_ptr<Attack>> attacks = monkeyPtr -> ProduceAttack(balloonPtr->GetPosition());
                    for (auto& attackPtra : attacks) {
                        m_Attacks.push_back(attackPtra);
                        m_Root.AddChild(attackPtra);
                    }
                    break;
                }
            }
        }
    }

    for (auto& attackPtr : m_Attacks) {
        attackPtr -> Move();
        if (attackPtr -> IsOut()) {
            remove_attacks.push_back(attackPtr);
        }
    }

    for (auto& attackPtr : remove_attacks) {
        m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attackPtr), m_Attacks.end());
        m_Root.RemoveChild(attackPtr);
    }
    remove_attacks = {};

    //##################################################################################################
    /*
     *  Do not touch the code below as they serve the purpose for validating the tasks,
     *  rendering the frame, and exiting the game.
    */


    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_EnterDown && m_Phase == Phase::LOBBY) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            ValidTask();
        }
    }

// ######################################################################

    if (m_Phase != Phase::LOBBY && m_Counters[2] -> GetCurrent() == m_Counters[2] -> GetMaxValue()) {
        if (m_Balloons.empty() && Level_Balloons[m_Counters[2] -> GetMaxValue()-1].empty()) {
            ValidTask();
        }
    }

    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);

    m_Root.Update();
}
