// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// zhu.c 朱元璋

#include <ansi.h>

inherit NPC;

void create()
{
	set_name("朱元璋", ({ "zhu yuanzhang","zhu","yuanzhang" }) );
	set("title", "明教洪水旗弟子");
	set("gender", "男性");        
	set("age",36);
	set("long","他是后来明朝的皇帝,依靠明教的影响才取得天下。\n");
       	set("str", 24);
	set("int", 30);
	set("con", 23);
	set("dex", 20);
        set("per", 10);
	set("max_qi",600);
	set("max_jing",400);
	set("neili",400);
	set("max_neili",400);
	set("jiali",20);
	set("eff_jingli",200);        
	set("shen_type",1);
	set("combat_exp", 90000);
        set("level", 8);
        
        set_skill("blade",80);
        set_skill("force",80);
        set_skill("dodge",80);
        set_skill("claw",80);
        set_skill("qingfu-shenfa",85);
        set_skill("shenghuo-shengong",85);
        set_skill("lieyan-dao",85);
        set_skill("yingzhua-shou",85);
        map_skill("claw","yingzhua-shou");
        map_skill("force","shenghuo-shengong");
        map_skill("dodge","qingfu-shenfa");
        map_skill("blade","lieyan-dao");
        map_skill("parry","lieyan-dao");
        
        prepare_skill("claw", "yingzhua-shou");
        
        create_family("明教",38,"弟子");
        
	setup();
	carry_object("/d/mingjiao/obj/black-cloth")->wear();
	carry_object("/clone/weapon/blade")->wield();
}

void attempt_apprentice(object ob)
{     
        command("say 呵呵，我武功低微，可不敢收徒啊。");
        command("say 这位"+RANK_D->query_respect(ob)+"还是请回吧。");
        return;
}
