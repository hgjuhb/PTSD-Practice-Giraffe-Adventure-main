#include "Balloon.hpp"
#include "Util/Logger.hpp"

Balloon::Balloon(std::vector<glm::vec2> coordinates)
    : m_Coordinates(coordinates){
    SetPosition(m_Coordinates[0]);
    m_Coordinates.erase(m_Coordinates.begin());
}

void Balloon::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void Balloon::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Balloon::Move() {
    float slow = UpdateDebuff();
    if (m_Coordinates.size() != 0){
        glm::vec2 next_coordinates = m_Coordinates[0];
        glm::vec2 direction = next_coordinates - m_Transform.translation;
        float distance = glm::length(direction);
        if (distance > 5.0f) {
            glm::vec2 unit_direction = glm::normalize(direction);
            m_Transform.translation += unit_direction * m_Speed * slow;
            m_Distance += m_Speed * slow;
        } else {
            m_Transform.translation = next_coordinates;
            m_Coordinates.erase(m_Coordinates.begin());
            m_Distance += distance;
            if (stage_Health != 0) {
                SetRotation();
            }
        }
        SetRectangleCorners();
    }
}

void Balloon::SetMoney(int n) {
    m_Money = n;
}

void Balloon::SetHealth(int n) {
    m_Health = n;
}

void Balloon::SetStageHealth(int n) {
    stage_Health = n;
}

void Balloon::SetSpeed(float n) {
    m_Speed = n;
}

void Balloon::SetWidth(int width) {
    m_Width = width;
}

void Balloon::SetHeight(int height) {
    m_Height = height;
}

void Balloon::SetRotation() {
    if (!m_Coordinates.empty()) {
        double dx = m_Coordinates[0].x - m_Transform.translation.x;
        double dy = m_Coordinates[0].y - m_Transform.translation.y;
        double angle_rad = atan2(dy, dx);
        m_Radian = angle_rad;
        m_Transform.rotation = angle_rad;
    }
}

void Balloon::SetRectangleCorners(){
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

void Balloon::SetDistance(float distance) {
    m_Distance = distance;
}

void Balloon::LoseHealth(int lose) {
    m_Health -= lose;;
}

std::vector<std::shared_ptr<Balloon>> Balloon::Burst() const {
    std::vector<std::shared_ptr<Balloon>> balloons;
    return balloons;
}

bool Balloon::IsCollision(const std::shared_ptr<Attack>& other) {
    std::vector<glm::vec2> rect1, rect2;
    std::vector<glm::vec2> edges, normals;
    edges ={};
    normals ={};
    rect1 = m_Corners;
    rect2 = other->GetConers();

    glm::vec2 edge = glm::vec2(rect1[1].x - rect1[0].x, rect1[1].y - rect1[0].y);
    edges.push_back(edge);
    normals.push_back(glm::vec2(-1*edge.y, 1*edge.x));
    edge = glm::vec2(rect1[2].x - rect1[1].x, rect1[2].y - rect1[1].y);
    edges.push_back(edge);
    normals.push_back(glm::vec2(-1*edge.y, 1*edge.x));

    edge = glm::vec2(rect2[1].x - rect2[0].x, rect2[1].y - rect2[0].y);
    edges.push_back(edge);
    normals.push_back(glm::vec2(-1*edge.y, 1*edge.x));
    edge = glm::vec2(rect2[2].x - rect2[1].x, rect2[2].y - rect2[1].y);
    edges.push_back(edge);
    normals.push_back(glm::vec2(-1*edge.y, 1*edge.x));

    for (int i = 0; i < 4; i++) {
        double min1, max1, min2, max2;
        min1 = max1 = rect1[0].x * normals[i].x + rect1[0].y * normals[i].y;
        for (int j = 1; j < 4; j++) {
            double proj = rect1[j].x * normals[i].x + rect1[j].y * normals[i].y;
            if (proj < min1) min1 = proj;
            if (proj > max1) max1 = proj;
        }
        min2 = max2 = rect2[0].x * normals[i].x + rect2[0].y * normals[i].y;
        for (int j = 1; j < 4; j++) {
            double proj = rect2[j].x * normals[i].x + rect2[j].y * normals[i].y;
            if (proj < min2) min2 = proj;
            if (proj > max2) max2 = proj;
        }
        if (!(max1 >= min2 && max2 >= min1)) {
            return false;
        }
    }
    return true;
}

void Balloon::Injured() {}

void Balloon::GetDebuff(std::vector<std::vector<int>> debuff) {
    for (auto tmp : debuff) {
        if (tmp[0] == 0 and m_Debuff[1] != 0) {
            continue;
        }
        if (tmp[0] != 3 || GetType() == spaceship) {
            m_Debuff[tmp[0]] += tmp[1];
        }
    }
    if (m_Debuff[0] > 350) {
        m_Debuff[0] = 0;
        m_Debuff[1] = 100;
    }
}

std::vector<std::shared_ptr<Util::GameObject>> Balloon::GetDebuffViews() {
    return {snow, ice, rubber};
}

float Balloon::UpdateDebuff() {
    float slow = 1;
    for (int i = 0; i < m_Debuff.size(); i++) {
        if (m_Debuff[i] != 0) {
            if (i == 0) { snow -> Update(GetPosition(), true); }
            else if (i == 1) { ice -> Update(GetPosition(), true); }
            else if (i == 2) { rubber -> Update(GetPosition(), true); }
            slow *= debuff_slow[i];
            m_Debuff[i] -= 1;
        }
        else if (i == 0) { snow -> Update(GetPosition(), false); }
        else if (i == 1) { ice -> Update(GetPosition(), false); }
        else if (i == 2) { rubber -> Update(GetPosition(), false); }
    }
    return slow;
};

void Balloon::SetType(Balloon::Type type) {
    m_Type = type;
}

// ##############################################################

RED::RED(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/red_balloon.png");
    m_Transform.scale = glm::vec2(0.8, 0.8);
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(1);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> RED::Burst() const{
    std::vector<std::shared_ptr<Balloon>> balloons;
    return balloons;
}

// ##############################################################

BLUE::BLUE(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/blue_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(1.4);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> BLUE::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<RED>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    return balloons;
}

// ##############################################################

GREEN::GREEN(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/green_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(1.8);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> GREEN::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<BLUE>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    return balloons;
}

// ##############################################################

YELLOW::YELLOW(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/yellow_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(3.2);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> YELLOW::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<GREEN>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    return balloons;
}

// ##############################################################

PINK::PINK(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/pink_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(3.5);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> PINK::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<YELLOW>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

BLACK::BLACK(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/black_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(1.8);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> BLACK::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<PINK>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<PINK>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

WHITE::WHITE(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/white_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(2.0);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> WHITE::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<PINK>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<PINK>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

PURPLE::PURPLE(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/purple_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(3.0);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> PURPLE::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<PINK>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<PINK>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

ZEBRA::ZEBRA(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/zebra_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(3.0);
    SetHealth(1);
    SetRectangleCorners();
}


std::vector<std::shared_ptr<Balloon>> ZEBRA::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<BLACK>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<WHITE>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

IRON::IRON(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/iron_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(1.0);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> IRON::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<BLACK>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<BLACK>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

RAINBOW::RAINBOW(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/rainbow_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(2.2);
    SetHealth(1);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> RAINBOW::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<ZEBRA>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<ZEBRA>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################

CERAMICS::CERAMICS(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Balloon/ceramics_balloon.png");
    SetType(balloon);
    SetMoney(1);
    SetWidth(50);
    SetHeight(50);
    SetSpeed(2.5);
    SetHealth(10);
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> CERAMICS::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    balloons.push_back(std::make_shared<RAINBOW>(coordinates));
    for (int i = 0; i < 10; i++) {balloons[0] -> Move();};
    balloons.push_back(std::make_shared<RAINBOW>(coordinates));
    balloons[0] -> SetDistance(GetDistance());
    balloons[1] -> SetDistance(GetDistance());
    return balloons;
}

// #############################################################


MOAB::MOAB(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Spaceship/M.O.A.B._1.png");
    SetType(spaceship);
    SetMoney(1);
    SetWidth(200);
    SetHeight(100);
    SetSpeed(1.0);
    SetHealth(200);
    SetStageHealth(40);
    SetRotation();
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> MOAB::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    for (int i=0; i<4; i++) {
        std::shared_ptr<Balloon> balloon = std::make_shared<CERAMICS>(coordinates);
        balloon -> SetDistance(GetDistance());
        for (int j=0; j<i*10; j++) {
            balloon -> Move();
        }
        balloons.push_back(balloon);
    }
    return balloons;
}

void MOAB::Injured() {
    int stageHealth = GetStageHealth();
    int health = GetHealth();
    int n = (5*stageHealth - health)/stageHealth+1;
    SetImage(GA_RESOURCE_DIR"/Spaceship/M.O.A.B._" + std::to_string(n) + ".png");
}

// #############################################################

BFB::BFB(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Spaceship/B.F.B._1.png");
    SetType(spaceship);
    SetMoney(1);
    SetWidth(200);
    SetHeight(100);
    SetSpeed(0.25);
    SetHealth(700);
    SetStageHealth(140);
    SetRotation();
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> BFB::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    for (int i=0; i<4; i++) {
        std::shared_ptr<Balloon> balloon = std::make_shared<MOAB>(coordinates);
        balloon -> SetDistance(GetDistance());
        for (int j=0; j<i*10; j++) {
            balloon -> Move();
        }
        balloons.push_back(balloon);
    }
    return balloons;
}

void BFB::Injured() {
    int stageHealth = GetStageHealth();
    int health = GetHealth();
    int n = (5*stageHealth - health)/stageHealth+1;
    SetImage(GA_RESOURCE_DIR"/Spaceship/B.F.B._" + std::to_string(n) + ".png");
}

// #############################################################

ZOMG::ZOMG(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Spaceship/Z.O.M.G._1.png");
    SetType(spaceship);
    SetMoney(1);
    SetWidth(200);
    SetHeight(100);
    SetSpeed(0.25);
    SetHealth(4000);
    SetStageHealth(800);
    SetRotation();
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> ZOMG::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    for (int i=0; i<4; i++) {
        std::shared_ptr<Balloon> balloon = std::make_shared<BFB>(coordinates);
        balloon -> SetDistance(GetDistance());
        for (int j=0; j<i*10; j++) {
            balloon -> Move();
        }
        balloons.push_back(balloon);
    }
    return balloons;
}

void ZOMG::Injured() {
    int stageHealth = GetStageHealth();
    int health = GetHealth();
    int n = (5*stageHealth - health)/stageHealth+1;
    SetImage(GA_RESOURCE_DIR"/Spaceship/Z.O.M.G._" + std::to_string(n) + ".png");
}

// #############################################################

DDT::DDT(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Spaceship/D.D.T._1.png");
    SetType(spaceship);
    SetMoney(1);
    SetWidth(200);
    SetHeight(100);
    SetSpeed(2.75);
    SetHealth(400);
    SetStageHealth(100);
    SetRotation();
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> DDT::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    for (int i=0; i<4; i++) {
        std::shared_ptr<Balloon> balloon = std::make_shared<CERAMICS>(coordinates);
        balloon -> SetDistance(GetDistance());
        for (int j=0; j<i*10; j++) {
            balloon -> Move();
        }
        balloons.push_back(balloon);
    }
    return balloons;
}

void DDT::Injured() {
    int stageHealth = GetStageHealth();
    int health = GetHealth();
    int n = (5*stageHealth - health)/stageHealth+1;
    SetImage(GA_RESOURCE_DIR"/Spaceship/D.D.T._" + std::to_string(n) + ".png");
}

// #############################################################

BAD::BAD(std::vector<glm::vec2> coordinates) : Balloon(coordinates){
    SetImage(GA_RESOURCE_DIR"/Spaceship/B.A.D._1.png");
    SetType(spaceship);
    SetMoney(1);
    SetWidth(200);
    SetHeight(100);
    SetSpeed(0.18);
    SetHealth(20000);
    SetStageHealth(4000);
    SetRotation();
    SetRectangleCorners();
}

std::vector<std::shared_ptr<Balloon>> BAD::Burst() const{
    std::vector<glm::vec2> coordinates = GetCoordinates();
    coordinates.insert(coordinates.begin(), GetPosition());
    std::vector<std::shared_ptr<Balloon>> balloons;
    for (int i=0; i<3; i++) {
        std::shared_ptr<Balloon> balloon = std::make_shared<DDT>(coordinates);
        balloon -> SetDistance(GetDistance());
        for (int j=0; j<i*10; j++) {
            balloon -> Move();
        }
        balloons.push_back(balloon);
    }
    for (int i=0; i<2; i++) {
        std::shared_ptr<Balloon> balloon = std::make_shared<ZOMG>(coordinates);
        balloon -> SetDistance(GetDistance());
        for (int j=0; j<i*10; j++) {
            balloon -> Move();
        }
        balloons.push_back(balloon);
    }
    return balloons;
}

void BAD::Injured() {
    int stageHealth = GetStageHealth();
    int health = GetHealth();
    int n = (5*stageHealth - health)/stageHealth+1;
    SetImage(GA_RESOURCE_DIR"/Spaceship/B.A.D._" + std::to_string(n) + ".png");
}
