#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "TaskText.hpp"
#include "Character.hpp"
#include "BackgroundImage.hpp"

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        return {m_TaskText, m_Background};
    }

    void NextPhase();
    [[nodiscard]] std::vector<glm::vec2> GetCoordinates(int level) const;
    [[nodiscard]] std::vector<std::vector<int>> GetBalloons(int level) const;

private:
    std::shared_ptr<TaskText> m_TaskText;
    std::shared_ptr<BackgroundImage> m_Background;

    std::vector<std::vector<glm::vec2>> coordinates = {
            // Level_1
        {glm::vec2(-230.0f, -353.0f),
            glm::vec2(-233.0f, -271.0f),
            glm::vec2(-356.0f, -260.0f),
            glm::vec2(-356.0f, -190.0f),
            glm::vec2(-488.0f, -181.0f),
            glm::vec2(-487.0f, -113.0f),
            glm::vec2(-607.0f, -107.0f),
            glm::vec2(-602.0f, -9.0f),
            glm::vec2(-199.0f, -7.0f),
            glm::vec2(-198.0f, 95.0f),
            glm::vec2(-604.0f, 108.0f),
            glm::vec2(-605.0f, 199.0f),
            glm::vec2(-489.0f, 203.0f),
            glm::vec2(-487.0f, 277.0f),
            glm::vec2(-357.0f, 279.0f),
            glm::vec2(-354.0f, 348.0f),
            glm::vec2(110.0f, 348.0f),
            glm::vec2(109.0f, 279.0f),
            glm::vec2(242.0f, 278.0f),
            glm::vec2(243.0f, 199.0f),
            glm::vec2(357.0f, 201.0f),
            glm::vec2(353.0f, 94.0f),
            glm::vec2(-46.0f, 91.0f),
            glm::vec2(-48.0f, -1.0f),
            glm::vec2(356.0f, -11.0f),
            glm::vec2(357.0f, -106.0f),
            glm::vec2(240.0f, -108.0f),
            glm::vec2(238.0f, -184.0f),
            glm::vec2(108.0f, -190.0f),
            glm::vec2(109.0f, -261.0f),
            glm::vec2(-15.0f, -263.0f),
            glm::vec2(-14.0f, -356.0f)},
            // Level_2
        {glm::vec2(-630.0f, -226.0f),
            glm::vec2(-465.0f, -227.0f),
            glm::vec2(-461.0f, -36.0f),
            glm::vec2(-363.0f, -30.0f),
            glm::vec2(-361.0f, 168.0f),
            glm::vec2(-263.0f, 169.0f),
            glm::vec2(-257.0f, 339.0f),
            glm::vec2(-128.0f, 334.0f),
            glm::vec2(-127.0f, -278.0f),
            glm::vec2(11.0f, -274.0f),
            glm::vec2(13.0f, 341.0f),
            glm::vec2(141.0f, 339.0f),
            glm::vec2(140.0f, 173.0f),
            glm::vec2(246.0f, 169.0f),
            glm::vec2(251.0f, -30.0f),
            glm::vec2(349.0f, -32.0f),
            glm::vec2(349.0f, -223.0f),
            glm::vec2(384.0f, -227.0f)}
    };
    std::vector<std::string> imagePaths = {
        "InTheWallDouble",
        "InTheWall",
    };

    std::vector<std::vector<std::vector<int>>> balloons = {
        {{0,0,0,0,0,0}, {1,1,1,1,1,1}, {2,2,2,2,2,2}, {3,3,3,3,3,3}, {4,4,4,4,4,4},
            {5,5,5,5,5,5}, {6,6,6,6,6,6}, {7,7,7,7,7,7}, {8,8,8,8,8,8}, {9,9,9,9,9,9} },

        {{0,0,0,0,0,0}, {1,1,1,1,1,1}, {2,2,2,2,2,2}, {3,3,3,3,3,3}, {4,4,4,4,4,4},
            {5,5,5,5,5,5}, {6,6,6,6,6,6}, {7,7,7,7,7,7}, {8,8,8,8,8,8}, {9,9,9,9,9,9} }
    };
    int m_Phase = 0;
};


#endif //PHASE_MANGER_HPP
