# lvgl 模拟器

## 简介

改了下工程目录结构，用 vscode 的 cmake 插件可以编译运行，默认运行的是 `lv_demo_benchmark`。

修改 lvgl 配置和驱动配置可以在`lib\config`里改头文件。

## 日志

- 2022.02.11: 支持了文件系统，把文件放到`./storage`里就能访问了。

## 用法

在 windows 上用 vscode 打开`lv_sim_vscode_sdl.code-workspace`工作区。

cmake 插件中选择编译器，编译，运行。

## 其他

`c_cpp_properties.json`中的`browse`里的`path`改成你自己的`mingw64`路径。
