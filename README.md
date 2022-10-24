# device_beken

#### 介绍
device_beken平台基于beken 7235芯片, 集wifi/ble功能于一体

#### 软件架构
app ==> func ==> driver



#### 安装教程

1.  在本地创建一个用于存放源码的目录，进入该目录
2.  输入以下命令初始化代码仓库 : 
	repo init -u https://gitee.com/openharmony-sig/manifest.git -b master --no-repo-verify -m devboard_beken.xml
3.  输入repo sync -c下载所有代码到本地
4.  输入hb set, 选择beken下的wifi_bk7235产品
6.  编译结果为out/bk7235x/wifi_bk7235/bk7235_sdk_ota.bin

#### 使用说明

1.  使用tools/目录下的bk_writer工具, bk7235_sdk_ota.bin下载到bk7235开发板中
2.  给bk7235开发板重新上电, 系统启动后进行命令行
3.  在命令行中输入help查看可用命令

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


