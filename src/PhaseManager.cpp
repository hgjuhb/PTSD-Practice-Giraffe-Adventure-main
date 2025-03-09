#include "PhaseResourceManger.hpp"

#include "Util/Logger.hpp"

PhaseResourceManger::PhaseResourceManger() {
    m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

void PhaseResourceManger::NextPhase() {
    if (m_Phase == 7) return;
    m_Background->NextPhase(imagePaths[m_Phase]);
    m_TaskText->NextPhase(m_Phase++);
}

std::vector<glm::vec2> PhaseResourceManger::GetCoordinates(int level) const {
    return coordinates[level];
}

std::vector<std::vector<int>> PhaseResourceManger::GetBalloons(int level) const {
    return balloons[level];
}

std::vector<std::vector<std::vector<glm::vec2>>> PhaseResourceManger::GetPlaceable(int level) const {
    std::vector<std::vector<std::vector<glm::vec2>>> placeable;
    placeable.push_back(normal_placeable[level]);
    placeable.push_back(water_placeable[level]);
    return placeable;
}
