# device_beken

#### 介绍
device_beken平台基于beken 7231芯片, 集wifi/ble功能于一体

#### 软件架构
app ==> func ==> driver


#### 安装教程

1.  在本地创建一个用于存放源码的目录，进入该目录
2.  输入以下命令初始化代码仓库 : 
	repo init -u https://gitee.com/openharmony-sig/manifest.git -b master --no-repo-verify -m devboard_beken.xml
3.  输入repo sync -c下载所有代码到本地
4.  输入hb set, 选择beken下的wifi_iot产品
5.  输入hb build -f --patch进行编译
6.  编译结果为out/bk7231/wifi_iot/bk7231_sdk_crc.bin

#### 使用说明

1.  使用tools/目录下的bk_writer工具, 将bk7231_sdk_crc.bin下载到bk7231开发板中
2.  给bk7231开发板重新上电, 系统启动后进行命令行
3.  在命令行中输入help查看可用命令

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
