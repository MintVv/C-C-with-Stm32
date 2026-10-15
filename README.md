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

本机 `hosts` 把 `github.com` 等域名全部指向 `127.0.0.1`（且没有本地代理在 443 端口监听），
所以 git 直连 github.com 会失败。本仓库的 `origin` 因此配成“拉取走镜像、推送走 GitHub”：

```text
fetch: https://ghproxy.net/https://github.com/MintVv/C-C-with-Stm32.git
push : https://github.com/MintVv/C-C-with-Stm32.git
```

```bat
git pull     :: 走镜像，当前可用
git push     :: 走真实 GitHub，需要在能直连时执行
```

要恢复直连，二选一：

1. 打开 Steam++（Watt Toolkit）→ 网络加速 → 勾选 GitHub；
2. 清掉 `C:\Windows\System32\drivers\etc\hosts` 里所有 github 相关的行（需要管理员权限）。

## 待推送内容

本地 `main` 领先 `origin/main` 1 个提交：

```text
3077c34  2026-09-29  Record 16 bit byte assembly exercise
```

这个提交只在本机，GitHub 上还没有，注意别把本地目录删了。
