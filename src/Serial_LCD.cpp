#include "Serial_LCD.h"
#include "LCD_callback.h"
#include "SoftwareSerial.h"
#include "LCD_keypad.h"

int page_state; // ��Ļ��ǰҳ��ID���
SoftwareSerial LCD_Serial; //��Ļ������

void LCD_debug(String str)
{
  Serial.println("[LCD��Ļ]:" + str); //��ӡcomdata����
}

void LCD_setup()
{
  LCD_Serial.begin(9600, SWSERIAL_8N1, 22, 23);
  //��Ϊ�����������ᷢ��88 ff ff ff,����Ҫ��մ��ڻ�����
  while (LCD_Serial.read() >= 0)
    ;                               //��մ��ڻ�����
  LCD_Serial.print("\xff\xff\xff"); //��������
  page_state = 0;
  LCD_print("page 0");
  LCD_debug("��Ļ��ʼ���ɹ�");
}

void LCD_print(String out_data)
{
  out_data = out_data + "\xff\xff\xff";
  LCD_Serial.print(out_data);
}

void LCD_while()
{
  if (LCD_Serial.available() > 0) //�ж��Ƿ񴮿�������
  {
    String raw_data = "";              //��������
    while (LCD_Serial.available() > 0) //ѭ�������Ƿ�������
    {
      raw_data += char(LCD_Serial.read()); //�������ݵ�raw_data
      delay(2);                            //��ʱ�ȴ���Ӧ
    }
    if (raw_data.length() > 0) //���raw_data������
    {
      deal_lcd_cmd(raw_data);
    }
  }
}

void deal_lcd_cmd(String raw_data)
{
  vector<String> raw_data_arry;
  int index = 0;
  int last_index = 0;
  while (1)
  {
    index = raw_data.indexOf("|", last_index);
    if (index == -1)
    {
      break;
    }
    String temp = raw_data.substring(last_index, index);
    raw_data_arry.push_back(temp);
    last_index = last_index + index + 1;
  }

  if (raw_data_arry.size() < 1)
  {
    LCD_debug("��Ч����Ļ��������");
    return;
  }

  if (raw_data_arry.at(0) == "add")
  {
    add_user(raw_data_arry.at(1));
  }
  if (raw_data_arry.at(0) == "del")
  {
    // Ԥɾ����������ʾ��
    show_confirm("��ʾ", "ȷ��ɾ��" + raw_data_arry.at(1) + "��", "2", "delR|" + raw_data_arry.at(1) + "|0");
  }
  if (raw_data_arry.at(0) == "delR")
  {
    delete_user(raw_data_arry.at(1));
  }
  if (raw_data_arry.at(0) == "adm")
  {
    set_admin(raw_data_arry.at(1));
  }
  if (raw_data_arry.at(0) == "chp")
  {
    // Ԥ�����û����룬������ʾ��
    show_confirm("��ʾ", "ȷ���û�����: " + raw_data_arry.at(1) + " ��", "6", "chpR|" + raw_data_arry.at(1) + "|0");
  }
  if (raw_data_arry.at(0) == "chpR")
  {
    change_user_password(raw_data_arry.at(1));
  }
  if (raw_data_arry.at(0) == "cha")
  {
    // Ԥ���ù������룬������ʾ��
    show_confirm("��ʾ", "ȷ�Ϲ�������: " + raw_data_arry.at(1) + " ��", "6", "chaR|" + raw_data_arry.at(1) + "|0");
  }
  if (raw_data_arry.at(0) == "chaR")
  {
    change_admin_password(raw_data_arry.at(1));
  }
  if (raw_data_arry.at(0) == "clk")
  {
    unlock(raw_data_arry.at(1));
  }
  if (raw_data_arry.at(0) == "page")
  {
    page_state = raw_data_arry.at(1).toInt();
    LCD_debug("��Ļ�л���" + String(page_state));
  }
}

/**
 * ������Ļ������ҳ��
 * @return ��
 */
void back_home_page()
{
  LCD_print("page 0");
}

/**
 * �����Ի���
 * @param title ����
 * @param center ����
 * @param last_page ����ҳ��ID
 * @param cmd ���ȷ��ִ�е�����
 * @return ��
 */
void show_confirm(String title, String center, String last_page, String cmd)
{
  LCD_print("confirm.title.txt=\"" + title + "\"");
  LCD_print("confirm.center.txt=\"" + center + "\"");
  LCD_print("confirm.last_page.val=" + last_page);
  LCD_print("confirm.cmd.txt=\"" + cmd + "\"");
  LCD_print("page 3");
}

/**
 * ������ʾ��
 * @param title ����
 * @param center ����
 * @param last_page ����ҳ��ID
 * @return ��
 */
void show_tips(String title, String center, String last_page)
{
  LCD_print("tips.title.txt=\"" + title + "\"");
  LCD_print("tips.center.txt=\"" + center + "\"");
  LCD_print("tips.last_page.val=" + last_page);
  LCD_print("page 4");
}

/**
 * �������Ա�˵�
 * @param password ��������
 * @return ��
 */
void show_admin(String password)
{
  LCD_print("admin.passwd.txt=\"" + password + "\"");
  LCD_print("page 7");
}
/**
 * ����ѡ��˵�
 * @return ��
 */
void show_menu()
{
  LCD_print("page 1");
}