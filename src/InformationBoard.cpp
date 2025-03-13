#include "InformationBoard.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

InformationBoard::InformationBoard(){
    SetImage(GA_RESOURCE_DIR"/MonkeyInformation/InformationBoard_test.png");
    SetPosition(glm::vec2(-450, 0));
    SetVisible(false);
    SetZIndex(5);
 };

void InformationBoard::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void InformationBoard::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

std::vector<std::shared_ptr<Util::GameObject>> InformationBoard::GetAllChildren(){
    std::vector<std::shared_ptr<Util::GameObject>> children =
        {monkey_image, upgrate_image1, upgrate_image2, upgrate_text1, upgrate_text2, upgrate_button1, upgrate_button2, sale_button};
    std::vector<std::shared_ptr<Util::GameObject>> tmp;
    tmp = upgrate_button1->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    tmp = upgrate_button2->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    tmp = sale_button->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    return children;
};

std::vector<int> InformationBoard::IsClick(glm::vec2 mousePosition, int money) {
    if (mousePosition.x > -625 && mousePosition.x < -275 && mousePosition.y > -250 && mousePosition.y < 250) {
        //關閉選取
        if (mousePosition.x > -307 && mousePosition.x < -279 && mousePosition.y > 221 && mousePosition.y < 247) {
            return {2};
        }
        if (mousePosition.x > -442 && mousePosition.x < -292 && mousePosition.y > -238 && mousePosition.y < -197) {
            return {3};
        }
        if (level < 4 && money >= m_upgradeCost[0][level] && upgradePath != 2 && mousePosition.x > -443 && mousePosition.x < -294 && mousePosition.y > -55 && mousePosition.y < -14) {
            level += 1;
            upgradePath = 1;
            UpdateUpgrade();
            return {4, m_upgradeCost[0][level-1]};
        }
        if (level < 4 && money >= m_upgradeCost[1][level] && upgradePath != 1 && mousePosition.x > -444 && mousePosition.x < -297 && mousePosition.y > -184 && mousePosition.y < -141) {
            level += 1;
            upgradePath = 2;
            UpdateUpgrade();
            return {5, m_upgradeCost[1][level-1]};
        }
        return {1};
    }
    return {0};
}

void InformationBoard::SetUpgradeCost(std::vector<std::vector<int>> upgradeCost) {
    m_upgradeCost = upgradeCost;
}

void InformationBoard::SetUpgradeText(std::vector<std::vector<std::string>> upgradeText) {
    m_upgradeText = upgradeText;
}

void InformationBoard::SetUpgrateName(std::string name) {
    upgrateName = name;
}

void InformationBoard::UpdateAllObjectVisible(bool isClicked) {
    monkey_image -> SetVisible(isClicked);
    upgrate_image1 -> SetVisible(isClicked);
    upgrate_image2 -> SetVisible(isClicked);
    upgrate_text1 -> SetVisible(isClicked);
    upgrate_text2 -> SetVisible(isClicked);
    upgrate_button1 -> UpdateVisible(isClicked);
    upgrate_button2 -> UpdateVisible(isClicked);
    sale_button -> UpdateVisible(isClicked);
}

void InformationBoard::UpdateUpgrade() {
    if (level == 4) {
        if (upgradePath == 1){upgrate_text1 -> Update("Level Max");}
        else{upgrate_text2 -> Update("Level Max");}
        upgrate_image1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_image2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_button1 -> Update("None");
        upgrate_button2 -> Update("None");
    }
    else if (upgradePath == 1) {
        upgrate_image1 -> SetImage(upgrateName + "_1_" + std::to_string(level+1) + ".png");
        upgrate_image2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_text1 -> Update(m_upgradeText[0][level]);
        upgrate_text2 -> Update("next path");
        upgrate_button1 -> Update(std::to_string(m_upgradeCost[0][level]));
        upgrate_button2 -> Update("None");
    }
    else {
        upgrate_image2 -> SetImage(upgrateName + "_2_" + std::to_string(level+1) + ".png");
        upgrate_image1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_text2 -> Update(m_upgradeText[1][level]);
        upgrate_text1 -> Update("next path");
        upgrate_button2 -> Update(std::to_string(m_upgradeCost[1][level]));
        upgrate_button1 -> Update("None");
    }
}

void InformationBoard::IsButtonTouch(glm::vec2 mousePosition) {
    upgrate_button1 -> IsTouch(mousePosition);
    upgrate_button2 -> IsTouch(mousePosition);
    sale_button -> IsTouch(mousePosition);
}

DartMonkeyInformationBoard::DartMonkeyInformationBoard() {
    SetUpgradeCost({{90, 120, 500, 1500, 2210},
                       {140, 170, 330, 8000, 8640}});
    SetUpgradeText({{"Long Range Darts", "Enhanced Eyesight", "Spike-o-pult", "Juggernaut"},
                          {"Sharp Shots", "Razor Sharp Shots", "Triple Darts", "Super Monkey Fan Club"}});
    auto monkeyImage  = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey");
    auto upgradeImage1  = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey_1_1.png");
    auto upgradeImage2  = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey_2_1.png");
    auto upgradeText1  = GetUpgrateText1();
    upgradeText1 -> Update("Long Range Darts");
    auto upgradeText2  = GetUpgrateText2();
    upgradeText2 -> Update("Sharp Shots");
    auto upgradeButton1  = GetUpgrateButton1();
    upgradeButton1 -> Update("90");
    auto upgradeButton2  = GetUpgrateButton2();
    upgradeButton2 -> Update("140");
}