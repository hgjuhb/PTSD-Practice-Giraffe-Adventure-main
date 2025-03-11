#pragma once
#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <vector>
#include <string>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Attributes.hpp"

class Attack : public Util::GameObject {
public:
    explicit Attack(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);

    void SetPosition(const glm::vec2& Position);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetImage(const std::string& ImagePath);
    void SetPenetration(int penetration);
    void SetSpeed(float speed);
    void SetPower(int power);
    void SetUnitDirection(const glm::vec2& goalPosition);
    void SetRotation(const glm::vec2& goalPosition);
    void SetCorners(const std::vector<glm::vec2>& corners);
    void RenewUnitDirection(glm::vec2 unitDirection);
    virtual void Move();
    void LosePenetration();
    void SetRectangleCorners();

    [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] virtual bool IsAlive() { return m_Attributes -> GetPenetration() > 0;}
    [[nodiscard]] float GetRadian() const { return m_Radian; }
    [[nodiscard]] float GetSpeed() const { return m_Attributes -> GetSpeed(); }
    [[nodiscard]] glm::vec2 GetUnitDirection() const { return m_UnitDirection; }
    [[nodiscard]] int GetPenetration() const { return m_Attributes -> GetPenetration(); }
    [[nodiscard]] int GetPower() const { return m_Attributes -> GetPower(); }
    [[nodiscard]] std::vector<glm::vec2> GetConers() const { return m_Corners; }
    [[nodiscard]] virtual bool IsOut();
    [[nodiscard]] std::shared_ptr<Attributes> GetAttributes() { return m_Attributes; }

private:
    int m_Width;
    int m_Height;
    float m_Radian;
    std::shared_ptr<Attributes> m_Attributes = std::make_shared<Attributes>();
    glm::vec2 m_UnitDirection;
    std::string m_ImagePath;
    std::vector<glm::vec2> m_Corners = {glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0)};
};

#endif //ATTACK_HPP

class Dart : public Attack {
public:
    explicit Dart(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Nail : public Attack {
public:
    explicit Nail(glm::vec2 position, glm::vec2 goal_position, int radius, std::shared_ptr<Attributes> attributes);
    void Move() override;
private:
    int m_Radius;
    glm::vec2 m_SourcePosition;
};

class Boomerang : public Attack {
public:
    explicit Boomerang(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationUnitDirection();
    void RotationImahe();
private:
    int isTurn = 0;
    int turn = 5;
    int rotation = 0;
    bool WillNotDisappear = true;
    glm::vec2 m_GoalPosition;
    glm::vec2 m_SourcePosition;
};

class Shuriken : public Attack {
public:
    explicit Shuriken(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    void RotationImahe();
private:
    int rotation = 0;
};

class Bomb : public Attack {
public:
    explicit Bomb(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Explosion : public Attack {
public:
    explicit Explosion(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attack> bomb, std::shared_ptr<Attributes> attributes);
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int existTime = 2;
    std::shared_ptr<Attack> m_Bomb;
};

class Airplane : public Attack {
public:
    explicit Airplane(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationImage(glm::vec2 position);
private:
    int time = 0;
    int max_Penetration;
    bool WillNotDisappear = true;
    glm::vec2 m_SourcePosition;
};

class Ray : public Attack {
public:
    explicit Ray(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Blizzard : public Attack {
public:
    explicit Blizzard(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int exit = 1;
};

class Rubber : public Attack {
public:
    explicit Rubber (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class MagicBall : public Attack {
public:
    explicit MagicBall (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};