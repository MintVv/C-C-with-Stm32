/*
 * 本节示例：16 位数值与两个字节之间的互换（拼接 / 拆分 / 字节序）
 *
 * 对应笔记章节：《把两个字节拼成一个 16 位数值》之后的下一题
 *               —— 把一个 16 位数值拆成两个字节发送
 *
 * 编译环境（本机实测可用）：
 *   Visual Studio MSVC 14.51.36231
 *   D:\visual_studio\VC\Auxiliary\Build\vcvars64.bat
 * 一键编译并运行：仓库根目录执行  build.bat code\lesson_byte_split.c
 */

#include <stdio.h>
#include <stdint.h>

/* 用一个函数把“拆分”封装起来，顺便复习指针传参：函数里改的正是外面那两个变量 */
static void split_u16(uint16_t value, uint8_t *high, uint8_t *low)
{
    *high = (uint8_t)(value >> 8);   /* 高 8 位：整体右移 8 位，再截成 1 字节 */
    *low  = (uint8_t)(value & 0xFF); /* 低 8 位：& 0xFF 只保留最右边 8 个 1 */
}

int main(void)
{
    /* ---------- 1. 拼接：两个字节 -> 16 位（笔记里做过的那题） ---------- */
    uint8_t  high_in = 0x12;
    uint8_t  low_in  = 0x34;
    uint16_t joined  = ((uint16_t)high_in << 8) | low_in;

    printf("[拼接] high=0x%02X low=0x%02X  ->  value=0x%04X (%u)\n",
           (unsigned)high_in, (unsigned)low_in, (unsigned)joined, (unsigned)joined);

    /* ---------- 2. 拆分：16 位 -> 两个字节（本节新内容） ---------- */
    uint16_t send_value = 0xABCD;
    uint8_t  out_high = 0;
    uint8_t  out_low  = 0;

    split_u16(send_value, &out_high, &out_low);

    printf("[拆分] value=0x%04X  ->  high=0x%02X low=0x%02X\n",
           (unsigned)send_value, (unsigned)out_high, (unsigned)out_low);

    /* ---------- 3. 往返验证：拆完再拼，必须回到原值 ---------- */
    uint16_t restored = ((uint16_t)out_high << 8) | out_low;

    printf("[往返] 0x%04X 再拼回来 = 0x%04X  %s\n",
           (unsigned)send_value, (unsigned)restored,
           (restored == send_value) ? "一致" : "不一致");

    /* ---------- 4. 常见错误对照 ---------- */
    printf("\n--- 常见错误对照 (value=0x%04X) ---\n", (unsigned)send_value);

    /* 错误 1：忘了右移，想取高字节却取到了低字节 */
    uint8_t wrong1 = (uint8_t)send_value;
    printf("错1 (uint8_t)value            = 0x%02X  <-- 取到的是低字节 0xCD\n", (unsigned)wrong1);

    /* 错误 2：左移 8 位再截成 1 字节，高字节被移出后只剩 0 */
    uint8_t wrong2 = (uint8_t)(send_value << 8);
    printf("错2 (uint8_t)(value << 8)     = 0x%02X  <-- 左移把高字节推出去了，得到 0x00\n", (unsigned)wrong2);

    /* 错误 3：先拆成字节，却用 int 收结果，忘记截断（这里只是演示移位结果）
     *       0xABCD >> 8 = 0xABCD，注意移位不会自动变窄            */
    unsigned wrong3 = (unsigned)(send_value >> 8);
    printf("错3 (unsigned)(value >> 8)    = 0x%X   <-- 值对，但类型是 unsigned 不是 1 字节\n", wrong3);

    /* 正确写法：右移 8 位 + 截断成 uint8_t，取高字节 */
    printf("对   (uint8_t)(value >> 8)    = 0x%02X\n", (unsigned)(uint8_t)(send_value >> 8));

    /* ---------- 5. 本机字节序：为什么“先发高字节还是先发低字节”要约定 ---------- */
    uint16_t probe = 0x0102;
    uint8_t *p = (uint8_t *)&probe;

    printf("\n--- 本机字节序 ---\n");
    printf("在内存里存 0x0102，低地址那个字节是 0x%02X\n", (unsigned)p[0]);
    printf("本机是%s端：%s\n",
           (p[0] == 0x02) ? "小" : "大",
           (p[0] == 0x02)
               ? "低字节存在低地址；直接 printf 打印和逐字节发送的结果会不一样"
               : "高字节存在低地址");

    /* ---------- 6. 串口“逐字节发送”的模拟：先发高字节，再发低字节 ---------- */
    uint8_t frame[2];

    frame[0] = out_high;   /* 发送顺序由协议规定，这里约定高字节在前 */
    frame[1] = out_low;

    printf("\n--- 模拟串口发送 ---\n");
    printf("发送顺序: 0x%02X 0x%02X\n", (unsigned)frame[0], (unsigned)frame[1]);

    /* 接收方按同样顺序拼回来 */
    uint16_t received = ((uint16_t)frame[0] << 8) | frame[1];
    printf("接收方拼回: 0x%04X  %s\n",
           (unsigned)received, (received == send_value) ? "正确" : "错误");

    /* 若接收方误按“低字节在前”拼，就会得到交换过的值 —— 字节序不约定清楚就会出这种错 */
    uint16_t swapped = ((uint16_t)frame[1] << 8) | frame[0];
    printf("若拼反了:   0x%04X  <-- 0xABCD 变成 0xCDAB\n", (unsigned)swapped);

    return 0;
}
