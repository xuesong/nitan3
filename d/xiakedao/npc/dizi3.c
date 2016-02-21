// dizi3.c ����

#include <ansi.h>

inherit NPC;
inherit F_MASTER;

void create()
{
        set_name("���µ���", ({ "di zi", "di", "zi" }));
        set("long", "������ʮ�����׳���������Բ���ǵ�������ԭ��ļ���ġ�\n");

        set("gender", "����");
        set("age", 30);
        set("shen_type",1);
        set("attitude", "peaceful");

        set("str", 20);
        set("int", 20);
        set("con", 20);
        set("dex", 20);

        set("qi", 500);
        set("max_qi", 500);
        set("jing", 200);
        set("max_jing", 200);
        set("neili", 300);
        set("max_neili", 300);
        set("jiali", 30);

        set("combat_exp", 30000);
        set("score", 10);
        set_skill("force", 70);
        set_skill("unarmed", 70);
        set_skill("dodge", 70);
        set_skill("parry", 70);
        set_skill("hand", 70);
        set_skill("staff", 50);
        create_family("���͵�", 0, "����");

        setup();
        carry_object(__DIR__"obj/bcloth")->wear();
}

void init()
{
        object ob;
        ob = this_player();
        ::init();
        if (interactive(ob) && !ob->is_fight())
        {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}

void greeting(object ob)
{
        object room;
        room = find_object("/d/xiakedao/shimen");

        if( !ob || environment(ob) != environment()
                || environment() != room)
                return;

        message_vision("���͵�����˵���������ж�ʮ����ʯ�ң���λ����������ȥȡ�ã�����
���ˣ��ɵ�����ɢ�ģ�һӦ��ʳ��ʯ���ھ����ñ�����λ����ȡ�ã���
�ؿ�����\n", this_object());
}