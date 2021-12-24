# bk7231_freertos_full_code



## 当前repository中submodule所在分支

| submodule | branch |
| --------- | ------ |
| beken378  | master |

> clone代码后，需执行如下命令：

```shell
cd beken378
git checkout master
```

> 如果不确定submodule所在分支，可用如下命令查找并手动确认

```shell
cd beken378
git branch -r --contains CommitID
```



## 代码管理策略 

随着项目的日益增多，决定使用`submodule+branch`的方式来管理
1. 每类项目单独创建一个`repository`
2. 同类项目用分支区分不同产品、不同代理商、不同客户
3. `beken378`使用`submodule`关联，`beken378`包含协议栈源码，相当于`fullcode`，发布时可使用脚本自动剔除协议栈等源码
4. 在项目稳定前，可直接使用`submodule`主分支，如果`submodule`有重大功能合入，可提前创建项目分支
5. 在提交代码时，不同目的的修改分开提交，便于向其它分支合入
6. 在提交代码后，如果需要将修改合入其它分支，使用`git cherry-pick commitid`
7. 在版本发布后，使用`git tag -a -m "notes" tag_name`创建标签，并用`git push --tags`提交



## submodule使用说明

### 创建submodule
1. `git submodule add http://192.168.0.6/wifi/beken_wifi_sdk.git beken378`
2. 修改`.gitmodules`中的`url`为相对路径，便于`ssh`和`http`同时使用
3. `git add .gitmodules beken378`
4. `git commit -m "xxx"`

注：
1. 所有`submodules`可能使用不同的分支，建议使用如下命令，指定所有`submodules`（包括主`repository`）的分支
`git branch --set-upstream-to=origin/xxx master`



### 获取submodule

> 如果使用`http`而不是`ssh`获取代码，请先运行如下命令(将用户名密码保存到本地文件中，解决`submodule`获取时无法访问的问题)
>
> `git config --global credential.helper store`



1. `git clone --recurse-submodules http://xxx.git`
    或者
2. `git clone http://xxx.git`
3. `cd xxx`
4. `git submodule update --init --recursive`



### 更新submodule
可以在`repository`根目录使用`git pull --recurse-submodules`命令更新整个工程
当然也可以进入特定submodule通过`git pull`单独更新

推荐使用前者更新，因为更新后`submodules`仍然停留在主`repository`关联的`commit`，不会主动切换到最新`commit`



### 提交submodule

需要先在`submodule`中提交修改，然后在主`repository`中更新`submodule`
1. `cd beken378` (the submodle name)
2. `git add xxx`
3. `git commit -m ""`
4. `git push origin xxx:yyy`
5. cd .. (the root of repo)
6. `git add beken378`
7. `git commit -m ""`
8. `git push origin`



## Build with dynamic configure

针对目前多系统、多平台、多项目同时运行的情况，为了使用统一基线代码，制定了一种针对多种编译方式可内嵌的动态编译机制，基本原理为：编译时指定目标参数，触发配置文件生成脚本来选择相应的配置文件，随后根据所选择的配置文件编译。

编译动态配置文件如下

```
├───generate_sys_config.bat
├───generate_sys_config.sh
├───Makefile
└───config
    ├───sys_config_bk7231.h
    ├───sys_config_bk7231u.h
    ├───sys_config_bk7231n.h
    ├───sys_config_bk7251.h
    └───sys_config.h
```



| 文件                  | 说明                                                         |
| --------------------- | ------------------------------------------------------------ |
| Makefile              | 主编译文件                                                   |
| generate_sys_config.* | 生成编译配置脚本，根据传入配置名称更新编译配置文件(sys_config.h) |
| config                | 编译配置目录，包含当前所有项目的编译配置文件                 |





#### Build with Makefile

针对使用Makefile的编译系统，如FreeRTOS，修改主Makefile如下：

```makefile
OS := $(shell uname)

ifeq ($(findstring MINGW32_NT, $(OS)), MINGW32_NT) 
define update_sys_config_if_needed
	generate_sys_config.bat $(1)
endef
else ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
define update_sys_config_if_needed
	generate_sys_config.bat $(1)
endef
else
define update_sys_config_if_needed
	./generate_sys_config.sh $(1)
endef
endif

%:
	$(call update_sys_config_if_needed, $@)
```



用法如下：

- Install arm-gcc tool-chain and add path to system environment
- update `make.exe` in rt-thread env with `tool\make\make.exe` of project directory if you want to use parallel compiling with it
- make target [-j6]


```shell
>make help
Usage of build with Beken SDK with Makefile

ip              build wifi library
ble             build ble  library
clean           clean project
bk7231          build bk7231 default project
bk7231u         build bk7231u default project
bk7231n         build bk7231n default project
bk7251          build bk7251 default project
```

```shell
make bk7231u -j8
make bk7251
make bk7231 -j6
make ip
make ble
make clean
```

#### Build with Scons

针对`rt-thread`平台，更新`rt-thread/tools/building.py`支持`--beken=BEKEN_TARGET`参数， 并调用`generate_sys_config`脚本更新`sys_config.h`文件，修改如下：

```diff
diff --git a/rt-thread/tools/building.py b/rt-thread/tools/building.py
index d58160f..479b9b7 100644
--- a/rt-thread/tools/building.py
+++ b/rt-thread/tools/building.py
@@ -204,6 +204,16 @@ def PrepareBuilding(env, root_directory, has_libcpu=False, remove_components = [
                 default = False,
                 help = 'print verbose information during build')

+    AddOption('--beken',
+                      dest = 'beken',
+                      type = 'string',
+                      help = 'set beken project: tuya/xindalu/etc...')
+    beken_target = GetOption('beken')
+    if sys.platform == 'win32':
+        os.system("generate_sys_config.bat " + beken_target)
+    else:
+        os.system("generate_sys_config.sh " + beken_target)
+
     Env = env
     Rtt_Root = os.path.abspath(root_directory)
     # set RTT_ROOT in ENV

```



用法如下：

```shell
scons --beken=bk7231u -j6
```



#### Build with aos



#### Add target

当前config目录预置了4个平台的缺省配置文件，如果需要新增配置文件，修改步骤如下：

1. 在config目录中增加配置文件，名称自定义，建议为sys_config_xxx.h
2. 修改generate_sys_config.bat，定义目标名称，如：
```shell
...
if "%1" equ "xiaoya" (
set new_sys_config=config\sys_config_bk7251_xiaoya.h
goto update_sys_config
)

set new_sys_config=config\%1.h
```

3. 修改generate_sys_config.sh，同上
```shell
case $1 in
	...
	xiaoya)
		new_sys_config=config/sys_config_bk7251_xiaoya.h
		;;
	*)
		new_sys_config=config/$1.h
esac
```

保存上述修改，可以使用如下命令使用新增加的配置

```shell
make xiaoya -j6
```



## 版本发布 

在发布版本时，`wifi/ble stack`中的代码编译以库的形式打包，然后对代码打标签并发布，方法如下

- 根据项目实际情况修改`sys_config.h`中的`CFG_SOC_NAME`，并返回项目根目录

编译库文件
- `scons --buildlib=beken_ip` 编译结束后会根据`CFG_SOC_NAME`重命名为`beken378/ip/libip72xx.a`
- `scons --buildlib=beken_ble` 编译结束后会重命名为`beken378/driver/ble/libble.a`

编译好库并提交代码以后，需要打标签，由于`submodule`本身就是以`commit id`的形式存在，无需单独打标签。
（当然，如果为了方便查找，可以用相同名称在相应代码库中打标签。）

- `git tag -a -m "" tag_name`
- `git push --tags`

运行脚本打包文件并发布
- run `release_tool.bat`
