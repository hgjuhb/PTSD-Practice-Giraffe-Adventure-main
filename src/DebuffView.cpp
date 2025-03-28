# include "DebuffView.hpp"

DebuffView::DebuffView(){};

void DebuffView::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void DebuffView::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void DebuffView::Update(const glm::vec2& Position, bool visable) {
    SetPosition(Position);
    SetVisible(visable);
}


Snow::Snow(){
    SetImage(GA_RESOURCE_DIR"/Debuff/Snow.png");
    SetVisible(false);
    SetZIndex(3);
};

Ice::Ice(){
    SetImage(GA_RESOURCE_DIR"/Debuff/Ice.png");
    m_Transform.scale = glm::vec2(0.8f, 0.8f);
    SetVisible(false);
    SetZIndex(3);
};

Mucus::Mucus(){
    SetImage(GA_RESOURCE_DIR"/Debuff/Rubber.png");
    SetVisible(false);
    SetZIndex(2);
};

RockNinjaDebuff::RockNinjaDebuff(){
    SetImage(GA_RESOURCE_DIR"/Debuff/RockNinja.png");
    SetVisible(false);
    SetZIndex(4);
};

