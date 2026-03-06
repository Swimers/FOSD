#include "../include/DseApplication.h"
#include "../include/DseStructUtil.h"

int main(int argc, char* argv[]) {
    DseApplication::FOSD::Init([](const std::string& workmode, const std::string& previousWorkmode){});

    //添加自己的代码
    //...

    return DseApplication::run();
}
