### 编译

1. cd ok-wallet
2. mkdir build
3. cd build
4. cmake ..(for release version, run cmake .. -DCMAKE_BUILD_TYPE=Release)
5. ls *.so
6. ls *.dylib

### 注意项
- **commitTransaction函数将交易提交后，会一直运行，不会退出**。目的是为了给stellar系统足够的时间将交易广播出去，使交易真正生效。因此当运行test_okwallet测试程序时，程序不会主动退出，除非按下ctrl+C键。
- 多长时间才能将一个交易广播出去真正生效呢？我试过两分钟是不行的，需要更长时间。（有一次我晚上把test_okwallet程序跑起来忘了停止了，第二天早上发现交易生效了......）
- stellar-core的log输出功能已被关闭。如果要打开，请去除CMakeLists.txt文件中关于ELPP_DISABLE_LOGS的定义。
- 每次调用produceUnsignedTx(或Java_com_okcoin_vault_jni_stellar_Stellarj_execute中的"createrawtransaction命令")时，都会先去更新本地的账本数据库。如果本地库比较旧，会比较耗时。
- 在调用模块时，如果在数据库目录下没有标志文件“.${net_type}_db_initialized”文件，则认为数据库没有初始化，模块会自动初始化数据库，并在成功后生成标志文件。因此如果想要重新初始化数据库，记得删除".${net_type}_db_initialized"文件。
