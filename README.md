
# FOSD( Fiber Optic Sensing Demodulator) 光纤传感解调仪上位机
> 接收光纤解调仪数据，进行应变解算、温度解耦，首先对应模型变形的三维可视化


## 📋 项目介绍
### 核心功能
- 功能1，实现与解调仪的USART、TCP/UDP连接通信控制，接收解算解调仪采集数据;
- 功能2，实现模型的显示和形变的实时显示;

### 技术栈
- 开发语言：C++ 11 / C
- 核心框架/库：VTK(8.2.0) / Qt(Qt 5.12.2)
- 开发工具：Qt Creater（4.8.2）
- 运行环境：Win 10+


## 🛠️ 环境要求与依赖安装
### 环境要求
- Qt 5.12.2；Cmake 3.17.1；VS 2017；VTK 8.2.0
- 内存：建议 2GB 及以上
- 存储空间：至少 200MB（不含测试数据）

### 依赖安装步骤
> To be continue

## 🚀 快速启动
### 基础功能示例 
> To be continue

### 配置参数说明
| 参数名 | 类型 | 说明 | 默认值 |
|--------|------|------|--------|
| \ | \ | \ | \ |


## 📁 项目目录结构
> FOSD-Pro 后版本开始沿用下述项目结构
```
{{项目仓库名}}/
├── bulid/              # 编译构建结果(Debug|Release在统一目录下)
|   ├── bin/        
|   ├── generated/
|   ├── obj/
|   └── Makefile 
├── config/             # 软件配置文件
├── lib/                # 软件依赖核心库
|   └── VTK/            # VTK的Debug|Release Win版本编译结果
├── res/                # 资源库
├── src/                # 核心源代码目录
│   ├── main
│   ├── Connect         # 通信界面
│   ├── Communication   # 通信接口函数
│   ├── Dependence      # 依赖计算函数
│   ├── Disp2D          # 二维显示界面
│   ├── Disp3D          # 三维显示界面
│   └── UserManage      # 用户管理
├── FOSD-Pro.pro        # Qmake编译配置
└── README.md           # 项目说明文档（本文档）
```

## 🤝 贡献指南
欢迎各位开发者贡献代码，助力项目完善！贡献流程如下：
1. Fork 本仓库（点击 GitHub 页面右上角 Fork 按钮）
2. 克隆 Fork 后的仓库到本地：`git clone https://github.com/你的用户名/{{项目仓库名}}.git`
3. 创建特性分支：`git checkout -b feature/{{特性名称，如：add-excel-export}}`
4. 编写代码并提交：`git commit -m "feat: 新增Excel导出功能"`
5. 推送分支到远程：`git push origin feature/{{特性名称}}`
6. 打开 Pull Request（PR）：在 GitHub 本仓库页面提交 PR，描述清楚修改内容

### 编码规范
- 提交信息格式：`类型: 描述`（类型：feat-新功能，fix-修复bug，docs-文档更新，style-代码格式调整）

## 🙏 致谢
- 感谢 VTK提供的技术支持
- 感谢 师兄楚辉超、凌立瞳的指导和帮助

## 📞 联系方式（可选）
- 作者：SuningJan
- 邮箱：1229419727@qq.com
- 项目地址：https://github.com/Swimers/FOSD_Project
