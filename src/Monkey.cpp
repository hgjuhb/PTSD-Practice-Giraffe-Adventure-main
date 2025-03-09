#include "Monkey.hpp"

#include "Util/Logger.hpp"
#define PI 3.14159265358979323846

Monkey::Monkey(glm::vec2 position)
{
    SetPosition(position);
}

void Monkey::SetPosition(const glm::vec2& Position){
    m_Transform.translation = Position;
}

void Monkey::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Monkey::SetRadius(int radius) {
    m_Radius = radius;
};

void Monkey::UpdateRange() {
    m_Range -> SetPosition(m_Transform.translation);
    m_Range -> SetScale(m_Radius);
};

void Monkey::SetCd(int cd) {
    m_Cd = cd;
};

std::vector<std::shared_ptr<Attack>> Monkey::ProduceAttack(glm::vec2 goalPosition){
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    return attacks;
}

bool Monkey::IsCollision(const std::shared_ptr<Balloon>& other) const{
    glm::vec2 position = other->GetPosition();
    float distance = sqrt(pow(position.x - m_Transform.translation.x, 2) + pow(position.y - m_Transform.translation.y, 2));
    return distance <= m_Radius;
}

void Monkey::SetRotation(glm::vec2 Position) {
    double dx = Position.x - m_Transform.translation.x;
    double dy = Position.y - m_Transform.translation.y;
    double angle_rad = atan2(dy, dx);
    m_Transform.rotation = angle_rad;
}

bool Monkey::Countdown() {
    if (m_Count == m_Cd) {
        return true;
    }
    m_Count += 1;
    return false;
}

void Monkey::ResetCount(){
    m_Count = 0;
}

// ####################################################################

DartMonkey::DartMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/DartMonkey.png");
    SetCd(50);
    SetRadius(150);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> DartMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition);
    attacks.push_back(attack);
    return attacks;
}

//#####################################################################

NailMonkey::NailMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/NailMonkey.png");
    SetCd(60);
    SetRadius(150);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> NailMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;
    for (int i = 0; i < 8; i++) {
        double theta = i * PI / 4;  // 角度轉為弧度
        double x = m_Transform.translation.x + 1 * cos(theta);
        double y = m_Transform.translation.y + 1 * sin(theta);
        std::shared_ptr<Attack> attack = std::make_shared<Nail>(GetPosition(), glm::vec2(x,y), GetRadius());
        attacks.push_back(attack);
    }
    return attacks;
}

//#####################################################################

SniperMonkey::SniperMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/SniperMonkey.png");
    SetCd(120);
    SetRadius(1500);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> SniperMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition);
    attack -> SetSpeed(100);
    attack -> SetPenetration(2);
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

BoomerangMonkey::BoomerangMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/BoomerangMonkey.png");
    SetCd(80);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> BoomerangMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Boomerang>(GetPosition(), goalPosition);
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

NinjaMonkey::NinjaMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/NinjaMonkey.png");
    SetCd(30);
    SetRadius(150);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> NinjaMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Shuriken>(GetPosition(), goalPosition);
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

Cannon::Cannon(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/Cannon.png");
    SetCd(100);
    SetRadius(160);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> Cannon::ProduceAttack(glm::vec2 goalPosition) {
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition);
    attacks.push_back(attack1);
    std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack1);
    attack2 -> SetVisible(false);
    attacks.push_back(attack2);
    return attacks;
}

//########################################################################

Airport::Airport(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/Airport.png");
    SetCd(100);
    SetRadius(20);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> Airport::ProduceAttack(glm::vec2 goalPosition) {
    glm::vec2 position = GetPosition();
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;
    for (auto& airplanePtr : m_Airplanes) {
        glm::vec2 position = airplanePtr -> GetPosition();
        for (int i=0; i < 8; i++) {
            glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 45*i);
            std::shared_ptr<Attack> attack = std::make_shared<Dart>(position, rotated_position);
            attacks.push_back(attack);
        }
    }

    if (m_Airplanes.size() < airplane_num) {
        glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 0);
        std::shared_ptr<Attack> attack = std::make_shared<Airplane>(position, rotated_position);
        m_Airplanes.push_back(attack);
        attacks.push_back(attack);
    }

    return attacks;
}

bool Airport::IsCollision(const std::shared_ptr<Balloon>& other) const{
    return true;
}


glm::vec2 Airport::ProduceCoordinateByAngle(glm::vec2 position, float angle) {
    double radian = angle *PI / 180.0;
    glm::vec2 rotated;
    glm::vec2 rotated_position;
    rotated.x = position.x * cos(radian) - position.y* sin(radian);
    rotated.y = position.x * sin(radian) + position.y * cos(radian);
    rotated_position.x = position.x + rotated.x;
    rotated_position.y = position.y + rotated.y;
    return rotated_position;
}