# 实施计划：NoahGameFrame CMake 构建系统统一升级

- [ ] 1. 重写根 [CMakeLists.txt](/Users/kemi/NoahGameFrame/CMakeLists.txt) 的头部与项目声明结构
   - 将 `cmake_minimum_required(VERSION 2.8)` 升级为 `cmake_minimum_required(VERSION 3.20)` 并置于文件首行
   - 把散落在头部的 `set_property(GLOBAL PROPERTY USE_FOLDERS On)`、`option(BUILD_TOOLS_ONLY ...)`、`option(BUILD_MID_WARE_SDK ...)`、`macro(link_NFSDK ...)` 全部移到 `cmake_minimum_required` 与 `project()` 之后
   - 将 `project(NoahFrame)` 升级为 `project(NoahFrame LANGUAGES CXX C)`
   - _需求：1.1、1.2、1.3、1.4、6.3_

- [ ] 2. 使用现代 C++ 标准声明替换根 CMakeLists 中的 `-std=c++17` 硬编码
   - 添加 `set(CMAKE_CXX_STANDARD 17)`、`set(CMAKE_CXX_STANDARD_REQUIRED ON)`、`set(CMAKE_CXX_EXTENSIONS OFF)`
   - 删除 `set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ...")` 中的 `-std=c++17` 片段
   - 删除针对非 APPLE UNIX 分支中的 `add_definitions(-std=c++17)` 全局定义
   - _需求：1.5_

- [ ] 3. 使用 `add_compile_options` / `if(MSVC)` 重写根 CMakeLists 的平台编译标志块
   - 将 UNIX 分支下的 `set(CMAKE_C_FLAGS ...)` / `set(CMAKE_CXX_FLAGS ...)` 拆分为 `add_compile_options(-fPIC -Wall ...)` 与 `add_compile_options($<$<CONFIG:Debug>:-O0;-g;-ggdb;-DDEBUG> $<$<CONFIG:Release>:-O3>)`
   - 将 Windows 分支下的 `SET(CMAKE_CXX_FLAGS "-D_X64 -D_WINDOWS /EHsc")` 改用 `if(MSVC) add_compile_definitions(_X64 _WINDOWS) add_compile_options(/EHsc) endif()`
   - 保留 `if(APPLE) set(CMAKE_SHARED_LIBRARY_SUFFIX ".so") endif()` 逻辑，但改到 `project()` 之后
   - _需求：1.6、4.1、4.3、4.4_

- [ ] 4. 更新根 CMakeLists 中的 `link_NFSDK` 宏使其使用 `PRIVATE` 关键字
   - 将宏体内 `target_link_libraries(${projName} NFCore NFMessageDefine NFNetPlugin)` 改为 `target_link_libraries(${projName} PRIVATE NFCore NFMessageDefine NFNetPlugin)`
   - 保留 `add_dependencies(${projName} NFNetPlugin NFCore NFMessageDefine)` 不变
   - _需求：2.6_

- [ ] 5. 修正根 CMakeLists 中 `option` 值判定的老式写法
   - 将 `if(${BUILD_TOOLS_ONLY} STREQUAL "ON")` 改为 `if(BUILD_TOOLS_ONLY)`；`if(${BUILD_MID_WARE_SDK} STREQUAL "ON")` 改为 `if(BUILD_MID_WARE_SDK)`
   - 保持三个分支下的 `add_subdirectory(...)` 调用顺序完全不变
   - _需求：3.3、6.3_

- [ ] 6. 升级 `NFComm/` 下全部子模块 CMakeLists（NFCore / NFPluginLoader / NFRenderPlugin / NFNetPlugin / NFKernelPlugin / NFConfigPlugin / NFLogPlugin / NFLuaScriptPlugin / NFMessageDefine / NFNoSqlPlugin / NFNavigationPlugin / NFTestPlugin / NFSecurityPlugin / NFBluePrintPlugin / NFActorPlugin）
   - 若文件顶部存在 `cmake_minimum_required` 或 `project()` 声明，全部移除
   - 将所有 `target_link_libraries(<tgt> <libs>)` 补充 `PRIVATE` 关键字（同一目标内保持一致，避免 signature 混用）
   - 将 `add_definitions(-DXXX)` 转为 `target_compile_definitions(<tgt> PRIVATE XXX)`；模块内的 `include_directories(...)` 转为 `target_include_directories(<tgt> PRIVATE ...)`
   - 特别处理 [NFComm/NFPluginLoader/CMakeLists.txt](/Users/kemi/NoahGameFrame/NFComm/NFPluginLoader/CMakeLists.txt) 中 40+ 条 `target_link_libraries` 全部统一为 `PRIVATE`
   - 保持所有目标名、`FOLDER` 属性、`ARCHIVE/RUNTIME/LIBRARY_OUTPUT_DIRECTORY` 设置不变
   - _需求：2.1、2.2、2.3、2.4、2.5、3.1、3.2_

- [ ] 7. 升级 `NFServer/` 下全部子模块 CMakeLists（18 个 plugin 目录）
   - 遍历 NFDBLogicPlugin、NFDBNet_ClientPlugin、NFDBNet_ServerPlugin、NFGameServerNet_ClientPlugin、NFGameServerNet_ServerPlugin、NFGameServerPlugin、NFLoginLogicPlugin、NFLoginNet_ClientPlugin、NFLoginNet_HttpServerPlugin、NFLoginNet_ServerPlugin、NFMasterNet_HttpServerPlugin、NFMasterNet_ServerPlugin、NFMasterServerPlugin、NFProxyLogicPlugin、NFProxyServerNet_ClientPlugin、NFProxyServerNet_ServerPlugin、NFWorldNet_ClientPlugin、NFWorldNet_ServerPlugin
   - 每个文件：删除多余 `cmake_minimum_required/project`（若有）、`target_link_libraries` 增补 `PRIVATE`、`add_definitions` 转为 `target_compile_definitions`
   - 保留 `link_NFSDK(<tgt>)` 调用不变
   - _需求：2.1、2.2、2.3、2.6、3.1、3.2_

- [ ] 8. 升级 `NFExamples/` 与 `Tutorial/` 下全部子模块 CMakeLists
   - NFExamples 子目录：NFChatPlugin、NFConsumeManagerPlugin、NFInventoryPlugin、NFServer
   - Tutorial 子目录：Tutorial1 ~ Tutorial7
   - 每个文件按需求 2 的规则做同样处理：`PRIVATE` 关键字、`target_compile_definitions` 转换、去除多余顶层声明
   - 保留 `add_definitions(-DWIN -D_USRDLL -DELPP_NO_DEFAULT_LOG_FILE)` 的语义（转为 `target_compile_definitions`）
   - _需求：2.1、2.2、2.3、2.5_

- [ ] 9. 升级 `NFTools/` 目录 CMakeLists，并保持第三方 zlib 不动
   - 处理 [NFTools/CMakeLists.txt](/Users/kemi/NoahGameFrame/NFTools/CMakeLists.txt) 与 [NFTools/NFFileProcess/CMakeLists.txt](/Users/kemi/NoahGameFrame/NFTools/NFFileProcess/CMakeLists.txt) 的现代化改造
   - **不修改** [NFTools/NFFileProcess/zlib/CMakeLists.txt](/Users/kemi/NoahGameFrame/NFTools/NFFileProcess/zlib/CMakeLists.txt)（属 zlib 上游源码）
   - _需求：2.1、2.2、5.1、5.2_

- [ ] 10. 自检与产物一致性核对
   - 扫描全部已修改的 `CMakeLists.txt`，确认不存在残留的 `cmake_minimum_required(VERSION 2.x)` 或 `-std=c++17` 硬编码
   - 逐目录核对：`add_library` / `add_executable` 的目标名、`OUTPUT_NAME_DEBUG`、`FOLDER`、`OUTPUT_DIRECTORY` 与升级前一一对应
   - 确认没有 C++ 源码（`.h/.cpp/.cc`）被误改，本次改动仅限于 `CMakeLists.txt`
   - 在关键修正处（如根 `option` 顺序、`link_NFSDK` 宏更新）添加 `# NOTE:` 注释
   - _需求：3.1、3.2、5.3、6.1、6.2、6.3_
