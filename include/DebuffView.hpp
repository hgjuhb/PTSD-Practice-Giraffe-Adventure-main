#pragma once
#ifndef DEBUFFVIEW_HPP
#define DEBUFFVIEW_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class DebuffView : public Util::GameObject {
public:

    explicit DebuffView();
    void SetImage(const std::string& ImagePath);
    void SetPosition(const glm::vec2& Position);
    void Update(const glm::vec2& Position, bool visable);

private:
    std::string m_ImagePath;
};

#endif //DEBUFFVIEW_HPP

class Snow : public DebuffView {
public:
    explicit Snow();
};

class Ice : public DebuffView {
public:
    explicit Ice();
};

class Mucus : public DebuffView {
public:
    explicit Mucus();
};
