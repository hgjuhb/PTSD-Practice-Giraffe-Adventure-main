#pragma once

#include "Util/Input.hpp"
#include <memory>
#include <string>
#include "Util/GameObject.hpp"
#include "Monkey.hpp"

class DragButton : public Util::GameObject {

public:
    DragButton(const glm::vec2& position);
    void Update();
    void SetImage(const std::string& ImagePath);
    void SetPosition(const glm::vec2& position);
    // 设置和获取按钮的尺寸
    void SetSize(const glm::vec2& size) { m_Size = size; }
    const glm::vec2& GetSize() const { return m_Size; }
    
    // 设置图片旋转角度（0-360度）
    void SetAngle(int degrees);
    // 获取当前角度（0-360度）
    int GetAngle() const;
    
    // 检查点是否在按钮范围内
    bool IsPointInside(const glm::vec2& point) const;
    [[nodiscard]] virtual std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition);
    
private:
    glm::vec2 m_Size{100.0f, 100.0f}; // 默认尺寸
    float m_HoverScale{1.1f};  // 新增：悬停时的放大倍数
    float m_HoverAnimTime{0.0f};  // 新增：悬停动画时间
    glm::vec2 m_OriginalSize{100.0f, 100.0f};  // 新增：原始尺寸
}; 


class DartMonkeyButton : public DragButton {
public:
      explicit DartMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};

class NailMonkeyButton : public DragButton {
public:
      explicit NailMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};

class SniperMonkeyButton : public DragButton {
public:
      explicit SniperMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};

class BoomerangMonkeyButton : public DragButton {
public:
      explicit BoomerangMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};

class NinjaMonkeyButton : public DragButton {
public:
      explicit NinjaMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};
class CannonButton : public DragButton {
public:
      explicit CannonButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};
class AirportButton : public DragButton {
public:
      explicit AirportButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};
class BuccaneerMonkeyButton : public DragButton {
public:
      explicit BuccaneerMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};

class SuperMonkeyButton : public DragButton {
public:
      explicit SuperMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
};