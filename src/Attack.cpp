#include "Attack.hpp"
#include <cmath>
#include <random>
#include "Util/Logger.hpp"
#define PI 3.14159265358979323846

Attack::Attack(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
{
    m_Attributes = std::make_shared<Attributes>(*attributes);
    SetPosition(position);
    SetUnitDirection(goal_position);
    SetRotation(goal_position);
}

void Attack::SetPosition(const glm::vec2& Position){
    m_Transform.translation = Position;
}

void Attack::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Attack::SetPenetration(int penetration) {
    m_Attributes -> SetPenetration(penetration);
}

void Attack::SetSpeed(float speed) {
    m_Attributes -> SetSpeed(speed);
}

void Attack::SetPower(int power) {
    m_Attributes -> SetPower(power);
}

void Attack::SetUnitDirection(const glm::vec2& goalPosition) {
    glm::vec2 direction = goalPosition - m_Transform.translation;
    glm::vec2 unit_direction = glm::normalize(direction);
    m_UnitDirection = unit_direction;
}

void Attack::SetRotation(const glm::vec2& goalPosition) {
    double dx = goalPosition.x - m_Transform.translation.x;
    double dy = goalPosition.y - m_Transform.translation.y;
    double angle_rad = atan2(dy, dx);
    m_Radian = angle_rad;
    m_Transform.rotation = angle_rad;
}

void Attack::SetCorners(const std::vector<glm::vec2>& corners) {
    m_Corners = corners;
}

void Attack::Move() {
    m_Transform.translation += m_UnitDirection * m_Attributes -> GetSpeed();
    SetRectangleCorners();
}

void Attack::SetRectangleCorners(){
    double cos_a = cos(m_Radian);
    double sin_a = sin(m_Radian);
    double half_w = m_Width / 2;
    double half_h = m_Height / 2;
    std::vector<std::vector<double>> localCorners = {
        { half_w,  half_h},
        {-half_w,  half_h},
        {-half_w, -half_h},
        { half_w, -half_h}
    };
    // 計算旋轉後的座標
    for (int i = 0; i < 4; i++) {
        m_Corners[i].x = m_Transform.translation.x + localCorners[i][0] * cos_a - localCorners[i][1] * sin_a;
        m_Corners[i].y = m_Transform.translation.y + localCorners[i][0] * sin_a + localCorners[i][1] * cos_a;
    }
}

void Attack::LosePenetration() {
    m_Attributes -> SetPenetration(m_Attributes -> GetPenetration() -1);
}

bool Attack::IsOut() {
    float x = m_Transform.translation.x;
    float y = m_Transform.translation.y;
    return x < -640 || x > 640 || y < -360 || y > 360;
}

void Attack::SetWidth(int width) {
    m_Width = width;
}

void Attack::SetHeight(int height) {
    m_Height = height;
}

void Attack::RenewUnitDirection(glm::vec2 unitDirection) {
    m_UnitDirection = unitDirection;
}

// #########################################################

Dart::Dart(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Dart.png");
    SetWidth(20);
    SetHeight(10);
    SetRectangleCorners();
}

// #########################################################

Nail::Nail(glm::vec2 position, glm::vec2 goal_position, int radius, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Nail.png");
    m_SourcePosition = position;
    m_Radius = radius;
    SetWidth(35);
    SetHeight(15);
    SetRectangleCorners();
}

void Nail::Move(){
    glm::vec2 UnitDirection = GetUnitDirection();
    m_Transform.translation += UnitDirection * GetSpeed();
    float distance = sqrt(pow(m_SourcePosition.x - m_Transform.translation.x, 2) + pow(m_SourcePosition.y - m_Transform.translation.y, 2));
    if (distance > m_Radius) {
        m_Transform.translation = glm::vec2(-650, -370);
    }
    SetRectangleCorners();
}


// #########################################################

Boomerang::Boomerang(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Boomerang.png");
    m_SourcePosition = position;

    glm::vec2 direction = goal_position - m_Transform.translation;  // 計算目標方向
    float distance = glm::length(direction);  // 計算當前距離
    glm::vec2 unit_direction = glm::normalize(direction);  // 計算單位方向向量

    // 更新目標位置，方向向量乘上距離並縮放
    m_GoalPosition = glm::vec2(
        m_Transform.translation.x + (unit_direction.x * distance * 2.0),  // X 分量
        m_Transform.translation.y + (unit_direction.y * distance * 2.0)   // Y 分量
    );

    SetWidth(45);
    SetHeight(45);
    SetRectangleCorners();
}

void Boomerang::Move(){
    RotationImage();
    if (isTurn != 0) {
        if (isTurn == 1) {
            SetUnitDirection(m_SourcePosition);
        }
        else {
            isTurn -= 1;
            RotationUnitDirection();
        }
    }
    glm::vec2 UnitDirection = GetUnitDirection();
    m_Transform.translation += UnitDirection * GetSpeed();
    float distance = sqrt(pow(m_GoalPosition.x - m_Transform.translation.x, 2) + pow(m_GoalPosition.y - m_Transform.translation.y, 2));
    if (distance <= GetSpeed()) {
        isTurn = turn;
    }
    SetRectangleCorners();
}

void Boomerang::RotationUnitDirection() {
    glm::vec2 unitDirection = GetUnitDirection();
    glm::vec2 rotated;
    float theta = PI/12;
    rotated.x = unitDirection.x * cos(theta) - unitDirection.y* sin(theta);
    rotated.y = unitDirection.x * sin(theta) + unitDirection.y * cos(theta);
    RenewUnitDirection(rotated);
}

void Boomerang::RotationImage() {
    float theta = 2*PI - rotation*PI/2;
    m_Transform.rotation = theta;
    rotation += 1;
    rotation %= 4;
};

[[nodiscard]] bool Boomerang::IsOut() {
    float distance = sqrt(pow(m_SourcePosition.x - m_Transform.translation.x, 2) + pow(m_SourcePosition.y - m_Transform.translation.y, 2));
    return isTurn == 1 and distance <= GetSpeed();
}

[[nodiscard]] bool Boomerang::IsAlive() {
    if (GetPenetration() == 0 && WillNotDisappear){
        WillNotDisappear = false;
        return true;
    }
    return GetPenetration() > 0;
}

//###########################################################

Shuriken::Shuriken(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes) {
    SetImage(GA_RESOURCE_DIR"/Attack/Shuriken.png");
    SetWidth(20);
    SetHeight(20);
    SetRectangleCorners();
}

void Shuriken::Move() {
    RotationImahe();
    glm::vec2 UnitDirection = GetUnitDirection();
    m_Transform.translation += UnitDirection * GetSpeed();
    SetRectangleCorners();
}

void Shuriken::RotationImahe() {
    float theta = 2*PI - rotation*PI/3;
    m_Transform.rotation = theta;
    rotation += 1;
    rotation %= 6;
};

//###########################################################

Bomb::Bomb(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes) {
    SetImage(GA_RESOURCE_DIR"/Attack/Bomb.png");
    SetWidth(20);
    SetHeight(20);
    SetRectangleCorners();
}

//###########################################################

Explosion::Explosion(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attack> bomb, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes) {
    SetImage(GA_RESOURCE_DIR"/Attack/Explosion.png");
    m_Transform.scale = glm::vec2( 2.0, 2.0);
    m_Bomb = bomb;
    SetWidth(120);
    SetHeight(120);
    SetRectangleCorners();
}

[[nodiscard]] bool Explosion::IsOut() {
    if (m_Bomb -> IsAlive() and !m_Bomb -> IsOut()) {
        return false;
    }
    else {
        if (existTime != 0) {
            existTime --;
            return false;
        }
        return true;
    }
}

[[nodiscard]] bool Explosion::IsAlive() {
    if (m_Bomb -> IsAlive()) {
        return false;
    }
    else {
        SetSpeed(0);
        SetVisible(true);
        return true;
    }
}

//###########################################################

Airplane::Airplane(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes) {
    auto m_attributes = GetAttributes();
    m_attributes = std::make_shared<Attributes>();
    m_attributes -> SetPenetration(5);
    m_attributes -> SetPower(1);
    m_attributes -> SetPenetration(10);

    SetImage(GA_RESOURCE_DIR"/Attack/Airplane.png");
    m_Transform.scale = glm::vec2( 1.5, 1.5);
    m_SourcePosition = position;
    SetWidth(102);
    SetHeight(121);
    max_Penetration = 5;
    SetRectangleCorners();
}

void Airplane::Move() {
    if (time == 0) {
        time = 20000;  // 設定初始時間
    }
    time -= 1;

    // 參數化 8 字型運動
    float A = 450.0f; // 控制 8 字形的寬度
    float B = 150.0f; // 控制 8 字形的高度
    float t = time * 0.04f; // 時間變數縮放 (調整速度)

    glm::vec2 newPos;

    // 8 字型路徑公式：sin(t) 和 sin(t) * cos(t)
    newPos.x = A * sin(t);  // 計算 X 軸的運動
    newPos.y = B * sin(2 * t);  // 計算 Y 軸的運動，形成 8 字形的效果
    newPos.x = m_SourcePosition.x + A * sin(t);
    newPos.y = m_SourcePosition.y + B * sin(2 * t);

    RotationImage(newPos);

    m_Transform.translation = newPos;

    float distance = sqrt(pow(m_SourcePosition.x - m_Transform.translation.x, 2) + pow(m_SourcePosition.y - m_Transform.translation.y, 2));
    if (distance < 20) {
        WillNotDisappear = true;
        SetPenetration(max_Penetration);
    }

    SetRectangleCorners();  // 更新物體的邊界
}

void Airplane::RotationImage(glm::vec2 goal_position) {
    double dx = goal_position.x - m_Transform.translation.x;
    double dy = goal_position.y - m_Transform.translation.y;
    double angle_rad = atan2(dy, dx);
    m_Transform.rotation = angle_rad;
}


[[nodiscard]] bool Airplane::IsOut() {
    return false;
}

[[nodiscard]] bool Airplane::IsAlive() {
    if (GetPenetration() == 0 && WillNotDisappear){
        WillNotDisappear = false;
        return true;
    }
    return GetPenetration() > 0;
}

//###########################################################

Ray::Ray(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Ray.png");
    m_Transform.scale = glm::vec2( 0.5, 0.8);
    SetWidth(50);
    SetHeight(32);
    SetRectangleCorners();
}

//##########################################################

Blizzard::Blizzard(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes, int radius)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Blizzard.png");
    float scale = radius*2/240;
    m_Transform.scale = glm::vec2( scale, scale);
    SetWidth(radius*2);
    SetHeight(radius*2);
    SetRectangleCorners();
}

void Blizzard::Move() {
    m_Transform.rotation = PI/2;
}

[[nodiscard]] bool Blizzard::IsOut() {
    if (exit != 0) {
        exit -= 1;
        return false;
    }
    return true;
}

[[nodiscard]] bool Blizzard::IsAlive() {
    return true;
}

//#######################################################

Rubber::Rubber (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Rubber.png");
    SetWidth(20);
    SetHeight(15);
    SetRectangleCorners();
}

//#######################################################

MagicBall::MagicBall (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/MagicBall.png");
    m_Transform.scale = glm::vec2( 2, 2);
    SetWidth(60);
    SetHeight(60);
    SetRectangleCorners();
}

//#######################################################

Rock::Rock(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Rock.png");
    SetWidth(50);
    SetHeight(50);
    SetRectangleCorners();
}

//#######################################################

Fire::Fire(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes, int radius)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Fire.png");
    m_scale = (radius*2/100);
    m_Transform.scale = glm::vec2( m_scale/exit, m_scale/exit);
    SetWidth(radius*2);
    SetHeight(radius*2);
    SetRectangleCorners();
}

void Fire::Move() {
    if (exit != 0) {
        m_Transform.scale = glm::vec2( m_scale/(exit-1), m_scale/(exit-1));
    }
}

[[nodiscard]] bool Fire::IsOut() {
    if (exit == 0) {
        return true;
    }
    exit -= 1;
    return false;
}

[[nodiscard]] bool Fire::IsAlive() {
    if (exit == 1) {
        return true;
    }
    return false;
}

//###########################################################################

Knife::Knife(glm::vec2 position, glm::vec2 goal_position, int radius, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/Knife.png");
    m_Transform.scale = glm::vec2(1.6,1.6);
    m_SourcePosition = position;
    m_Radius = radius;
    SetWidth(80);
    SetHeight(80);
    SetRectangleCorners();
}

void Knife::Move(){
    glm::vec2 UnitDirection = GetUnitDirection();
    m_Transform.translation += UnitDirection * GetSpeed();
    float distance = sqrt(pow(m_SourcePosition.x - m_Transform.translation.x, 2) + pow(m_SourcePosition.y - m_Transform.translation.y, 2));
    if (distance > m_Radius) {
        m_Transform.translation = glm::vec2(-650, -370);
    }
    SetRectangleCorners();
}

//###########################################################

BladedDisc::BladedDisc(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/BladedDisc.png");
    m_Transform.scale = glm::vec2(1.6, 1.6);
    m_SourcePosition = position;

    glm::vec2 direction = goal_position - m_Transform.translation;  // 計算目標方向
    float distance = glm::length(direction);  // 計算當前距離
    glm::vec2 unit_direction = glm::normalize(direction);  // 計算單位方向向量

    // 更新目標位置，方向向量乘上距離並縮放
    m_GoalPosition = glm::vec2(
        m_Transform.translation.x + (unit_direction.x * distance * 2.0),  // X 分量
        m_Transform.translation.y + (unit_direction.y * distance * 2.0)   // Y 分量
    );

    SetWidth(80);
    SetHeight(80);
    SetRectangleCorners();
}

void BladedDisc::Move(){
    RotationImage();
    if (isTurn != 0) {
        if (isTurn == 1) {
            SetUnitDirection(m_SourcePosition);
        }
        else {
            isTurn -= 1;
            RotationUnitDirection();
        }
    }
    glm::vec2 UnitDirection = GetUnitDirection();
    m_Transform.translation += UnitDirection * GetSpeed();
    float distance = sqrt(pow(m_GoalPosition.x - m_Transform.translation.x, 2) + pow(m_GoalPosition.y - m_Transform.translation.y, 2));
    if (distance <= GetSpeed()) {
        isTurn = turn;
    }
    SetRectangleCorners();
}

void BladedDisc::RotationUnitDirection() {
    glm::vec2 unitDirection = GetUnitDirection();
    glm::vec2 rotated;
    float theta = PI/12;
    rotated.x = unitDirection.x * cos(theta) - unitDirection.y* sin(theta);
    rotated.y = unitDirection.x * sin(theta) + unitDirection.y * cos(theta);
    RenewUnitDirection(rotated);
}

void BladedDisc::RotationImage() {
    float theta = 2*PI - rotation*PI/2;
    m_Transform.rotation = theta;
    rotation += 1;
    rotation %= 4;
};

[[nodiscard]] bool BladedDisc::IsOut() {
    float distance = sqrt(pow(m_SourcePosition.x - m_Transform.translation.x, 2) + pow(m_SourcePosition.y - m_Transform.translation.y, 2));
    return isTurn == 1 and distance <= GetSpeed();
}

[[nodiscard]] bool BladedDisc::IsAlive() {
    if (GetPenetration() == 0 && WillNotDisappear){
        WillNotDisappear = false;
        return true;
    }
    return GetPenetration() > 0;
}

//###########################################################

BladedDisc_Rebound::BladedDisc_Rebound(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/BladedDisc.png");
    m_Transform.scale = glm::vec2(1.6, 1.6);
    m_SourcePosition = position;

    SetWidth(80);
    SetHeight(80);
    SetRectangleCorners();
}

void BladedDisc_Rebound::Move(){
    RotationImage();
    float x = m_Transform.translation.x;
    float y = m_Transform.translation.y;
    if (x < -640 || x > 391 || y < -360 || y > 360){
        RotationUnitDirection();
    }
    glm::vec2 UnitDirection = GetUnitDirection();
    m_Transform.translation += UnitDirection * GetSpeed();
    SetRectangleCorners();
}

void BladedDisc_Rebound::RotationUnitDirection() {
    std::random_device rd;  // 用來獲取隨機種子
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-60, 60);
    int random_number = dis(gen);
    double radians = random_number * PI / 180.0;  // 角度轉為弧度
    double cosA = cos(radians);
    double sinA = sin(radians);
    glm::vec2 center = m_Transform.translation;
    double x = center.x + (m_SourcePosition.x - center.x) * cosA - (m_SourcePosition.y - center.y) * sinA;
    double y = center.y + (m_SourcePosition.x - center.x) * sinA + (m_SourcePosition.y - center.y) * cosA;
    SetUnitDirection(glm::vec2(x, y));
}

void BladedDisc_Rebound::RotationImage() {
    float theta = 2*PI - rotation*PI/2;
    m_Transform.rotation = theta;
    rotation += 1;
    rotation %= 4;
};

[[nodiscard]] bool BladedDisc_Rebound::IsOut() {
    return false;
}

//###########################################################

BladedDisc_Around::BladedDisc_Around(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes)
: Attack(position, goal_position, attributes){
    SetImage(GA_RESOURCE_DIR"/Attack/BladedDisc.png");
    m_Transform.scale = glm::vec2(1.6, 1.6);
    m_SourcePosition = position;
    max_Penetration = attributes -> GetPenetration();
    renewPenetrationCd = 100;
    SetPosition(goal_position);
    SetWidth(80);
    SetHeight(80);
    SetRectangleCorners();
}

void BladedDisc_Around::Move(){
    RotationImage();
    if (renewPenetrationCd == 0) {
        auto m_attributes = GetAttributes();
        m_attributes -> SetPenetration(max_Penetration);
        renewPenetrationCd = 100;
        WillNotDisappear = true;
    }
    renewPenetrationCd -= 1;
    glm::vec2 direction = m_Transform.translation - m_SourcePosition;  // 計算當前位置到中心的方向
    float angle = atan2(direction.y, direction.x) + 0.05;  // 計算新的角度並加上旋轉速度
    float radius = 80;  // 保持與 m_SourcePosition 的距離
    m_Transform.translation.x = m_SourcePosition.x + radius * cos(angle);  // 更新 x 座標
    m_Transform.translation.y = m_SourcePosition.y + radius * sin(angle);  // 更新 y 座標
    SetRectangleCorners();
}


void BladedDisc_Around::RotationImage() {
    float theta = 2*PI - rotation*PI/2;
    m_Transform.rotation = theta;
    rotation += 1;
    rotation %= 4;
};

[[nodiscard]] bool BladedDisc_Around::IsAlive() {
    if (GetPenetration() == 0 && WillNotDisappear){
        WillNotDisappear = false;
        return true;
    }
    return GetPenetration() > 0;
}

[[nodiscard]] bool BladedDisc_Around::IsOut() {
    return false;
}