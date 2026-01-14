#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <set>
#include <unordered_map>
#include <utility>
#include <shared_mutex>


struct Point3f {
    double x, y, z;
};

class Config
{
public:
    // 获取单例实例
    static Config& getInstance();

    // 加载配置文件
    bool loadFromFile();
    // 保存配置到文件
    bool saveToFile();

    // Getter / Setter 接口
    // 数据显示频率
    int getDataShowFrep() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return DataShowFrep;
    }
    void setDataShowFrep(int value){
        std::unique_lock<std::shared_mutex> lock(configMutex);
        DataShowFrep = value;
    }

    // 数据保存频率
    int getDataSaveFrep() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return DataSaveFrep;
    }
    void setDataSaveFrep(int value) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        DataSaveFrep = value;
    }

    // 滤波系数
    float getFilterCoeff() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return filterCoeff;
    }
    void setFilterCoeff(float val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        filterCoeff = val;
    }

    // 应变顺序数组
    std::vector<int> getOrder() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return order;
    }
    void setOrder(const std::vector<int>& val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        order = val;
    }

    // 应变系数数组
    std::vector<double> getPara() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return para;
    }
    void setPara(const std::vector<double>& val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        para = val;
    }

    // 模型刷新频率
    int getModelFlushFrep() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return ModelFlushFrep;
    }
    void setModelFlushFrep(int value) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        ModelFlushFrep = value;
    }

    // 关键点小球数组
    // std::vector<int> getFlagPoint() const { return std::vector<int>(flagPoint.begin(), flagPoint.end()); }
    // void setFlagPoint(const std::vector<int>& val) {
    //    flagPoint.clear();
    //    flagPoint.insert(val.begin(), val.end());
    // }
    // void addFlagPoint(int val) { flagPoint.insert(val); }
    // void deleteFlagPoint(int val) { flagPoint.erase(val); }
    std::unordered_map<int,Point3f> getFlagPoint1() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return flagPoint1;
    }
    void setFlagPoint1(const std::unordered_map<int, Point3f>& val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        flagPoint1 = val;
    }
    void addFlagPoint1(int id,const Point3f& pt) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        flagPoint1[id] = pt;
    }
    void deleteFlagPoint1(int id) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        flagPoint1.erase(id);
    }


    // 关键点小球大小
    float getPointSize() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return pointSize;
    }
    void setPointSize(float val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        pointSize = val;
    }

    // 颜色映射模式
    int getColorMode() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return ColorMode;
    }
    void setColorMode(int val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        ColorMode = val;
    }

    // 颜色映射最小值
    float getMinRange() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return minRange;
    }
    void setMinRange(float val){
        std::unique_lock<std::shared_mutex> lock(configMutex);
        minRange = val;
    }

    // 颜色映射最大值
    float getMaxRange() const {
        std::shared_lock<std::shared_mutex> lock(configMutex);
        return maxRange;
    }
    void setMaxRange(float val) {
        std::unique_lock<std::shared_mutex> lock(configMutex);
        maxRange = val;
    }

private:
    Config(const std::string& file) : filename(file) {}
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

private:
    std::string filename;
    // 解调仪模块
    int DataShowFrep;
    int DataSaveFrep;
    // 算法的模块
    float filterCoeff;
    std::vector<int> order;     //顺序数组
    std::vector<double> para;   //系数数组

    // 三维模块
    int ModelFlushFrep;
    std::set<int> flagPoint;        // 这个存关键点保存项
    std::unordered_map<int,Point3f> flagPoint1;
    float pointSize;                // 关键点小球大小
    int ColorMode;                  // 映射模式
    float minRange;                 // 颜色映射最小值
    float maxRange;                 // 颜色映射最大值


    mutable std::shared_mutex configMutex;  // 只要保证在同一个线程中读写就不需要用锁，但是懒得改，建议把整个代码重构，避免多个线程读写情况，加锁对性能太不友好
};

#endif // CONFIG_H
