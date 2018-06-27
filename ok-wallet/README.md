### 编译

1. cd ok-wallet
2. mkdir build
3. cd build
4. cmake ..(for release version, run cmake .. -DCMAKE_BUILD_TYPE=Release)
5. ls *.so
6. ls *.dylib

### 注意项
- stellar-core的log输出功能已被关闭。如果要打开，请去除CMakeLists.txt文件中关于ELPP_DISABLE_LOGS的定义。
- 每次调用produceUnsignedTx(或Java_com_okcoin_vault_jni_stellar_Stellarj_execute中的"createrawtransaction命令")时，都会先去更新本地的账本数据库。如果本地库比较旧，会比较耗时。
