#include <weapon.h>
inherit STAFF;

void create()
{
        set_name("�л��", ({ "zhaohun fan", "zhaohun", "fan" }));
        set_weight(2000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", "����һ�澫���������л�ᦡ�\n");
                set("value", 800);
                set("rigidity", 100);
                set("material", "steel");
                set("wield_msg", "$N���һ��$n�������С�\n");
                set("unwield_msg", "$N�����е�$n������䡣\n");
        }
        init_staff(35);
        setup();
}