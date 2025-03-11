#include "Monkey.hpp"

#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

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
void Monkey::SetSize(glm::vec2 size){
    m_Size = size;
};
void Monkey::UpdateRange() {
    m_Range -> SetPosition(m_Transform.translation);
    m_Range -> SetScale(m_Radius);
};
bool Monkey::IsInside(glm::vec2 mousePosition){
    // 计算猴子的边界范围
    float leftBound = m_Transform.translation.x - m_Size.x / 2.0f;
    float rightBound = m_Transform.translation.x + m_Size.x / 2.0f;
    float topBound = m_Transform.translation.y - m_Size.y / 2.0f;
    float bottomBound = m_Transform.translation.y + m_Size.y / 2.0f;
    
    // 判断鼠标点击位置是否在猴子范围内
    return (mousePosition.x >= leftBound &&
            mousePosition.x <= rightBound &&
            mousePosition.y >= topBound &&
            mousePosition.y <= bottomBound);
}

// 判断猴子是否在一个矩形区域内
bool Monkey::IsMonkeyInRectangle(glm::vec2 topLeft, glm::vec2 bottomRight) {
    // 计算猴子的边界范围
    float monkeyLeft = m_Transform.translation.x - m_Size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + m_Size.x / 2.0f;
    float monkeyTop = m_Transform.translation.y - m_Size.y / 2.0f;
    float monkeyBottom = m_Transform.translation.y + m_Size.y / 2.0f;
    LOG_DEBUG("monkeyLeft: {}, monkeyRight: {}, monkeyTop: {}, monkeyBottom: {}", monkeyLeft, monkeyRight, monkeyTop, monkeyBottom);
    
    // 使用 AABB 碰撞检测来判断是否有重叠
    // 如果一个矩形在另一个矩形的左侧、右侧、上方或下方，则它们不重叠
    if (monkeyRight < topLeft.x || 
        monkeyLeft > bottomRight.x || 
        monkeyBottom > topLeft.y || 
        monkeyTop < bottomRight.y) {
        return false;
    }
    
    // 如果没有满足上述任一条件，则矩形重叠
    return true;
}

void Monkey::SetRangeColor(bool is_placeable){
    if (is_placeable){
        m_Range -> SetImage(GA_RESOURCE_DIR"/Monkey/range.png");
    } else {
        m_Range -> SetImage(GA_RESOURCE_DIR"/Monkey/range_red.png");
    }
}
bool Monkey::Touched(Monkey& other){
    // 计算当前猴子的边界范围
    float thisLeftBound = m_Transform.translation.x - m_Size.x / 2.0f;
    float thisRightBound = m_Transform.translation.x + m_Size.x / 2.0f;
    float thisTopBound = m_Transform.translation.y - m_Size.y / 2.0f;
    float thisBottomBound = m_Transform.translation.y + m_Size.y / 2.0f;
    
    // 计算另一个猴子的边界范围
    float otherLeftBound = other.m_Transform.translation.x - other.m_Size.x / 2.0f;
    float otherRightBound = other.m_Transform.translation.x + other.m_Size.x / 2.0f;
    float otherTopBound = other.m_Transform.translation.y - other.m_Size.y / 2.0f;
    float otherBottomBound = other.m_Transform.translation.y + other.m_Size.y / 2.0f;
    
    // 检查两个矩形是否重叠（AABB碰撞检测）
    // 如果一个矩形在另一个矩形的左侧、右侧、上方或下方，则它们不重叠
    if (thisRightBound < otherLeftBound || 
        thisLeftBound > otherRightBound || 
        thisBottomBound < otherTopBound || 
        thisTopBound > otherBottomBound) {
        return false;
    }
    
    // 如果没有满足上述任一条件，则矩形重叠
    return true;
}

void Monkey::CheckRangeVisible() {
    glm::vec2 mousePosition = Util::Input::GetCursorPosition();
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        if (IsInside(mousePosition)) {
            m_Range -> SetVisible(true);
        } else {
            m_Range -> SetVisible(false);
        }
    }
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

bool Monkey::Placeable(std::vector<std::vector<std::vector<glm::vec2>>> Level_Placeable){
    // 检查猴子是否在 x >= 388 的区域
    float monkeyLeft = m_Transform.translation.x - m_Size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + m_Size.x / 2.0f;
    if (monkeyRight >= 388.0) {
        LOG_DEBUG("猴子位置超出 x=388 的限制");
        return false;
    }

    std::vector<std::vector<glm::vec2>> groundplacelist = Level_Placeable[0];
    for (auto& groundplacestr : groundplacelist) {
        glm::vec2 topLeft = groundplacestr[0];    // 左上角坐标
        glm::vec2 bottomRight = groundplacestr[1]; // 右下角坐标
        LOG_DEBUG("topLeft: {}, bottomRight: {}", topLeft, bottomRight);
        
        if (IsMonkeyInRectangle(topLeft, bottomRight)) {
            LOG_DEBUG("猴子在矩形区域内");
            return false;
        }
    }
    // 如果猴子不在任何一个矩形区域内，返回true
    return true;
}
// ####################################################################

DartMonkey::DartMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetSize(glm::vec2(100.0f, 100.0f));
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
    SetSize(glm::vec2(100.0f, 100.0f));
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
    SetSize(glm::vec2(100.0f, 100.0f));
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
    SetSize(glm::vec2(100.0f, 100.0f));
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
    SetSize(glm::vec2(100.0f, 100.0f));
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
    SetSize(glm::vec2(100.0f, 100.0f));
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
    SetRadius(50);
    UpdateRange();
    SetSize(glm::vec2(100.0f, 100.0f));
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
    double radian = angle * PI / 180.0;
    glm::vec2 rotated;
    glm::vec2 rotated_position;
    rotated.x = position.x * cos(radian) - position.y * sin(radian);
    rotated.y = position.x * sin(radian) + position.y * cos(radian);
    rotated_position.x = position.x + rotated.x;
    rotated_position.y = position.y + rotated.y;
    return rotated_position;
}

//########################################################################

BuccaneerMonkey::BuccaneerMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.5f, 1.5f);
    SetImage(GA_RESOURCE_DIR"/Monkey/BuccaneerMonkey.png");
    SetCd(120);
    SetRadius(200);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> BuccaneerMonkey::ProduceAttack(glm::vec2 goalPosition) {
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

SuperMonkey::SuperMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/SuperMonkey.png");
    SetCd(20);
    SetRadius(300);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> SuperMonkey::ProduceAttack(glm::vec2 goalPosition) {
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;

    glm::vec2 direction = goalPosition - m_Transform.translation;
    glm::vec2 unit_direction = glm::normalize(direction);
    double perp_x = -unit_direction.y;
    double perp_y = unit_direction.x;
    int distance = 15;

    glm::vec2 movePosition = glm::vec2(distance * perp_x, distance * perp_y);

    std::shared_ptr<Attack> attack = std::make_shared<Ray>(GetPosition()+movePosition, goalPosition+movePosition);
    attacks.push_back(attack);
    attack = std::make_shared<Ray>(GetPosition()-movePosition, goalPosition-movePosition);
    attacks.push_back(attack);
    return attacks;
}