// zhangfapu.c ����Ʒ���
// By Lgg,1998.9

#include <ansi.h>
inherit ITEM;

void create()
{
        set_name(HIY"����Ʒ���" NOR, ({ "zhangfa pu","pu", }));
        set_weight(1000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
             set("unit", "��");
             set("long",
        "����һ�����ƾɵ��飬�����������һЩС�˻���ȭ�ŵ�ͼ�Ρ�\n",
         );
         set("material", "paper");
         set("skill", ([
                        "name":       "haotian-zhang",
                        "exp_required" : 10000,
                        "jing_cost":  30,
                        "difficulty": 20,
                        "max_skill":  80,
			"min_skill":  10,
                ]) );
        }
}
