#pragma once
#ifndef BALLOON_HPP
#define BALLOON_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Attack.hpp"


class Balloon : public Util::GameObject {
public:

    explicit Balloon(std::vector<glm::vec2> coordinates);

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void SetHealth(int num);
    void SetStageHealth(int num);
    void SetSpeed(float num);
    void SetRectangleCorners();
    void SetRotation();
    void SetDistance(float distance);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetMoney(int money);
    void LoseHealth(int lose);
    void Move();
    virtual void Injured();

    [[nodiscard]] virtual std::vector<std::shared_ptr<Balloon>> Burst() const;

    [[nodiscard]] bool IsCollision(const std::shared_ptr<Attack>& other);
    [[nodiscard]] bool IsAlive() const { return m_Health > 0;};
    [[nodiscard]] bool IsArrive() const { return m_Coordinates.size() == 0;}


    [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] std::vector<glm::vec2> GetCoordinates() const { return m_Coordinates; }
    [[nodiscard]] int GetHealth() const { return m_Health; }
    [[nodiscard]] int GetStageHealth() const { return stage_Health; }
    [[nodiscard]] int GetMoney() const { return m_Money; }
    [[nodiscard]] float GetSpeed() const { return m_Speed; }
    [[nodiscard]] float GetDistance() const { return m_Distance; }
    [[nodiscard]] const std::string& GetImagePaths() const { return m_ImagePath; }
    [[nodiscard]] std::vector<glm::vec2> GetConers() const { return m_Corners; }

private:
    int m_Health = 0;
    int stage_Health = 0;
    float m_Speed = 0;
    float m_Distance = 0;
    float m_Radian = 0;
    int m_Width = 0;
    int m_Height = 0;
    int m_Money= 0;
    std::string m_ImagePath;
    std::vector<glm::vec2> m_Coordinates;
    std::vector<glm::vec2> m_Corners = {glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0)};
};

#endif // BALLOON_HPP

class RED : public Balloon {
public:
    explicit RED(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BLUE : public Balloon {
public:
    explicit BLUE(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class GREEN : public Balloon {
public:
    explicit GREEN(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class YELLOW : public Balloon {
public:
    explicit YELLOW(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class PINK : public Balloon {
public:
    explicit PINK(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BLACK : public Balloon {
public:
    explicit BLACK(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class WHITE : public Balloon {
public:
    explicit WHITE(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class PURPLE : public Balloon {
public:
    explicit PURPLE(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class ZEBRA : public Balloon {
public:
    explicit ZEBRA(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class IRON : public Balloon {
public:
    explicit IRON(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class RAINBOW : public Balloon {
public:
    explicit RAINBOW(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class CERAMICS : public Balloon {
public:
    explicit CERAMICS(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class MOAB : public Balloon {
public:
    explicit MOAB(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BFB : public Balloon {
public:
    explicit BFB(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class ZOMG : public Balloon {
public:
    explicit ZOMG(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class DDT : public Balloon {
public:
    explicit DDT(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BAD : public Balloon {
public:
    explicit BAD(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

