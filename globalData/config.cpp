#include "config.h"

#include "json.hpp"
#include <fstream>



Config &Config::getInstance()
{
    static Config instance("config.json");
    return instance;
}

bool Config::loadFromFile()
{
    std::unique_lock<std::shared_mutex> lock(configMutex);
    std::ifstream in(filename);
    if (!in.is_open()) return false;
    nlohmann::json j;
    in >> j;
    in.close();

    DataShowFrep = j.value("DataShowFrep", 0);
    DataSaveFrep = j.value("DataSaveFrep", 0);
    filterCoeff = j.value("filterCoeff", 1.0f);
    order = j.value("order", std::vector<int>{});
    para = j.value("para", std::vector<double>{});

    ModelFlushFrep = j.value("ModeFlushFrep", 0);
    // std::vector<int> tempFlag = j.value("flagPoint", std::vector<int>{});
    // flagPoint.clear();
    // flagPoint.insert(tempFlag.begin(), tempFlag.end());
    flagPoint1.clear();
    if (j.find("flagPoint1") != j.end()) {
        for (nlohmann::json::iterator it = j["flagPoint1"].begin(); it != j["flagPoint1"].end(); ++it) {
            int id = std::stoi(it.key());
            const nlohmann::json& val = it.value();
            if (val.is_array() && val.size() == 3) {
                flagPoint1[id] = {val[0], val[1], val[2]};
            }
        }
    }
    pointSize = j.value("pointSize", 1.0f);
    ColorMode = j.value("ColorMode", 0);
    minRange = j.value("minRange", 0.0f);
    maxRange = j.value("maxRange", 1.0f);

    return true;
}

bool Config::saveToFile()
{
    std::shared_lock<std::shared_mutex> lock(configMutex);

    // 保存配置到文件
    nlohmann::json j;
    j["DataShowFrep"] = DataShowFrep;
    j["DataSaveFrep"] = DataSaveFrep;
    j["filterCoeff"] = filterCoeff;
    j["order"] = order;
    j["para"] = para;

    j["ModeFlushFrep"] = ModelFlushFrep;
    // j["flagPoint"] = std::vector<int>(flagPoint.begin(), flagPoint.end());
    nlohmann::json fp1;
    for (std::unordered_map<int, Point3f>::const_iterator it = flagPoint1.begin(); it != flagPoint1.end(); ++it) {
        int id = it->first;
        const Point3f& pt = it->second;
        fp1[std::to_string(id)] = {pt.x, pt.y, pt.z};
    }
    j["flagPoint1"] = fp1;
    j["pointSize"] = pointSize;
    j["ColorMode"] = ColorMode;
    j["minRange"] = minRange;
    j["maxRange"] = maxRange;

    std::ofstream out(filename);
    if (!out.is_open()) return false;
    out << j.dump(4);
    out.close();
    return true;
}







