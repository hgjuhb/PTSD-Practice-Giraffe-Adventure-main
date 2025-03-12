#ifndef INFORMATIONBOARD_HPP
#define INFORMATIONBOARD_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "InformationImage.hpp"

class InformationBoard : public Util::GameObject {
public:

    explicit InformationBoard();

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void SetUpgrateName(std::string name);
    void UpdateAllObjectVisible(bool isClicked);
    void UpdateUpgrade();
    void SetUpgradeCost(std::vector<std::vector<int>> upgradeCost);

    [[nodiscard]] std::vector<int> IsClick(glm::vec2 mousePosition, int money);
    [[nodiscard]] int GetLevel() { return level;}
    [[nodiscard]] int GetUpgradePath() { return upgradePath;}
    [[nodiscard]] std::shared_ptr<InformationImage> GetMonkeyImage(){ return monkey_image;}
    [[nodiscard]] std::shared_ptr<InformationImage> GetUpgrateImage1(){ return upgrate_image1;}
    [[nodiscard]] std::shared_ptr<InformationImage> GetUpgrateImage2(){ return upgrate_image2;}
    [[nodiscard]] std::vector<std::shared_ptr<GameObject>> GetAllChildren();

private:
    int level = 0;
    int upgradePath = 0;
    std::vector<std::vector<int>> m_upgradeCost = {};
    std::string upgrateName = GA_RESOURCE_DIR"//MonkeyInformation/DartMonkey";
    std::string m_ImagePath;
    std::shared_ptr<InformationImage> monkey_image = std::make_shared<InformationImage>(GA_RESOURCE_DIR"/MonkeyInformation/WithoutImage.png", glm::vec2(-500, 155));
    std::shared_ptr<InformationImage> upgrate_image1 = std::make_shared<InformationImage>(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey_1_1.png", glm::vec2(-530, 0));
    std::shared_ptr<InformationImage> upgrate_image2 = std::make_shared<InformationImage>(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey_2_1.png", glm::vec2(-530, -125));
};

#endif //INFORMATIONBOARD_HPP

class DartMonkeyInformationBoard : public InformationBoard {
public:
    explicit DartMonkeyInformationBoard();
};
