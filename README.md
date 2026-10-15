# C-C-with-Stm32

嵌入式 C 语言学习仓库。

- 主笔记：[嵌入式C语言学习笔记.md](嵌入式C语言学习笔记.md)
- 练习源码：`code/`
- 一键编译运行：`build.bat`

## 编译运行

本机使用 Visual Studio 的 MSVC（`D:\visual_studio\VC\Tools\MSVC\14.51.36231`）。
在仓库根目录执行：

```bat
build.bat code\lesson_byte_split.c
```

脚本会自动调用 `D:\visual_studio\VC\Auxiliary\Build\vcvars64.bat` 初始化编译环境，
再用 `cl /utf-8` 编译并立即运行。

- `.c` 源码是 UTF-8，所以 `cl` 的 `/utf-8` 参数不能去掉，否则中文会乱码；
- `build.bat` 本身只写英文：`cmd.exe` 用 OEM 代码页（本机 936）解析批处理，
  文件里出现 UTF-8 中文会把脚本解析坏；
- 编译产物（`*.obj`、`*.exe`）已在 `.gitignore` 中忽略。

## 与 GitHub 同步

`origin` 指向 GitHub 官方地址：

```bat
git pull
git push
```

注意：本机 `hosts` 把 `github.com` 等域名全部指向 `127.0.0.1`，直连会失败。
解决方法是开启加速器接管这些域名：

1. 打开 Steam++（Watt Toolkit）；
2. 网络加速 → 勾选 GitHub。

开启后它会监听本机 443 端口，git 直连即可正常工作（2026-10-15 实测通过）。

如果一时连不上，还可以走镜像**只做拉取**（镜像不支持推送）：

```bat
git fetch mirror
```

`mirror` = `https://ghproxy.net/https://github.com/MintVv/C-C-with-Stm32.git`

## 同步状态

本地 `main` 与 `origin/main` 一致，全部提交都已推送到 GitHub。
