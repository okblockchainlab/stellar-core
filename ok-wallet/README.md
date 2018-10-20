### 编译

##### centos7的编译依赖项
```shell
sudo yum install autoconf automake libtool bison flex gcc
sudo yum install glibc-static libstdc++-static
```
项目中用到了c++14标准，但centos的源上的gcc一般版本都比较低不支持c++14，所以有可能需要下载gcc源码手工编译安装。


##### 编译步骤
- git clone https://github.com/okblockchainlab/stellar-core.git
- cd stellar-core
- mkdir depslib
- export COIN_DEPS=\`pwd\`/depslib
- ./build.sh (only run this script if you first time build the project)
- ./runbuild.sh

### 注意项
- 生成raw tx时需要from账号的sequence number。需求方法参考：https://www.stellar.org/developers/horizon/reference/endpoints/accounts-single.html
- stellar-core的log输出功能已被关闭。如果要打开，请去除CMakeLists.txt文件中关于ELPP_DISABLE_LOGS的定义。
- commitTransaction函数将交易提交后，会一直运行，不会退出。目的是为了给stellar系统足够的时间将交易广播出去，使交易真正生效。因此当运行test_okwallet测试程序时，程序不会主动退出，除非按下ctrl+C键。
