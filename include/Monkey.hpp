#pragma once
#ifndef MONKEY_HPP
#define MONKEY_HPP

#include <vector>
#include <string>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Attack.hpp"
#include "Balloon.hpp"
#include "Range.hpp"

class Monkey : public Util::GameObject {
public:

      explicit Monkey(glm::vec2 position);
      void SetPosition(const glm::vec2& Position);
      void SetImage(const std::string& ImagePath);
      void SetRadius(int radius);
      void SetRotation(glm::vec2 Position);
      void SetCd(int cd);
      void UpdateRange();
      void ResetCount();
      bool IsInside(glm::vec2 mousePosition);
      void CheckRangeVisible();
      void SetSize(glm::vec2 size);
      void SetRangeColor(bool is_placeable);
      bool Touched(Monkey& other);
      bool IsMonkeyInRectangle(glm::vec2 topLeft, glm::vec2 bottomRight);
      bool Placeable(std::vector<std::vector<std::vector<glm::vec2>>> Level_Placeable);

      [[nodiscard]] virtual std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition);
      [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
      [[nodiscard]] int GetRadius() const { return m_Radius; }
      [[nodiscard]] std::shared_ptr<Range> GetRange() const { return m_Range; }
      [[nodiscard]] virtual bool IsCollision(const std::shared_ptr<Balloon>& other) const;
      [[nodiscard]] bool Countdown();

private:
      std::string m_ImagePath;
      int m_Radius;
      glm::vec2 m_Size;
      int m_Count = 0;
      int m_Cd;
      std::shared_ptr<Range> m_Range = std::make_shared<Range>(m_Transform.translation, m_Radius);
};

#endif //MONKEY_HPP

class DartMonkey : public Monkey {
public:
      explicit DartMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};

class NailMonkey : public Monkey {
public:
      explicit NailMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};

class SniperMonkey : public Monkey {
public:
      explicit SniperMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};

class BoomerangMonkey : public Monkey {
public:
      explicit BoomerangMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};

class NinjaMonkey : public Monkey {
public:
      explicit NinjaMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};

class Cannon : public Monkey {
public:
      explicit Cannon(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};


class Airport : public Monkey {
public:
      explicit Airport(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
      [[nodiscard]] bool IsCollision(const std::shared_ptr<Balloon>& other) const override;
      glm::vec2 ProduceCoordinateByAngle(glm::vec2 position, float angle);
private:
      int airplane_num = 1;
      std::vector<std::shared_ptr<Attack>> m_Airplanes;
};

class BuccaneerMonkey : public Monkey {
public:
      explicit BuccaneerMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};

class SuperMonkey : public Monkey {
public:
      explicit SuperMonkey(glm::vec2 position);
      [[nodiscard]] std::vector<std::shared_ptr<Attack>> ProduceAttack(glm::vec2 goalPosition) override;
};