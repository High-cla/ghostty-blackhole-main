# Debug State — Blackhole v7: UI 优化 (复制/粘贴/保存/加载)

## 当前状态
### 编译 ✓ - 零警告零错误

## 文件结构
| 文件 | 说明 |
|------|------|
| src/main.cpp | 主程序 |
| src/gui_config.h | GUI 头文件 + 函数声明 |
| src/gui_config.cpp | GUI 实现 + clipboard + save/load |
| blackhole_presets.txt | 预设配置文件(程序运行时自动创建) |

---

## 新增功能 (2026-06-27)

### 复制/粘贴预设
- `g_clipboard` 静态变量跨 GUI 调用周期保持
- 右侧编辑面板：名称输入框旁增加 "复制" "粘贴" 按钮
- 复制后显示 "(已复制)" 提示

### 保存/加载配置
- 保存: 将全部预设(名称+14个参数)写入 `blackhole_presets.txt`
- 加载: 从 `blackhole_presets.txt` 读取预设数据
- 文件格式: 纯文本，第一行注释，第二行预设数量，随后每个预设两行(名称 + 参数)
- 左侧面板底部：新增 "保存配置" "加载配置" 按钮

### 修改文件
- `src/gui_config.h` — 新增 `SavePresetsToFile` / `LoadPresetsFromFile` 声明
- `src/gui_config.cpp`:
  - 新增 `#include <cstdlib>`
  - 新增静态剪贴板变量 (`g_clipboard`, `g_clipName`, `g_hasClipboard`)
  - 新增 `SavePresetsToFile()` / `LoadPresetsFromFile()` 函数
  - 右侧面板新增 "复制" "粘贴" 按钮
  - 左侧面板新增 "保存配置" "加载配置" 按钮

---

## 当前修改状态

| 步骤 | 状态 |
|------|------|
| 修改 gui_config.h | ✅ 完成 |
| 添加 clipboard + save/load 函数 | ✅ 完成 |
| 添加 UI 按钮 | ✅ 完成 |
| 编译验证 | ✅ 完成 (零错误零警告) |
