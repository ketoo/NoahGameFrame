# 需求文档：NoahGameFrame CMake 构建系统统一升级

## 引言

NoahGameFrame 工程目前包含 50+ 个 `CMakeLists.txt`，其中根目录 [CMakeLists.txt](/Users/kemi/NoahGameFrame/CMakeLists.txt) 使用了 `cmake_minimum_required(VERSION 2.8)`，且 `cmake_minimum_required()` 出现在了 `project()` 之前的错误顺序中。自 CMake 3.27 起，对 <3.5 版本的兼容策略会产生弃用错误，CMake 4.0 起彻底删除；同时工程中广泛存在 `add_definitions(-std=c++17)`、手写 `CMAKE_CXX_FLAGS`、无 `PUBLIC/PRIVATE` 的 `target_link_libraries` 等老旧写法。这些问题合力导致当前工程在较新版本 CMake 上"跑不起来"。

本次工作的目标：**扫描工程内所有 `CMakeLists.txt`，统一升级 CMake 的最低版本要求与相关写法到当前主流可用的最新版本**，使工程在 CMake 3.20+（含 4.x）下可以正常配置和生成，同时**不改变工程原有的目录结构、目标名和链接关系**。

工程内 `CMakeLists.txt` 分布概览（50+ 个）：

- 根目录：`/CMakeLists.txt`
- 二级目录：`NFComm/`、`NFServer/`、`NFExamples/`、`Tutorial/`、`NFTools/`
- 各插件 / 教程模块：`NFComm/NFCore`、`NFComm/NFPluginLoader`、`NFComm/NFRenderPlugin` ... `Tutorial/Tutorial1` 等
- 第三方：`NFTools/NFFileProcess/zlib/CMakeLists.txt`（**不在本次升级范围内**，属外部依赖代码）

## 需求

### 需求 1：统一根 CMakeLists.txt 的最低版本与结构

**用户故事：** 作为一名维护者，我希望根 `CMakeLists.txt` 能在最新版本的 CMake（包括 3.20 ~ 4.x）下正常工作，以便新加入的开发者可以直接使用其操作系统自带或最新的 CMake 完成配置。

#### 验收标准

1. WHEN 使用 CMake 3.20 或更高版本执行 `cmake ..` THEN 根 `CMakeLists.txt` SHALL 不再报出 "Compatibility with CMake < 3.5 has been removed" 一类错误或严重弃用警告。
2. WHEN 读取根 `CMakeLists.txt` THEN 该文件 SHALL 在文件第一条有效指令位置声明 `cmake_minimum_required(VERSION 3.20)`，且必须出现在 `project()` 之前。
3. WHEN 读取根 `CMakeLists.txt` THEN `project(NoahFrame)` 声明 SHALL 显式带上 `LANGUAGES CXX C`。
4. IF 当前根文件中同时存在 `option(...)`、`macro(...)`、`set_property(...)` 混杂在 `cmake_minimum_required` 之前 THEN 升级后的文件 SHALL 将它们全部移动到 `cmake_minimum_required` 与 `project()` 之后的合理位置。
5. WHEN 读取根 `CMakeLists.txt` THEN 文件 SHALL 使用 `set(CMAKE_CXX_STANDARD 17)`、`set(CMAKE_CXX_STANDARD_REQUIRED ON)`、`set(CMAKE_CXX_EXTENSIONS OFF)` 取代散落各处的 `add_definitions(-std=c++17)` 及 `-std=c++17` 硬编码。
6. WHEN 读取根 `CMakeLists.txt` THEN 平台相关的编译选项 SHALL 使用 `add_compile_options()` / `if(MSVC)` / `if(APPLE)` / `if(UNIX)` 组织，而不是直接覆盖 `CMAKE_CXX_FLAGS`。

### 需求 2：统一所有子目录 CMakeLists.txt 的写法

**用户故事：** 作为一名维护者，我希望所有子模块的 `CMakeLists.txt` 都遵循一致的、现代化的写法，以便阅读、修改和后续新增插件。

#### 验收标准

1. WHEN 遍历 `NFComm/*/CMakeLists.txt`、`NFServer/*/CMakeLists.txt`、`NFExamples/*/CMakeLists.txt`、`Tutorial/*/CMakeLists.txt` THEN 每个子 `CMakeLists.txt` SHALL 不再重复声明 `cmake_minimum_required` 或 `project()`（保持由根文件统一声明）。
2. WHEN 子模块中出现 `target_link_libraries(<tgt> <libs>)` 且未指定 `PUBLIC/PRIVATE/INTERFACE` THEN 升级后 SHALL 显式带上 `PRIVATE`（除非当前需要将头文件传递给依赖方，此时使用 `PUBLIC`）。
3. WHEN 子模块中原本使用 `add_definitions(-Dxxx)` 用于该目标 THEN 升级后 SHALL 使用 `target_compile_definitions(<tgt> PRIVATE xxx)`。
4. WHEN 子模块中原本使用 `include_directories(...)` 用于该目标 THEN 升级后 SHALL 使用 `target_include_directories(<tgt> PRIVATE ...)`；若某些 include 是全工程共享（如 `Dependencies/` 根路径），可保留在根 `CMakeLists.txt` 中。
5. WHEN 读取任一子 `CMakeLists.txt` THEN 该文件 SHALL 保持原有的 `add_library` / `add_executable` 目标名、`FOLDER` 属性、输出目录设置不变。
6. IF 子模块存在 `link_NFSDK(<tgt>)` 宏调用 THEN 升级后 SHALL 保留该宏的调用点，但宏本身在根文件里的实现 SHALL 更新为使用 `PRIVATE` 关键字。

### 需求 3：不破坏当前的目录树与目标名

**用户故事：** 作为一名依赖当前构建产物的开发者，我希望升级后所有生成的库/可执行文件名、输出目录和依赖关系保持完全一致，以便下游脚本、IDE 配置、部署工具无需改动。

#### 验收标准

1. WHEN 构建完成 THEN 生成的目标名（如 `NFCore`、`NFPluginLoader`、`Tutorial1` ...）SHALL 与升级前完全一致。
2. WHEN 构建完成 THEN 输出目录 `_Out/Debug` / `_Out/Release` 下的产物名称与升级前完全一致。
3. WHEN 使用 `BUILD_TOOLS_ONLY=ON` 或 `BUILD_MID_WARE_SDK=ON` 选项配置 THEN 各 `add_subdirectory` 分支 SHALL 保持原有语义。

### 需求 4：跨平台兼容性

**用户故事：** 作为一名同时使用 macOS / Linux / Windows 的开发者，我希望升级后的 CMake 脚本在三大平台上都能配置成功。

#### 验收标准

1. WHEN 在 macOS 上执行 `cmake ..` THEN 配置 SHALL 成功，`APPLE` 分支的 SDL2 / OpenGL 等 Framework 检索 SHALL 保持工作。
2. WHEN 在 Linux 上执行 `cmake ..` THEN 配置 SHALL 成功，使用 vcpkg `x64-linux` triplet。
3. WHEN 在 Windows 上使用 Visual Studio 生成器执行 `cmake ..` THEN 配置 SHALL 成功，MSVC 相关标志（`/EHsc`、`_X64`、`_WINDOWS`）SHALL 通过 `if(MSVC)` 分支设置。
4. WHEN 在 macOS 且 CMake >= 3.20 上配置 THEN 强制 `.so` 后缀这段旧逻辑 SHALL 保留，但通过 `if(APPLE)` 而不是过时写法生效。

### 需求 5：不修改的范围

**用户故事：** 作为一名维护者，我希望本次升级不触碰第三方源码内自带的 CMake 脚本，以便未来同步上游更方便。

#### 验收标准

1. WHEN 遍历工程 THEN [NFTools/NFFileProcess/zlib/CMakeLists.txt](/Users/kemi/NoahGameFrame/NFTools/NFFileProcess/zlib/CMakeLists.txt) SHALL 不被修改（属 zlib 上游源码）。
2. IF 文件位于 `Dependencies/` 目录下并且属于第三方原始来源 THEN 该文件 SHALL 不被修改。
3. WHEN 升级完成 THEN C++ 源代码（`.h/.cpp/.cc`）SHALL 全部保持不变，本次任务仅涉及 CMake 脚本。

### 需求 6：可验证性

**用户故事：** 作为一名维护者，我希望在升级完成后能快速自证"当前 CMake 工程能正常 configure"，以便建立信心。

#### 验收标准

1. WHEN 升级完成 THEN 根 `CMakeLists.txt` SHALL 在执行 `cmake -S . -B build` 时输出的 Warning 中 **不含** `CMP0000`、`CMake Deprecation Warning` 与 `Compatibility with CMake` 类信息。
2. WHEN 升级完成 THEN 每个被修改的 `CMakeLists.txt` 顶部 SHALL 保留原有的注释与目标语义（不删注释、不改语义）。
3. IF 升级过程中发现原文件的宏 / 逻辑错误（例如 `option()` 出现在 `cmake_minimum_required` 之前）THEN 应在对应 `CMakeLists.txt` 内以 `# NOTE:` 注释形式指出并同时修正。

## 边界与说明

- **CMake 目标版本选择**：采用 `cmake_minimum_required(VERSION 3.20)`，理由：3.20 覆盖 Ubuntu 22.04、Xcode 14、VS 2022 自带工具链；同时保持在 CMake 4.x 上 `Policy CMP0048/CMP0054/CMP0077` 等均已默认为 NEW，避免弃用报错。若用户要求提高到 3.25/3.28，属可调参数。
- **不涉及**：vcpkg 集成方式、CMakePresets.json、CI 配置的新增 —— 若需要，视为后续工作单独提出。
- **风险点**：`NFPluginLoader` 有大量 `target_link_libraries(...)` 调用未显式指定 keyword，一旦其中一处使用了 keyword 而其它没用，CMake 会报 "signature mismatch"，因此需要在同一目标上**全部**改为 `PRIVATE`。

