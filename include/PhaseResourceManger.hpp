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
    [[nodiscard]] std::vector<std::vector<std::vector<glm::vec2>>> GetPlaceable(int level) const;

private:
    std::shared_ptr<TaskText> m_TaskText;
    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::vector<std::vector<std::vector<glm::vec2>>>> nonplaceinfo ={
        // Level_1
        {{{glm::vec2(-622.0, 125.0),glm::vec2(-183.0, -26.0)},{glm::vec2(-64.0, 116.5),glm::vec2(306.0, -25.5)},{glm::vec2(-621.0, 119.5),glm::vec2(-174.0, -27.5)},{glm::vec2(-619.0, -28.5),glm::vec2(-582.0, -122.5)},{glm::vec2(-619.0, -82.5),glm::vec2(-466.0, -129.5)},{glm::vec2(-510.0, -87.5),glm::vec2(-460.0, -204.5)},{glm::vec2(-464.0, -163.5),glm::vec2(-331.0, -206.5)},{glm::vec2(-377.0, -201.5),glm::vec2(-328.0, -279.5)},{glm::vec2(-335.0, -244.5),glm::vec2(-206.0, -286.5)},{glm::vec2(-251.0, -279.5),glm::vec2(-202.0, -372.5)},{glm::vec2(-42.0, -237.5),glm::vec2(10.0, -374.5)},{glm::vec2(-41.0, -234.5),glm::vec2(129.0, -282.5)},{glm::vec2(91.0, -168.5),glm::vec2(128.0, -280.5)},{glm::vec2(90.0, -166.5),glm::vec2(265.0, -207.5)},{glm::vec2(220.0, -86.5),glm::vec2(264.0, -205.5)},{glm::vec2(220.0, -84.5),glm::vec2(306.0, -125.5)},{glm::vec2(-373.0, 376.5),glm::vec2(134.0, 332.5)},{glm::vec2(81.0, 378.5),glm::vec2(136.0, 260.5)},{glm::vec2(87.0, 305.5),glm::vec2(264.0, 256.5)},{glm::vec2(213.0, 298.5),glm::vec2(266.0, 180.5)},{glm::vec2(219.0, 228.5),glm::vec2(307.0, 178.5)},{glm::vec2(-380.0, 377.5),glm::vec2(-331.0, 256.5)},{glm::vec2(-507.0, 304.5),glm::vec2(-331.0, 255.5)},{glm::vec2(-507.0, 305.5),glm::vec2(-463.0, 179.5)},{glm::vec2(-464.0, 221.5),glm::vec2(-624.0, 178.5)},{glm::vec2(-624.0, 218.5),glm::vec2(-580.0, 106.5)},{glm::vec2(-455.0, 244.5),glm::vec2(-332.0, 166.5)},{glm::vec2(89.0, 243.5),glm::vec2(211.0, 166.5)},{glm::vec2(15.0, -296.5),glm::vec2(136.0, -371.5)},{glm::vec2(-374.0, -295.5),glm::vec2(-254.0, -373.5)}},{{glm::vec2(-459.0, 254.5),glm::vec2(-329.0, 159.5)},{glm::vec2(91.0, 253.5),glm::vec2(216.0, 157.5)},{glm::vec2(12.0, -292.5),glm::vec2(136.0, -380.5)},{glm::vec2(-374.0, -288.5),glm::vec2(-260.0, -379.5)}}},
        // Level_2
        {{{glm::vec2(390.0, 359.0),glm::vec2(623.0, -354.0)},{glm::vec2(-158.0, 386.5),glm::vec2(43.0, -310.5)},{glm::vec2(11.0, 381.5),glm::vec2(168.0, 307.5)},{glm::vec2(118.0, 317.5),glm::vec2(174.0, 135.5)},{glm::vec2(117.0, 205.5),glm::vec2(275.0, -67.5)},{glm::vec2(101.0, 125.5),glm::vec2(209.0, -66.5)},{glm::vec2(210.0, 11.5),glm::vec2(372.0, -69.5)},{glm::vec2(316.0, -0.5),glm::vec2(377.0, -260.5)},{glm::vec2(317.0, -190.5),glm::vec2(389.0, -263.5)},{glm::vec2(-281.0, 379.5),glm::vec2(-98.0, 305.5)},{glm::vec2(-281.0, 380.5),glm::vec2(-230.0, 133.5)},{glm::vec2(-383.0, 202.5),glm::vec2(-230.0, 133.5)},{glm::vec2(-389.0, 202.5),glm::vec2(-333.0, -65.5)},{glm::vec2(-336.0, 5.5),glm::vec2(-624.0, -64.5)},{glm::vec2(-436.0, -253.5),glm::vec2(-622.0, -67.5)},{glm::vec2(-624.0, -41.5),glm::vec2(-495.0, -92.5)},{glm::vec2(389.0, 386.5),glm::vec2(620.0, -385.5)}},{{glm::vec2(99.0, 123.5),glm::vec2(209.0, -65.5)},{glm::vec2(-609.0, 12.5),glm::vec2(-503.0, -183.5)}}}
        };
    // std::vector<std::vector<std::vector<glm::vec2>>> normal_placeable = {
    //     // Level_1
    //     {{glm::vec2(-626.0f, 358.0f), glm::vec2(-512.0f, 225.0f)},
    //         {glm::vec2(-504.0f, 359.0f), glm::vec2(-378.0f, 304.0f)},
    //         {glm::vec2(-504.0f, 359.0f), glm::vec2(-378.0f, 304.0f)},
    //         {glm::vec2(130.0f, 358.0f), glm::vec2(256.0f, 297.0f)},
    //         {glm::vec2(256.0f, 358.0f), glm::vec2(384.0f, 229.0f)},
    //         {glm::vec2(-328.0f, 334.0f), glm::vec2(95.0f, 120.0f)},
    //         {glm::vec2(-574.0f, 177.0f), glm::vec2(-335.0f, 124.0f)},
    //         {glm::vec2(96.0f, 168.0f), glm::vec2(330.0f, 122.0f)},
    //         {glm::vec2(-177.0f, 111.0f), glm::vec2(-69.0f, -17.0f)},
    //         {glm::vec2(-577.0f, -25.0f), glm::vec2(332.0f, -84.0f)},
    //         {glm::vec2(-467.0f, -90.0f), glm::vec2(213.0f, -161.0f)},
    //         {glm::vec2(-467.0f, -90.0f), glm::vec2(213.0f, -161.0f)},
    //         {glm::vec2(-334.0f, -180.0f), glm::vec2(84.0f, -242.0f)},
    //         {glm::vec2(-211.0f, -249.0f), glm::vec2(-41.0f, -358.0f)},
    //         {glm::vec2(-623.0f, -129.0f), glm::vec2(-508.0f, -359.0f)},
    //         {glm::vec2(-497.0f, -204.0f), glm::vec2(-381.0f, -357.0f)},
    //         {glm::vec2(133.0f, -208.0f), glm::vec2(271.0f, -358.0f)},
    //         {glm::vec2(264.0f, -129.0f), glm::vec2(388.0f, -359.0f)}}
    //        };
    // std::vector<std::vector<std::vector<glm::vec2>>> water_placeable = {
    //     // Level_1
    //     {
    //         {glm::vec2(-444.0f, 238.0f), glm::vec2(-342.0f, 172.0f)},
    //         {glm::vec2(100.0f, 237.0f), glm::vec2(203.0f, 175.0f)},
    //         {glm::vec2(23.0f, -306.0f), glm::vec2(127.0f, -356.0f)},
    //         {glm::vec2(-371.0f, -310.0f), glm::vec2(-263.0f, -355.0f)},}
    // };
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
        {{12,12,12,12,12}, {1,1,1,1,1,1}, {2,2,2,2,2,2}, {3,3,3,3,3,3}, {4,4,4,4,4,4},
            {5,5,5,5,5,5}, {6,6,6,6,6,6}, {7,7,7,7,7,7}, {8,8,8,8,8,8}, {9,9,9,9,9,9} },

        {{0,0,0,0,0,0}, {1,1,1,1,1,1}, {2,2,2,2,2,2}, {3,3,3,3,3,3}, {4,4,4,4,4,4},
            {5,5,5,5,5,5}, {6,6,6,6,6,6}, {7,7,7,7,7,7}, {8,8,8,8,8,8}, {9,9,9,9,9,9} }
    };
    int m_Phase = 0;
};


#endif //PHASE_MANGER_HPP