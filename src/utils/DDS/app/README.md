本示例并没有实际运行效果，主要目的是展示：
1.main函数的基本结构
2.CMakeLists.txt文件的配置方法

编译及执行方法：
1.编译环境：gcc(版本推荐7及以上)和cmake(版本推荐3.5及以上)
2.当前文件夹打开终端
3.新建build文件夹：
    mkdir build
4.进入build文件夹：
    cd build
5.执行cmake命令：
    cmake ..
6.执行make命令：
    make
7.执行make install命令：
    make install
8.main.cpp同级路径下生成了一个名为install的文件夹，打开文件夹可以看到FOSD文件
9.进入install目录：
    cd ../install
10.加载RTIDDS和dseapp的环境变量：
    . /usr/local/rtidds/rtidds_env
    . /usr/local/dseapp/dseapp_env
11.运行FOSD：
    ./FOSD
