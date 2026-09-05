# C 语言与嵌入式学习总结

## 当前目标

目标是从项目经理逐步转到嵌入式软件研发。现阶段不需要把 C 语言学得很深，先达到以下水平：

- 能读懂并修改简单的 STM32 示例工程；
- 能理解传感器采样、串口/RS485 通讯、告警处理这类代码；
- 能编译、下载、调试简单的 MCU 程序；
- 后续争取从固件测试、通讯验证或小功能修改切入研发。

## 当前基础摸底结果

已经掌握或能很快恢复的内容：

- 变量、条件判断、循环；
- 数组的基本使用；
- 结构体和 `.` 访问成员；
- 基础位运算：`1 << 2`、`|=` 置位；
- 通过推导理解简单程序的运行结果。

接下来需要重点强化：

1. 函数传参：值传递和指针传参的区别；
2. 指针：`&`、`*`、`->`；
3. `const`、`static`、`volatile`；
4. 比较、赋值及括号等代码细节；
5. 将上述知识放进真实 STM32 工程中理解。

## 今天学到的关键知识

### 1. 值传递和指针传参

```c
void add_one(int x)
{
    x = x + 1;
}
```

调用 `add_one(a)` 时，`a` 的值会复制给 `x`。修改 `x` 不会修改外面的 `a`。

```c
void add_one(int *x)
{
    *x = *x + 1;
}
```

调用 `add_one(&a)` 时，传入的是 `a` 的地址。`*x` 表示该地址中存放的实际变量，所以能修改 `a`。

记忆：

- `&a`：取 `a` 的地址；
- `p`：保存地址；
- `*p`：访问该地址里的值；
- `*p = 20`：修改地址对应变量的值，不会修改 `p` 保存的地址。

### 2. 结构体指针

```c
typedef struct
{
    int temperature;
    unsigned char alarm;
} Sensor;

Sensor s;
Sensor *p = &s;
```

- 普通结构体变量访问成员：`s.temperature`；
- 结构体指针访问成员：`p->temperature`；
- `p->temperature` 等价于 `(*p).temperature`。

### 3. `const`

```c
int get_temperature(const Sensor *sensor)
{
    return sensor->temperature;
}
```

`const Sensor *sensor` 表示函数通过 `sensor` 只能读取结构体，不能修改它：

```c
sensor->temperature;      // 可以读取
sensor->alarm = 1;        // 不允许
```

`const` 只限制当前这条指针的操作，不代表其他地方永远不能修改这个结构体。

### 4. `static`

```c
void count(void)
{
    static int value = 0;
    value++;
}
```

`static` 局部变量只初始化一次，函数结束后值仍会保留；但它只能在当前函数内访问。

普通局部变量每次调用函数都会重新创建和初始化。

### 5. `volatile` 与中断

```c
volatile unsigned char rx_done = 0;
```

`volatile` 不是“初始化为 0”。`= 0` 才是初始化。

`volatile` 告诉编译器：这个变量可能被中断或硬件改动，每次代码访问它时，都不要擅自认为它的值没变。

```c
while (rx_done == 0)
{
}
```

若串口中断把 `rx_done` 改为 `1`，主循环才能在下一次读取时发现变化并退出。

- `interrupt`：决定中断发生后执行什么中断函数；
- `volatile`：确保中断改动的共享变量能被主程序正确读取。

### 6. 位运算

```c
status |= (1 << 2);
```

作用：把 `status` 的第 2 位（从 0 开始）置为 `1`，其他位保持不变。

```c
1 << 2          // 0b00000100
status |= ...   // 设置第 2 位
```

常用于告警标志、寄存器位和通讯状态。

### 7. 常见符号

C 代码中要使用英文半角符号：

```c
=    // 赋值
==   // 是否相等
!=   // 不相等
>    // 大于
<    // 小于
>=   // 大于或等于
<=   // 小于或等于
```

注意：C 代码不能写 `≥`，必须分开输入 `>` 和 `=`，组成 `>=`。

## 写过的典型代码

### 判断是否超温（只读取数据）

```c
unsigned char is_over_temperature(const Sensor *sensor)
{
    return sensor->temperature >= 80;
}
```

### 更新告警状态（直接修改数据）

```c
void update_alarm(Sensor *sensor)
{
    sensor->alarm = (sensor->temperature >= 80);
}
```

上面一行等价于：温度大于等于 80 时 `alarm = 1`，否则 `alarm = 0`。

## 接下来三周学习计划

### 第 1 周：函数和指针

- 复习函数、参数、返回值；
- 练习 `&`、`*`、指针传参；
- 练习数组传参；
- 能解释 `p`、`*p` 和 `&a` 的区别。

### 第 2 周：结构体和模块化

- 练习 `struct`、`typedef`、`.`、`->`；
- 理解 `.c` 和 `.h` 文件的职责；
- 继续练 `static`、`const` 和宏定义；
- 写一个保存温度、告警、通讯状态的结构体。

### 第 3 周：嵌入式 C 与 STM32

- 练 `uint8_t`、`uint16_t`、`uint32_t`；
- 练位运算、`volatile`、中断标志；
- 开始看 STM32 的 GPIO、串口、定时器、ADC 示例；
- 优先理解一个完整小功能：数据采集 -> 判断 -> 串口发送/告警。

## 学习方法

- 工位碎片时间：看概念、读公开资料/允许查看的工程、记问题；
- 下班集中时间：写小程序、跑 STM32 示例、调试；
- 每次只学一个点，必须自己写一段代码或答几道题；
- 不必等 C 全学完再碰 STM32，遇到真实工程中的 C 知识再补；
- 看同事工程前确认公司允许查看，不要把公司保密代码外传。

## 今天继续：数组、循环与通讯缓冲区

### 8. 数组与循环：批量处理采样数据

数组把一批同类型数据放在一起，用下标访问。多次采样、接收缓冲区，本质都是数组。

```c
int sample[4];          // 声明 4 个 int，下标 0~3

sample[0] = 25;
sample[1] = 27;
sample[2] = 30;
sample[3] = 26;
```

- 下标从 0 开始：`sample[0]` 是第一个，`sample[3]` 是最后一个；
- 声明时直接给初值：`int sample[4] = {25, 27, 30, 26};`
- `sample[4]` 已经越界。越界编译器不一定报错，但运行时会读写到别的变量，是最隐蔽的 bug，遍历时务必用 `i < 4`，不要写 `i <= 4`。

用 for 循环遍历数组：

```c
int sum = 0;
for (int i = 0; i < 4; i++)
{
    sum = sum + sample[i];
}

float average = (float)sum / 4;   // 先转 float 再除，否则整数除法会丢小数
```

- `for` 三个部分：初始化 `int i = 0`；条件 `i < 4`；步进 `i++`；
- 遍历 n 个元素统一写 `for (i = 0; i < n; i++)`，记成“i 小于长度”，最不容易错；
- `sum += sample[i]` 等价于 `sum = sum + sample[i]`。

数组长度用宏定义：

```c
#define SAMPLE_COUNT 4

int sample[SAMPLE_COUNT];
```

以后要改成采 8 个点，只改这一处。宏是编译前把名字替换成数值，后面不加分号。

嵌入式主循环 while(1)：

```c
int main(void)
{
    init_system();
    while (1)
    {
        process();
    }
}
```

- `while (1)` 是无限循环，MCU 程序跑起来后一直在这里转；
- `do-while`：先执行一次循环体再判断条件，适合“至少执行一次”的场景。

### 9. 数组传参：数组名就是首元素地址

```c
float average_of(const int *samples, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += samples[i];
    }
    return (float)sum / n;
}
```

调用：

```c
float avg = average_of(sample, SAMPLE_COUNT);
```

关键点：

- 数组名 `sample` 就是首元素地址，等价于 `&sample[0]`，所以数组传给函数时，函数拿到的是地址，和指针传参本质一样；
- 函数内 `samples[i]` 和外面的 `sample[i]` 是同一块内存，函数里写 `samples[i] = 0`，外面数组也会变；
- 只读函数加 `const int *samples`，防止手误改掉采样数据；
- 数组传参后，函数内 `sizeof(samples)` 是指针大小（4 或 8），不是数组大小，所以必须额外传长度参数 `n`；
- 需要修改数组内容的函数不加 `const`：

```c
void clear_samples(int *samples, int n)
{
    for (int i = 0; i < n; i++)
    {
        samples[i] = 0;
    }
}
```

结构体数组：管理多个传感器。

```c
#define SENSOR_COUNT 3

Sensor sensors[SENSOR_COUNT];

sensors[0].temperature = 26;
sensors[1].temperature = 82;

for (int i = 0; i < SENSOR_COUNT; i++)
{
    if (sensors[i].temperature >= 80)
    {
        sensors[i].alarm = 1;
    }
}
```

### 10. 字符串与字符数组

串口打印、日志输出、AT 指令都离不开字符串。

```c
char buf[16];
buf[0] = 'A';
buf[1] = 'B';
buf[2] = '\0';          // 字符串结束符，值就是 0

// 等价写法
char buf2[] = "AB";
```

- `'A'` 是字符（1 字节），`"A"` 是字符串（占 2 字节：`'A'` + `'\0'`）；
- 字符串以 `\0` 结尾，`strlen` 等函数靠它判断到哪结束；
- 常用函数（需 `#include <string.h>`）：

```c
strcpy(buf, "OK");            // 把 "OK" 拷贝进 buf
sprintf(buf, "temp=%d", 26);  // 按格式生成字符串
```

- 注意 `sprintf` 不检查缓冲区大小，内容太长会溢出。对长度没把握时用 `snprintf(buf, sizeof(buf), ...)`；
- 通讯数据里可能包含 `0x00`，不能靠 `\0` 判断一帧结束，所以嵌入式通讯常用“字节数组 + 长度”，见下一节。

### 11. 简单通讯接收缓冲区

场景：串口中断每次收到 1 个字节，先存进数组，一帧收完后再由主循环解析。

```c
#define RX_BUF_SIZE 64

volatile unsigned char rx_buf[RX_BUF_SIZE];   // 接收缓冲区
volatile unsigned char rx_len = 0;            // 已收到几个字节
volatile unsigned char rx_ready = 0;          // 一帧收完的标志
```

串口中断里收 1 个字节（在中断里只做“存”这件事，解析放主循环）：

```c
void on_rx_byte(unsigned char byte)
{
    if (rx_len < RX_BUF_SIZE)     // 越界保护，防止写满后溢出
    {
        rx_buf[rx_len] = byte;
        rx_len++;
    }
}
```

主循环里处理：

```c
if (rx_ready)
{
    // 解析 rx_buf[0] ~ rx_buf[rx_len - 1]
    rx_len = 0;     // 清空计数，准备收下一帧
    rx_ready = 0;
}
```

为什么这样写：

- 缓冲区、长度、标志都被中断修改、又被主循环读取，必须加 `volatile`；
- `if (rx_len < RX_BUF_SIZE)` 是越界保护：满 64 就不再写入，避免写坏数组后面的内存；
- 真实工程里“一帧结束”靠帧头、帧尾、长度、校验（CRC）判断，这里先掌握“收字节 -> 存数组 -> 置标志”这条主线。

### 12. switch-case 和枚举：处理命令与状态

收到上位机命令、处理设备状态时，`switch-case` 比一长串 `if-else` 清晰：

```c
typedef enum
{
    CMD_NONE = 0,
    CMD_READ_TEMP,
    CMD_READ_ALARM,
    CMD_RESET
} Cmd;

void handle_cmd(Cmd cmd)
{
    switch (cmd)
    {
        case CMD_READ_TEMP:
            send_temp();
            break;
        case CMD_READ_ALARM:
            send_alarm();
            break;
        case CMD_RESET:
            do_reset();
            break;
        default:
            send_error();
            break;
    }
}
```

- `enum` 给一组常量起名字，默认从 0 递增，也可以手动指定；
- 每个 `case` 结尾要 `break;`，漏掉会继续往下执行下一个 case；
- `default` 处理没匹配的情况，建议都写上。

## 练习题（先自己写，再看答案）

1. 定义 `int adc[8] = {10, 20, 30, 40, 50, 60, 70, 80};`，写函数求最大值并返回；
2. 写函数 `void inc_all(int *arr, int n)`，把数组每个元素加 1；
3. 用结构体数组保存 3 个传感器温度，写循环统计超温（>= 80）的数量；
4. 说出下面代码的输出（提示：注意 `continue` 和 `break`）：

```c
for (int i = 0; i < 10; i++)
{
    if (i == 3) continue;
    if (i == 6) break;
    printf("%d ", i);
}
```

参考答案：

1.

```c
int max_of(const int *arr, int n)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}
```

2.

```c
void inc_all(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = arr[i] + 1;
    }
}
```

3.

```c
typedef struct
{
    int temperature;
    unsigned char alarm;
} Sensor;

int count_over_temp(const Sensor *sensors, int n)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (sensors[i].temperature >= 80)
        {
            count++;
        }
    }
    return count;
}
```

4. 输出 `0 1 2 4 5`：`i == 3` 时 `continue` 跳过本次循环后面的代码；`i == 6` 时 `break` 直接结束整个循环。

## 今天继续：数据类型、逻辑与位运算

### 13. 数据类型与 uint8_t（嵌入式标准写法）

```c
uint8_t a;      // 无符号 8 位，0~255
uint16_t b;     // 无符号 16 位，0~65535
uint32_t c;     // 无符号 32 位，0~4294967295
int8_t d;       // 有符号 8 位，-128~127
int16_t e;      // 有符号 16 位，-32768~32767
```

- `uint8_t` 等价于 `unsigned char`，`uint16_t` 等价于 `unsigned short`，`uint32_t` 等价于 `unsigned int/long`，需要 `#include <stdint.h>`；
- 好处：位宽明确，写寄存器、通讯协议字段时不会因为 int 大小不同而出错，STM32 HAL 库到处用这些类型；
- `char` 有没有符号取决于编译器，所以一律用 `int8_t`/`uint8_t` 最稳妥；
- STM32 上常见大小：`char` 8 位、`short` 16 位、`int` 32 位；
- 打印格式：`%d` 有符号十进制、`%u` 无符号十进制、`%x` 十六进制、`%02X` 两位大写十六进制（打印 0x0A 显示 "0A"）。

```c
uint8_t temp = 200;
printf("temp=%u (0x%02X)\n", temp, temp);
```

### 14. 位运算进阶：置位、清位、翻转、读位

之前学了置位 `|=`。补齐另外三个常用操作：

```c
uint8_t reg = 0x00;

reg |= (1 << 2);        // 置位：第 2 位变 1
reg &= ~(1 << 2);       // 清位：第 2 位变 0，其他位不变
reg ^= (1 << 2);        // 翻转：第 2 位 0<->1
bit = (reg >> 2) & 1;   // 读位：取出第 2 位的值（0 或 1）
```

- `~` 按位取反：`~(1 << 2)` 是“除第 2 位为 0，其他位全是 1”，这样 `&=` 只把第 2 位清零；
- 一次操作多个位：`reg |= (1 << 0) | (1 << 3);`
- 寄存器操作示意（风格类似 STM32 寄存器操作）：

```c
GPIOB->ODR |= (1 << 5);      // PB5 输出高电平
GPIOB->ODR &= ~(1 << 5);     // PB5 输出低电平
```

- 补充：`0b1010` 这种二进制写法是 GCC 扩展（多数嵌入式编译器支持），标准 C 里写十六进制 `0xA6` 更通用。

### 15. 逻辑运算：&&、||、!

位运算按“位”算，逻辑运算按“真假”算，结果只有 0 或 1。

```c
if (a >= 80 && b <= 100)   // 两个条件同时成立才为真
if (a >= 80 || alarm)      // 任一成立即为真
if (!rx_ready)             // 取反：rx_ready == 0 时为真
```

- 任何非 0 都算“真”，0 算“假”；
- `&&` 和 `||` 有短路特性：`a && b` 中 a 为假则 b 不执行；`a || b` 中 a 为真则 b 不执行。利用短路可以先判空再访问：

```c
if (p != NULL && p->temperature >= 80)
{
    // p 为空时不会执行 p->temperature，避免崩溃
}
```

- 区分 `&`（位与）和 `&&`（逻辑与）：`1 & 2` 结果是 0，`1 && 2` 结果是 1。

### 16. 三目运算符

```c
alarm = (temperature >= 80) ? 1 : 0;
// 等价于
if (temperature >= 80)
{
    alarm = 1;
}
else
{
    alarm = 0;
}
```

- 结构：`条件 ? 值A : 值B`，条件为真取 A，为假取 B；
- 适合“一行赋值二选一”，逻辑复杂时用 if-else 更清楚。

## 练习题（先自己写，再看答案）

1. 用位运算把 `uint8_t x = 0x00;` 的 bit3、bit5 置 1；
2. 把 `uint8_t y = 0xFF;` 的 bit0 清 0；
3. 读 `uint8_t z = 0b10100110;` 的 bit2，结果是多少；
4. 判断 `0b0001 & 0b0010` 和 `0b0001 && 0b0010` 的结果分别是什么。

参考答案：

1. `x |= (1 << 3) | (1 << 5);` 结果 `x = 0x28`（0x08 | 0x20）。
2. `y &= ~(1 << 0);` 结果 `y = 0xFE`。
3. `(z >> 2) & 1`，`z = 0b10100110` 的 bit2 是 `1`，所以结果是 `1`。
4. `0b0001 & 0b0010 = 0`（按位与，没共同位）；`0b0001 && 0b0010 = 1`（逻辑与，两个都非 0 即为真）。

## 训练记录

### 数组最大值

题目：在 `max_value()` 中遍历数组并找出最大元素。

```c
int max_value(const int *data, int count)
{
    int max = data[0];

    for (int i = 1; i < count; i++)
    {
        if (max < data[i])
        {
            max = data[i];
        }
    }

    return max;
}
```

要点：

- 初始时把第一个元素 `data[0]` 当作最大值；
- 从下标 `1` 开始逐个与当前 `max` 比较；
- 只有当前元素更大时，才更新 `max`；
- 不需要 `else`，因为当前元素不大于 `max` 时应保持原来的最大值；
- 函数调用时必须保证 `data` 有数据，即 `count > 0`；
- `if` 后的大括号 `}` 不需要再加分号；写成 `};` 通常也能编译，但多余且不建议。

### 数组元素批量加 1

题目：通过数组指针和长度参数，把每个元素加 `1`。

```c
void inc_all(int *data, int count)
{
    for (int i = 0; i < count; i++)
    {
        data[i] = data[i] + 1;
    }
}
```

也可简写为：

```c
data[i] += 1;
```

要点：

- 数组名传给 `inc_all()` 时会传入首元素地址，因此函数内修改 `data[i]` 会直接修改调用方的原数组；
- 循环范围必须是 `i < count`，以免访问 `data[count]` 导致越界；
- 若函数只读取数组，参数写成 `const int *data`；本题需要修改数组，所以不能加 `const`。

### 结构体数组统计超温数量

题目：统计结构体数组中温度大于等于 `80` 的传感器数量。

```c
int count_over_temperature(const Sensor *sensors, int count)
{
    int over_count = 0;

    for (int i = 0; i < count; i++)
    {
        if (sensors[i].temperature >= 80)
        {
            over_count++;
        }
    }

    return over_count;
}
```

要点：

- `Sensor` 是类型名，`sensors` 才是函数参数中的结构体数组；
- `sensors[i]` 表示第 `i` 个结构体对象，因此用 `.` 访问成员：`sensors[i].temperature`；
- 如果变量本身是结构体指针，才使用 `sensor->temperature`；
- `sensor->temperature` 等价于 `(*sensor).temperature`；
- `over_count++` 让计数值增加 1；
- `if` 代码块结尾的 `}` 后不需要额外加分号。

### `const` 数组指针

题目：判断下面函数能否清空传入的数组。

```c
void clear_data(const int *data, int count)
{
    for (int i = 0; i < count; i++)
    {
        data[i] = 0;    // 编译错误
    }
}
```

答案：不能正常编译。

`const int *data` 表示通过 `data` 只能读取数组元素，不能修改数组元素。若函数需要清空数组，应去掉 `const`：

```c
void clear_data(int *data, int count)
{
    for (int i = 0; i < count; i++)
    {
        data[i] = 0;
    }
}
```

要点：

- `const int *data`：允许读取，不允许通过 `data` 修改数据；
- `int *data`：允许读取，也允许修改数据；
- `const` 与 `count` 是否大于 0 无关，它在编译阶段限制写操作。

### `const` 的编译期限制

题目：判断下面函数能否正常编译。

```c
int get_temperature(const Sensor *sensor)
{
    if (sensor->temperature >= 80)
    {
        sensor->alarm = 1;    // 编译错误
    }

    return sensor->temperature;
}
```

答案：不能正常编译。

`const Sensor *sensor` 表示不能通过 `sensor` 修改结构体的任何成员。即使温度小于 `80`、运行时不会进入 `if`，编译器仍然会检查这行赋值，因此 `const` 的限制与运行时条件无关。

如果函数需要修改 `alarm`，参数应改为：

```c
Sensor *sensor
```

如果函数只负责读取温度，则保留 `const` 并删除修改操作：

```c
int get_temperature(const Sensor *sensor)
{
    return sensor->temperature;
}
```

## 下次继续的位置

下一题：复习 `static` 局部变量，判断函数多次调用后的结果。