#include "LCD_keypad.h"

extern int page_state; // 屏幕当前页面ID标记

const byte ROWS = 4; // 每行按钮数量
const byte COLS = 4; // 每列按钮数量
/* 按钮对应的标签 */
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'E'},
    {'4', '5', '6', 'M'},
    {'7', '8', '9', 'U'},
    {'P', '0', 'O', 'D'}};
/* 按钮对应的引脚 */
byte colPins[COLS] = {12, 27, 25, 32}; // 连接行引脚
byte rowPins[ROWS] = {33, 26, 14, 13}; // 连接列引出线

// 页面数量，添加页面时需要在此处添加
const int page_number = 10;
// 按键数量 (4*4键盘布局)
const int key_number = 16;
// 页面ID与键盘ID映射表，添加页面时需要在此处添加
String key_map[page_number][key_number] = {
    {"0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 0
    {"4", "3", "2", "1",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 1
    {"2", "3", "4", "1",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 2
    {"0", "0", "0", "1",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "2", "0"}, // 3
    {"0", "0", "0", "1",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 4
    {"1", "2", "3", "12",
     "4", "5", "6", "0",
     "7", "8", "9", "0",
     "0", "10", "11", "0"}, // 5
    {"1", "2", "0", "3",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 6
    {"1", "2", "3", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 7
    {"0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"}, // 8
    {"3", "0", "0", "1",
     "0", "0", "0", "0",
     "0", "0", "0", "0",
     "0", "0", "0", "0"} // 9
};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String msg;
void keypad_while()
{
    if (kpd.getKeys())
    {
        for (int i = 0; i < LIST_MAX; i++) // 扫描整个按键列表
        {
            if (kpd.key[i].stateChanged) // 仅查找已更改状态的按键
            {
                if (kpd.key[i].kstate == PRESSED)
                {

                    // 当按钮按下的时候会执行下面的代码
                    if (page_state < page_number && page_state >= 0 && kpd.key[i].kcode < key_number && kpd.key[i].kcode >= 0)
                    {
                        String click_id = key_map[page_state][kpd.key[i].kcode];
                        LCD_debug("模拟按下ID:" + click_id);
                        LCD_print("click " + click_id + ",1");
                        delay(100);
                        LCD_print("click " + click_id + ",0");
                    }
                }
            }
        }
    }
}