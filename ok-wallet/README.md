### 编译

1. cd ok-wallet
2. mkdir build
3. cd build
4. cmake ..(for release version, run cmake .. -DCMAKE_BUILD_TYPE=Release)
5. ls *.so
6. ls *.dylib

### 注意项
- stellar-core的log输出功能已被关闭。如果要打开，请去除CMakeLists.txt文件中关于ELPP_DISABLE_LOGS的定义。
