// yang.c 杨老板 

inherit BUNCHER;

inherit F_DEALER;

void create()
{
	set_name("杨永福", ({ "yang yongfu", "yang" }));
	set("title", "杂货铺老板");
	set("shen_type", 1);

	set("gender", "男性");
	set("age", 45);
	set("long",
		"杨老板是土生土长的扬州人，做了几十年的小买卖。\n");
	set_skill("unarmed", 50);
	set_skill("dodge", 50);
	set_temp("apply/damage", 15);

	set("combat_exp", 40000);
	set("attitude", "friendly");
	set("vendor_goods", ({
                "/clone/cloth/cuttonp",
                "/clone/misc/sleepbag",
		"/d/city/npc/obj/mabudai",
		"/d/city/npc/obj/beixin",
                "/d/city/npc/obj/kaolu",
		"/d/city/npc/obj/toukui",
		"/d/city/npc/obj/cycle",
		"/d/city/npc/obj/surcoat",
		"/d/city/npc/obj/shield",
		"/d/city/npc/obj/huwan",
		"/d/city/npc/obj/zhitao",
		"/d/city/npc/obj/huyao",
		"/d/city/npc/obj/caoxie",
		"/d/city/npc/obj/pixue",
		"/d/city/npc/obj/shoutao",
		"/d/city/npc/obj/tieshou",
		"/d/city/npc/obj/jinsijia",
                // "/d/city/npc/obj/muxiang",
		"/d/xingxiu/obj/fire",
		"/d/item/obj/chanhs",
		"/d/city/obj/rrose",
		"/d/city/obj/yrose",
		"/clone/misc/wood",
		"/clone/misc/shoeshine",
		"/clone/misc/brush",
                "/clone/misc/yanhuo",
                "/clone/misc/diaogan",
                "/clone/misc/yuer",
                "/d/taohua/obj/bagua",                
	}));
	
	setup();
	carry_object("/clone/misc/cloth")->wear();
}

void init()
{
        add_action("do_list", "list");
        add_action("do_buy", "buy");
}

