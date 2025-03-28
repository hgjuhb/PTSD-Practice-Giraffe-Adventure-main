#include "App.hpp"
#include <iostream>
#include <algorithm>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <random>
#include <cxxabi.h>

int count = 20;
bool blocked = false;
int block_time = 0;
int nuclear_bomb_time = 0;

std::vector<std::pair<std::shared_ptr<Balloon>, std::shared_ptr<Rope_tail>>> grabbedBalloons; // 存儲所有正在被拉動的氣球

std::vector<std::shared_ptr<Balloon>> new_balloons;
std::vector<std::shared_ptr<Balloon>> remove_balloons;
std::vector<std::shared_ptr<Attack>> remove_attacks;
std::vector<std::shared_ptr<Attack>> m_drops;
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

int current_room(App::Phase phase) {
    switch (phase) {
        case App::Phase::LOBBY:
            return 0;
        case App::Phase::FIRST_LEVEL:
            return 1;
        case App::Phase::SECOND_LEVEL:
            return 2;
    }
}

void App::Update() {
    LOG_TRACE("Update");
    if (m_Phase == Phase::LOBBY) {
        glm::vec2 position = Util::Input::GetCursorPosition ();
        if (!Choose_Level_Board -> GetVisible()) {
            Lobby_Buttons[0] -> IsTouch(position);
            Lobby_Buttons[1] -> IsTouch(position);
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                if (Lobby_Buttons[0] -> IsClicked(position)) {
                    Choose_Level_Board -> UpdateVisible(true);
                }
                if (Lobby_Buttons[1] -> IsClicked(position)) {
                    ValidTask(2);
                }
            }
        }
        else {
            Choose_Level_Board -> IsTouch(position);
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                int isClick = Choose_Level_Board -> IsClicked(position);
                if (isClick >= 0 && IsLevelUnlock[isClick]) {
                    ValidTask(isClick+1);
                }
            }
        }
    }
    else if(!Win_Board -> GetVisible() && !Lose_Board -> GetVisible()) {
        remove_balloons = {};
        // for dropbox
        std::vector<std::shared_ptr<Attack>> remove_drops;
        for (auto& dropboxPtr : m_drops) {
            if (dropboxPtr -> IsOut()) {
                //add the money to the counter
                m_Counters[1] -> AddValue(20000);
                remove_drops.push_back(dropboxPtr);
            }
        }
        for (auto& dropboxPtr : remove_drops) {
            m_drops.erase(std::remove(m_drops.begin(), m_drops.end(), dropboxPtr), m_drops.end());
            m_Root.RemoveChild(dropboxPtr);
        }
        //for dropbox end
        // skill countdown area start
        if (blocked) {
            block_time -= 1;
            if (block_time == 0) {
                blocked = false;
            }
        }
        if (nuclear_bomb_time>0){
            nuclear_bomb_time -= 1;
            if (nuclear_bomb_time == 0){
                for (auto& balloonPtr : m_Balloons) {
                    balloonPtr -> LoseHealth(350);
                }
            }
        }
        // 在更新循環裡處理被抓取的氣球
        if (!grabbedBalloons.empty()) {
            std::vector<size_t> finishedIndices; // 儲存已完成移動的氣球索引

            for (size_t i = 0; i < grabbedBalloons.size(); ++i) {
                auto rope = std::make_shared<Rope>(grabbedBalloons[i].second->GetSourcePosition(), grabbedBalloons[i].second->GetPosition(), std::make_shared<Attributes>());
                m_Attacks.push_back(rope);
                m_Root.AddChild(rope);
                if (grabbedBalloons[i].second -> CheckAndReverse()){
                    grabbedBalloons[i].first -> Move();
                    // LOG_DEBUG("move...");
                }
                // LOG_DEBUG("check out");
                if (grabbedBalloons[i].second -> IsOut()){
                    // LOG_DEBUG("out");
                    finishedIndices.push_back(i);
                    auto boom = std::make_shared<Explosive_cannon>(grabbedBalloons[i].second->GetPosition());
                    boom -> SetScale(glm::vec2(0.3, 0.3));
                    m_Attacks.push_back(boom);
                    m_Root.AddChild(boom);
                }
            }

            // 從列表中移除已完成的項目（從後往前刪除，避免索引變化）
            std::sort(finishedIndices.begin(), finishedIndices.end(), std::greater<size_t>());
            for (auto idx : finishedIndices) {
                if (idx < grabbedBalloons.size()) {
                    for (auto& debuffPtr : grabbedBalloons[idx].first -> GetDebuffViews()) {
                        m_Root.RemoveChild(debuffPtr);
                    }
                    m_Root.RemoveChild(grabbedBalloons[idx].first);
                    grabbedBalloons.erase(grabbedBalloons.begin() + idx);
                }
            }
        }
        // skill countdown area end

        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && m_Phase != Phase::LOBBY){
            glm::vec2 position = Util::Input::GetCursorPosition ();

            LOG_DEBUG("Mouse position: " + std::to_string(position.x) + ", " + std::to_string(position.y));
        }

        // 更新所有拖拽按钮
        for (auto& dragButtonPtr : m_DragButtons) {
            dragButtonPtr->Update();
            dragButtonPtr->UpdateButtonState(m_Counters[1]->GetCurrent());
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
                    m_Counters[1] -> MinusValue(m_DragMonkey -> GetCost());
                    // 确保将猴子添加到场景根节点
                    // m_Root.AddChild(m_DragMonkey);
                }else{
                    m_Root.RemoveChild(m_DragMonkey);
                    m_Root.RemoveChild(m_DragMonkey->GetRange());
                    std::vector<std::shared_ptr<Util::GameObject>> InfortionBoardObject = m_DragMonkey-> GetAllInfortionBoardObject();
                    for (auto& objectPtr : InfortionBoardObject) {
                        m_Root.RemoveChild(objectPtr);
                    }
                }

                // 无论是否成功放置，都清除拖拽引用
                m_DragMonkey = nullptr;
            }
        } else {
            // 检查是否有按钮开始拖拽
            for (auto& dragButtonPtr : m_DragButtons) {
                if (dragButtonPtr->IsPointInside(mousePosition) && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                    // 點到正確的按鈕
                    // if cant add press to try
                    m_DragMonkey = dragButtonPtr->ProduceMonkey(mousePosition); // 這個地方 按鈕如果是灰的就 必定返回空

                    if (m_DragMonkey) {
                        LOG_DEBUG("Created monkey at: " + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y));
                        m_Root.AddChild(m_DragMonkey);
                        m_Root.AddChild(m_DragMonkey-> GetRange());
                        m_Root.AddChildren(m_DragMonkey-> GetAllInfortionBoardObject());
                        break; // 只处理第一个拖动的按钮
                    }else{
                        //錢不夠所以不返回猴子
                    }
                }
            }
        }
        // cd -= 1;

        if (m_Phase != Phase::LOBBY && count == 0) {
            int round = m_Counters[2] -> GetCurrent()-1;
            if (m_Balloons.empty() && Level_Balloons[round].empty()) {
                m_Counters[2] -> AddValue(1);
            }
            std::shared_ptr<Balloon> m_Balloon;
            if (!Level_Balloons[round].empty() && !blocked) {
                int num = Level_Balloons[round][0];
                Level_Balloons[round].erase(Level_Balloons[round].begin());
                m_Balloon = factory(num, Level_Coordinates);
                m_Balloons.push_back(m_Balloon);
                m_Root.AddChildren(m_Balloon -> GetDebuffViews());
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
                    balloonPtr -> LoseHealth(balloonPtr -> IsAttackEffective(attackPtr -> GetProperties(), attackPtr -> GetPower()));
                    balloonPtr -> GetDebuff(attackPtr -> GetAttributes() -> GetDebuff());
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
            std::vector<std::shared_ptr<Util::GameObject>> debuffView = balloonPtr -> GetDebuffViews();
            m_Balloons.erase(std::remove(m_Balloons.begin(), m_Balloons.end(), balloonPtr), m_Balloons.end());
            for (auto& debuffPtr : debuffView) {
                m_Root.RemoveChild(debuffPtr);
            }
            m_Root.RemoveChild(balloonPtr);
        }


        for (auto& balloonPtr : m_Balloons) {
            balloonPtr -> Move();
            if (balloonPtr -> IsArrive()) {
                remove_balloons.push_back(balloonPtr);
                m_Counters[0] ->MinusValue(1);
            }
        }
        // passive skill every screen effect
        for (auto& monkeyPtr : m_Monkeys) {
            int status;
            std::string monkeyType = abi::__cxa_demangle(typeid(*monkeyPtr).name(), 0, 0, &status);
            if (monkeyType == "NailMonkey" && monkeyPtr -> GetSkillCountdown() > 0) {
                std::vector<std::shared_ptr<Attack>> attacks = monkeyPtr -> ProduceAttack(glm::vec2(100000, 100000));
                for (auto& attackPtr : attacks) {
                    m_Attacks.push_back(attackPtr);
                    m_Root.AddChild(attackPtr);
                }
            }
            else if (monkeyType == "SuperMonkey" && monkeyPtr -> GetSkillCountdown() > 0 ) {
                if (!m_Balloons.empty()) {  // 首先检查数组是否为空
                    std::shared_ptr<Balloon> balloonPtr = m_Balloons[0];
                    if (balloonPtr) {  // 再次确认指针不为空
                        if (balloonPtr->GetType() == Balloon::Type::spaceship) {
                            balloonPtr->LoseHealth(200);
                        } else {
                            remove_balloons.push_back(balloonPtr);
                        }
                        auto explosive_cannon = std::make_shared<Explosive_cannon>(balloonPtr->GetPosition());
                        explosive_cannon -> SetScale(glm::vec2(0.3, 0.3));
                        m_Attacks.push_back(explosive_cannon);
                        m_Root.AddChild(explosive_cannon);
                        // LOG_DEBUG(balloonPtr -> GetType());
                    }
                }
            }

        }

        for (auto& balloonPtr : remove_balloons) {
            std::vector<std::shared_ptr<Util::GameObject>> debuffView = balloonPtr -> GetDebuffViews();
            m_Balloons.erase(std::remove(m_Balloons.begin(), m_Balloons.end(), balloonPtr), m_Balloons.end());
            for (auto& debuffPtr : debuffView) {
                m_Root.RemoveChild(debuffPtr);
            }
            m_Root.RemoveChild(balloonPtr);
        }

        for (auto& balloonPtr : new_balloons) {
            m_Balloons.push_back(balloonPtr);
            m_Root.AddChildren(balloonPtr -> GetDebuffViews());
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
        if (m_ClickedMonkey) {
            glm::vec2 position = Util::Input::GetCursorPosition ();
            m_ClickedMonkey -> IsButtonTouch(position);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            glm::vec2 position = Util::Input::GetCursorPosition ();
            int clickInformationBoard = 0; //0:無, 1:有, 2:關閉, 3:賣掉, 4:使用技能, 其他:升級(多少錢回多少
            if (m_ClickedMonkey) {
                clickInformationBoard = m_ClickedMonkey -> IsInformationBoardClicked(position, m_Counters[1] -> GetCurrent());
                if (clickInformationBoard == 2) {
                    m_ClickedMonkey -> UpdateAllObjectVisible(false);
                    m_ClickedMonkey = nullptr;
                }
                else if (clickInformationBoard == 3) {
                    m_Monkeys.erase(std::remove(m_Monkeys.begin(), m_Monkeys.end(), m_ClickedMonkey), m_Monkeys.end());
                    m_Root.RemoveChild(m_ClickedMonkey);
                    m_Root.RemoveChild(m_ClickedMonkey->GetRange());
                    std::vector<std::shared_ptr<Util::GameObject>> InfortionBoardObject = m_ClickedMonkey-> GetAllInfortionBoardObject();
                    for (auto& objectPtr : InfortionBoardObject) {
                        m_Root.RemoveChild(objectPtr);
                    }
                    std::vector<std::shared_ptr<Attack>> attacks = m_ClickedMonkey-> GetAttackChildren();
                    for (auto& attacktPtr : attacks) {
                        m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attacktPtr), m_Attacks.end());
                        m_Root.RemoveChild(attacktPtr);
                    }
                    m_Counters[1] -> AddValue(m_ClickedMonkey -> GetValue());
                    m_ClickedMonkey = nullptr;
                }
                else if (clickInformationBoard == 4) {
                    int status;
                    std::string monkeyType = abi::__cxa_demangle(typeid(*m_ClickedMonkey).name(), 0, 0, &status);
                    if (monkeyType == "DartMonkey") {
                        for (auto& monkeyPtr : m_Monkeys) {
                            monkeyType = abi::__cxa_demangle(typeid(*monkeyPtr).name(), 0, 0, &status);
                            if (monkeyType == "DartMonkey") {
                                monkeyPtr -> UseSkill();
                            }
                        }
                    }
                    else if (monkeyType == "NailMonkey") {
                        m_ClickedMonkey -> UseSkill();
                    }
                    else if (monkeyType == "RubberMonkey") {
                        for (auto& BalloonPtr : m_Balloons) {
                            BalloonPtr -> GetDebuff(m_ClickedMonkey -> GetAttributes() -> GetDebuff());
                        }
                    }
                    else if (monkeyType == "IceMonkey") {
                        for (auto& BalloonPtr : m_Balloons) {
                            if (BalloonPtr -> GetType() != Balloon::Type::spaceship) {
                                BalloonPtr -> GetDebuff({{1, 240}});
                            }
                        }
                    }
                    else if (monkeyType == "Cannon") {
                        for (auto& BalloonPtr : m_Balloons) {
                            if (BalloonPtr -> GetType() == Balloon::Type::spaceship) {
                                BalloonPtr -> LoseHealth(1000);
                                auto explosive_cannon = std::make_shared<Explosive_cannon>(BalloonPtr -> GetPosition());
                                m_Attacks.push_back(explosive_cannon);
                                m_Root.AddChild(explosive_cannon);
                                break;
                            }
                        }
                    }
                    else if (monkeyType == "NinjaMonkey") {
                        blocked = true;
                        block_time = 600;
                        glm::vec2 position = Level_Coordinates[0];
                        for (auto& balloonPtr : m_Balloons) {
                            balloonPtr -> GetDebuff({{4, 600}});
                        }
                        auto rock_ninja = std::make_shared<RockNinja>(position);
                        m_Attacks.push_back(rock_ninja);
                        m_Root.AddChild(rock_ninja);
                    }
                    else if (monkeyType == "BoomerangMonkey") {
                        m_ClickedMonkey -> UseSkill();
                    }
                    else if (monkeyType == "SuperMonkey") {
                        m_ClickedMonkey -> UseSkill();
                    }
                    else if (monkeyType == "Airport") {
                        auto nuclear_bomb = std::make_shared<Nuclear_bomb>(glm::vec2(0.0, 0.0));
                        m_Attacks.push_back(nuclear_bomb);
                        m_Root.AddChild(nuclear_bomb);
                        nuclear_bomb_time = 138;
                    }
                    else if (monkeyType == "BuccaneerMonkey") {
                        // 找最前頭的飛船氣球
                        // remove_balloons = {};
                        for (size_t i = 0; i < m_Balloons.size(); i++) {
                            if (m_Balloons[i]->GetType() == Balloon::Type::spaceship && m_Balloons[i] -> GetHealth() > 0) { // make sure to not grab the bust ballone
                            // but actually it will not happen,  cause i move it from the m_Balloons list so even the balloon health is 0, it cant active burst
                                auto balloonPtr = m_Balloons[i];
                                auto rope_tail = std::make_shared<Rope_tail>(m_ClickedMonkey->GetPosition(), balloonPtr->GetPosition(), m_ClickedMonkey->GetAttributes());
                                m_Attacks.push_back(rope_tail);
                                m_Root.AddChild(rope_tail);
                                balloonPtr->SetTargetPosition(m_ClickedMonkey->GetPosition());
                                balloonPtr->SetSpeed(20.0f);
                                grabbedBalloons.push_back(std::make_pair(balloonPtr, rope_tail));
                                m_Balloons.erase(m_Balloons.begin() + i);
                                break;
                            }
                        }
                    }
                    else if (monkeyType == "SniperMonkey") {
                        // random in x(-600 350) y(0/300)
                        auto dropbox = std::make_shared<Dropbox>(glm::vec2(rand() % 950 - 600, rand() % 300));
                        dropbox -> SetScale(glm::vec2(0.1, 0.1));
                        m_drops.push_back(dropbox);
                        m_Root.AddChild(dropbox);
                    }
                    else if (monkeyType == "MagicMonkey") {
                        auto thebird = std::make_shared<TheBird>(m_ClickedMonkey->GetPosition(), glm::vec2(0.0, 0.0), m_ClickedMonkey->GetAttributes());
                        m_Attacks.push_back(thebird);
                        m_Root.AddChild(thebird);
                    }
                }
                else if (clickInformationBoard != 0 && clickInformationBoard != 1) {
                    m_Counters[1] -> MinusValue(clickInformationBoard);
                }

            }
            if (clickInformationBoard == 0) {
                m_ClickedMonkey = nullptr;
                for (auto& monkeyPtr : m_Monkeys) {
                    if (monkeyPtr -> IsClicked(position)) {
                        m_ClickedMonkey = monkeyPtr;
                    }
                }
            }
        }

        // fucking shit
        for (auto& monkeyPtr : m_Monkeys) {
            if (monkeyPtr -> Countdown()) {

                std::vector<int> properties = monkeyPtr -> GetProperties();
                std::sort(properties.begin(), properties.end());
                bool camouflage = std::binary_search(properties.begin(), properties.end(), 2);
                //
                for (auto& balloonPtr : m_Balloons) {

                    if (balloonPtr -> GetProperty(1) == 2 && !camouflage) {
                        continue;
                    }

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

    // ######################################################################

        if (m_Counters[2] -> GetCurrent() == m_Counters[2] -> GetMaxValue()) {
            if (m_Balloons.empty() && Level_Balloons[m_Counters[2] -> GetMaxValue()-1].empty()) {
                Win_Board -> UpdateVisible(true);
                IsLevelUnlock[current_room(m_Phase)] = true;
            }
        }

        if (!Win_Board -> GetVisible() && m_Counters[0] -> GetCurrent() == 0) {
            Lose_Board -> UpdateVisible(true);
        }


    }
    else {
        glm::vec2 position = Util::Input::GetCursorPosition ();
        if (Win_Board -> GetVisible()) {
            Win_Board -> IsTouch(position);
        }
        else if (Lose_Board -> GetVisible()) {
            Lose_Board -> IsTouch(position);
        }
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            bool isClick;
            if (Win_Board -> GetVisible()) {
                isClick = Win_Board -> IsClicked(position);
                if (isClick >= 0) {
                    switch (isClick) {
                        case 0:
                            ValidTask(current_room(m_Phase)+1);
                            break;
                        case 1:
                            ValidTask(0);
                            break;
                    }
                }
            }
            else if (Lose_Board -> GetVisible()) {
                isClick = Lose_Board -> IsClicked(position);
                if (isClick >= 0) {
                    switch (isClick) {
                        case 0:
                            ValidTask(current_room(m_Phase));
                            break;
                        case 1:
                            ValidTask(0);
                            break;
                    }
                }
            }
        }
    }

    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);
    m_Root.Update();
}
