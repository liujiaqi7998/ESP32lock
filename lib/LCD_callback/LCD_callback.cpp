#include "LCD_callback.h"
#include "Serial_LCD.h"

/**
* 添加用户
* @param school_id 屏幕传回来的学号
* @return 空
*/
void add_user(String school_id)
{
  LCD_debug("添加用户: " + school_id);
}

/**
* 删除用户
* @param school_id 屏幕传回来的学号
* @return 空
*/
void delete_user(String school_id)
{
  LCD_debug("删除用户: " + school_id);
}

/**
* 设置管理员
* @param school_id 屏幕传回来的学号
* @return 空
*/
void set_admin(String school_id)
{
  LCD_debug("设置管理员: " + school_id);
}

/**
* 修改用户密码
* @param password 屏幕传回来的解锁密码
* @return 空
*/
void change_user_password(String password)
{
  LCD_debug("修改解锁密码: " + password);
}

/**
* 修改管理密码
* @param password 屏幕传回来的解锁密码
* @return 空
*/
void change_admin_password(String password)
{
  LCD_debug("修改管理密码: " + password);
}

/**
* 密码解锁回调
* @param password 解锁密码
* @return 空
*/
void unlock(String password)
{
  LCD_debug("密码解锁: " + password);
  //解锁逻辑
}