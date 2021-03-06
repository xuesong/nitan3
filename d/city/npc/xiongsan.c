// xiongsan.c 熊三 

inherit BUNCHER;
inherit F_DEALER;
#include <ansi.h>
mixed ask_back();
void create()
{
        set_name("熊三", ({ "xiong san", "xiong" }));
        set("title", HIY"熊记月饼店老板"NOR);
        set("gender", "男性");
        set("age", 45);
        set("long",
                "熊老板据说是和桃花岛有些关系，乘着中秋，在扬州开了家月饼店。\n");
        set("attitude", "friendly");
        set("vendor_goods", ({
                "/d/city/npc/obj/yuebing/bj-yuebing",
                "/d/city/npc/obj/yuebing/bp-yuebing",
                "/d/city/npc/obj/yuebing/cy-yuebing",
                "/d/city/npc/obj/yuebing/gs-yuebing",
                "/d/city/npc/obj/yuebing/hw-yuebing",
                "/d/city/npc/obj/yuebing/nl-yuebing",
                "/d/city/npc/obj/yuebing/yn-yuebing",
        }));
        set("inquiry", ([
                "桃花盛开" : (: ask_back :),
        ]));
        
        setup();
        carry_object("/clone/misc/cloth")->wear();
}

void init()
{
        add_action("do_list", "list");
        add_action("do_buy", "buy");
}

mixed ask_back()
{
        object me;

        me = this_player();
        if (me->query("family/family_name") != "桃花岛")
        {
                message_vision("$N自顾自的干活，好象没有听见$n在说什么。\n",
                               this_object(), me);
                return -1;
        }

        message_sort("$N猛然抬起头来，上上下下打量了一会"
                     "儿$n，眼角微微一动，立刻放下手中的"
                     "活计，从内室领出一只白雕，说道：你"
                     "乘坐这只白雕回桃花岛吧。\n", 
                     this_object(), me);

        tell_object(me, HIW "你坐上白雕飞了很久，越过大海，忽然眼前一亮，"
                    "你已经来到了桃花岛。\n" NOR);
        me->move("/d/taohua/jingshe");
        return -1;
}
