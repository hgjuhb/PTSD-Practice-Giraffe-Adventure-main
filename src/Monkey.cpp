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
    float topBound = m_Transform.translation.y + m_Size.y / 2.0f;
    float bottomBound = m_Transform.translation.y - m_Size.y / 2.0f;
    
    // 判断鼠标点击位置是否在猴子范围内
    return (mousePosition.x >= leftBound &&
            mousePosition.x <= rightBound &&
            mousePosition.y >= bottomBound &&
            mousePosition.y <= topBound);
}

// 判断猴子是否在一个矩形区域内
bool Monkey::IsMonkeyInRectangle(glm::vec2 topLeft, glm::vec2 bottomRight) {
    // 计算猴子的边界范围
    float monkeyLeft = m_Transform.translation.x - m_Size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + m_Size.x / 2.0f;
    float monkeyTop = m_Transform.translation.y + m_Size.y / 2.0f;
    float monkeyBottom = m_Transform.translation.y - m_Size.y / 2.0f;
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
    float thisTopBound = m_Transform.translation.y + m_Size.y / 2.0f;
    float thisBottomBound = m_Transform.translation.y - m_Size.y / 2.0f;
    
    // 计算另一个猴子的边界范围
    float otherLeftBound = other.m_Transform.translation.x - other.m_Size.x / 2.0f;
    float otherRightBound = other.m_Transform.translation.x + other.m_Size.x / 2.0f;
    float otherTopBound = other.m_Transform.translation.y + other.m_Size.y / 2.0f;
    float otherBottomBound = other.m_Transform.translation.y - other.m_Size.y / 2.0f;
    
    // 检查两个矩形是否重叠（AABB碰撞检测）
    // 如果一个矩形在另一个矩形的左侧、右侧、上方或下方，则它们不重叠
    if (thisRightBound < otherLeftBound || 
        thisLeftBound > otherRightBound || 
        thisBottomBound > otherTopBound || 
        thisTopBound < otherBottomBound) {
        return false;
    }
    
    // 如果没有满足上述任一条件，则矩形重叠
    return true;
}

bool Monkey::IsClicked(glm::vec2 mousePosition) {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        if (IsInside(mousePosition)) {
            UpdateAllObjectVisible(true);
            return true;
        } else {
            UpdateAllObjectVisible(false);
            return false;
        }
    }
    return false;
}

void Monkey::SetCd(int cd) {
    m_Cd = cd;
};

void Monkey::SetCost(int cost) {
    m_Cost = cost;
    m_Value = cost;
}

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

int Monkey::IsInformationBoardClicked(glm::vec2 mousePosition, int money) {
    std::vector<int> val = m_InformationBoard -> IsClick(mousePosition, money);
    int res = val[0];
    if (val[0] == 4) {
        level += 1;
        upgradePath = 1;
        m_Value += val[1];
        res = val[1];
    }
    else if (val[0] == 5) {
        level += 1;
        upgradePath = 2;
        m_Value += val[1];
        res = val[1];
    }
    return res;
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
        // 驗證 topleft 和 bottomright 的順序
        if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
            std::swap(topLeft, bottomRight);
        }
        
        if (IsMonkeyInRectangle(topLeft, bottomRight)) {
            LOG_DEBUG("碰到了: {}, {}", topLeft, bottomRight);
            return false;
        }
        LOG_DEBUG("沒碰到:{}, {}", topLeft, bottomRight);
    }
    // 如果猴子不在任何一个矩形区域内，返回true
    return true;
}

void Monkey::IsButtonTouch(glm::vec2 mousePosition) {
    m_InformationBoard -> IsButtonTouch(mousePosition);
}


std::vector<std::shared_ptr<Util::GameObject>> Monkey::GetAllInfortionBoardObject() {
    std::vector<std::shared_ptr<Util::GameObject>> infortionBoardObjects = m_InformationBoard -> GetAllChildren();
    infortionBoardObjects.push_back(m_InformationBoard);
    return infortionBoardObjects;
}

void Monkey::UpdateAllObjectVisible(bool isClicked) {
    m_Range -> SetVisible(isClicked);
    m_InformationBoard -> SetVisible(isClicked);
    m_InformationBoard -> UpdateAllObjectVisible(isClicked);
}

// ####################################################################

DartMonkey::DartMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<DartMonkeyInformationBoard>();

    SetCost(200);
    SetImage(GA_RESOURCE_DIR"/Monkey/DartMonkey.png");
    SetCd(50);
    SetRadius(150);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> DartMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}


//#####################################################################

NailMonkey::NailMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    SetCost(360);
    SetImage(GA_RESOURCE_DIR"/Monkey/NailMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));
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
        std::shared_ptr<Attack> attack = std::make_shared<Nail>(GetPosition(), glm::vec2(x,y), GetRadius(), GetAttributes());
        attacks.push_back(attack);
    }
    return attacks;
}

//#####################################################################

SniperMonkey::SniperMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(2);
    attributes -> SetPower(1);
    attributes -> SetSpeed(100);

    SetCost(400);
    SetImage(GA_RESOURCE_DIR"/Monkey/SniperMonkey.png");
    SetCd(120);
    SetRadius(1500);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> SniperMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition, GetAttributes());
    attack -> SetSpeed(100);
    attack -> SetPenetration(2);
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

BoomerangMonkey::BoomerangMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/BoomerangMonkey.png");

    auto attributes = GetAttributes();
    attributes -> SetPenetration(3);
    attributes -> SetPower(1);
    attributes -> SetSpeed(10);

    SetCost(400);
    SetCd(80);
    SetRadius(120);
    UpdateRange();
    SetSize(glm::vec2(50.0f, 50.0f));
}

std::vector<std::shared_ptr<Attack>> BoomerangMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Boomerang>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

NinjaMonkey::NinjaMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/NinjaMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(60);

    SetCost(600);
    SetCd(30);
    SetRadius(150);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> NinjaMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Shuriken>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

Cannon::Cannon(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/Cannon.png");

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    SetCost(700);
    SetCd(100);
    SetRadius(160);
    UpdateRange();
    SetSize(glm::vec2(50.0f, 50.0f));
}

std::vector<std::shared_ptr<Attack>> Cannon::ProduceAttack(glm::vec2 goalPosition) {
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack1);
    std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack1, GetAttributes());
    attack2 -> SetVisible(false);
    attacks.push_back(attack2);
    return attacks;
}

//########################################################################

Airport::Airport(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/Airport.png");

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    SetCost(950);
    SetCd(100);
    SetRadius(50);
    UpdateRange();
    SetSize(glm::vec2(50.0f, 50.0f));
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
            std::shared_ptr<Attack> attack = std::make_shared<Dart>(position, rotated_position, GetAttributes());
            attacks.push_back(attack);
        }
    }

    if (m_Airplanes.size() < airplane_num) {
        glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 0);
        std::shared_ptr<Attack> attack = std::make_shared<Airplane>(position, rotated_position, GetAttributes());
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
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    SetCost(600);
    SetCd(120);
    SetRadius(200);
    UpdateRange();
}

bool BuccaneerMonkey::Placeable(std::vector<std::vector<std::vector<glm::vec2>>> Level_Placeable){
    // 检查猴子是否在有效区域内
    glm::vec2 size = GetSize();
    float monkeyLeft = m_Transform.translation.x - size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + size.x / 2.0f;

    std::vector<std::vector<glm::vec2>> groundplacelist = Level_Placeable[1];
    for (auto& groundplacestr : groundplacelist) {
        glm::vec2 topLeft = groundplacestr[0];    // 左上角坐标
        glm::vec2 bottomRight = groundplacestr[1]; // 右下角坐标
        // 驗證 topleft 和 bottomright 的順序
        if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
            std::swap(topLeft, bottomRight);
        }
        
        if (IsMonkeyInRectangle(topLeft, bottomRight)) {
            LOG_DEBUG("碰到了: {}, {}", topLeft, bottomRight);
            return true;
        }
        LOG_DEBUG("沒碰到:{}, {}", topLeft, bottomRight);
    }
    // 如果猴子不在任何一个矩形区域内，返回true
    return false;
}

std::vector<std::shared_ptr<Attack>> BuccaneerMonkey::ProduceAttack(glm::vec2 goalPosition) {
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack1);
    std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack1, GetAttributes());
    attack2 -> SetVisible(false);
    attacks.push_back(attack2);
    return attacks;
}

//########################################################################

SuperMonkey::SuperMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/SuperMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(3);
    attributes -> SetSpeed(60);

    SetCost(4000);
    SetCd(20);
    SetRadius(300);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> SuperMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;

    glm::vec2 direction = goalPosition - m_Transform.translation;
    glm::vec2 unit_direction = glm::normalize(direction);
    double perp_x = -unit_direction.y;
    double perp_y = unit_direction.x;
    int distance = 15;

    glm::vec2 movePosition = glm::vec2(distance * perp_x, distance * perp_y);

    std::shared_ptr<Attack> attack = std::make_shared<Ray>(GetPosition()+movePosition, goalPosition+movePosition, GetAttributes());
    attacks.push_back(attack);
    attack = std::make_shared<Ray>(GetPosition()-movePosition, goalPosition-movePosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

IceMonkey::IceMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/IceMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(0);
    attributes -> SetPower(0);
    attributes -> SetSpeed(0);
    attributes -> AddDebuff({0, 2});

    SetCost(380);
    SetCd(1);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> IceMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;

    std::shared_ptr<Attack> attack = std::make_shared<Blizzard>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

RubberMonkey::RubberMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/RubberMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(0);
    attributes -> SetSpeed(50);
    attributes -> AddDebuff({2, 100});

    SetCost(300);
    SetCd(60);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> RubberMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;

    std::shared_ptr<Attack> attack = std::make_shared<Rubber>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}

//########################################################################

MagicMonkey::MagicMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/MagicMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(2);
    attributes -> SetPower(2);
    attributes -> SetSpeed(80);

    SetCost(550);
    SetCd(60);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> MagicMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;

    std::shared_ptr<Attack> attack = std::make_shared<MagicBall>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    return attacks;
}