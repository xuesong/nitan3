// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

inherit NPC;
inherit F_MASTER;
inherit F_QUESTER;

#include <ansi.h>
#include "shaolin.h"

mixed ask_qingxin();
mixed ask_luan();
mixed ask_shang();
mixed ask_zhe();

void create()
{
	set_name("达摩老祖", ({
		"da mo",
		"da",
		"mo",
	}));
	set("long",
		"他是一位卷发络腮须的天竺老僧，身穿一袭邋遢金丝绣红袈裟。\n"
		"但却满面红光，目蕴慈笑，眼现智珠，一派得道高僧气势。\n"
                "他就是少林派开山鼻祖、当今武林的隐世高僧达摩祖师。\n"
	);

	set("title", HIR"少林开山祖师"NOR);
	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");

	set("age", 300);
	set("shen_type", 1);

	set("str", 40);
	set("int", 50);
	set("con", 45);
	set("dex", 40);

	set("max_qi", 8000);
	set("max_jing", 4000);
	set("neili", 9000);
	set("max_neili", 9000);
	set("jiali", 50);
	set("level", 60);
	set("combat_exp", 10000000);

	set_skill("buddhism", 600);
	set_skill("literate", 600);
	set_skill("sanscrit", 600);
	set_skill("martial-cognize", 500); 

	set_skill("blade", 600);
	set_skill("claw", 600);
	set_skill("club", 600);
	set_skill("cuff", 600);
	set_skill("dodge", 600);
	set_skill("finger", 600);
	set_skill("force", 600);
	set_skill("hand", 600);
	set_skill("parry", 600);
	set_skill("staff", 600);
	set_skill("strike", 600);
	set_skill("sword", 600);
	set_skill("whip", 600);
        set_skill("unarmed", 600);
	set_skill("medical", 600);
	set_skill("shaolin-yishu", 600);

	set_skill("banruo-zhang", 600);
	set_skill("shenzhang-bada", 600);
	set_skill("xumishan-zhang", 600);
	set_skill("cibei-dao", 600);
	set_skill("damo-jian", 600);
	set_skill("fengyun-shou", 600);
	set_skill("fumo-jian", 600);
	set_skill("hunyuan-yiqi", 600);
	set_skill("shaolin-xinfa", 600);
	set_skill("jingang-quan", 600);
	set_skill("longzhua-gong", 600);
	set_skill("luohan-quan", 600);
	set_skill("nianhua-zhi", 600);
	set_skill("xiuluo-zhi", 600);
        set_skill("yipai-liangsan", 600);
	set_skill("duoluoye-zhi", 600);
	set_skill("jingang-zhi", 600);
	set_skill("wuxiang-zhi", 600);
	set_skill("pudu-zhang", 600);
	set_skill("qianye-shou", 600);
	set_skill("sanhua-zhang", 600);
	set_skill("riyue-bian", 600);
	set_skill("shaolin-shenfa", 600);
	set_skill("weituo-gun", 600);
	set_skill("wuchang-zhang", 600);
	set_skill("xiuluo-dao", 600);
	set_skill("yingzhua-gong", 600);
	set_skill("yijinjing", 600);
	set_skill("yizhi-chan", 600);
	set_skill("zui-gun", 600);
        set_skill("zui-quan", 600);

	map_skill("blade", "cibei-dao");
	map_skill("claw", "longzhua-gong");
	map_skill("club", "wuchang-zhang");
	map_skill("cuff", "luohan-quan");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("finger", "nianhua-zhi");
	map_skill("force", "yijinjing");
	map_skill("hand", "fengyun-shou");
	map_skill("parry", "nianhua-zhi");
	map_skill("staff", "weituo-gun");
	map_skill("strike", "sanhua-zhang");
	map_skill("sword", "fumo-jian");
	map_skill("whip", "riyue-bian");

	prepare_skill("finger", "nianhua-zhi");
	prepare_skill("strike", "sanhua-zhang");

        set("inquiry", ([
                "绝招"   : "你想学什么绝招？",
                "绝技"   : "你想学什么绝技？",
                "达摩清心剑": (: ask_qingxin :),
                "清心剑"    : (: ask_qingxin :),
                "达摩乱气剑": (: ask_luan :),
                "乱气剑"    : (: ask_luan :),
                "达摩伤神剑": (: ask_shang :),
                "伤神剑"    : (: ask_shang :),
                "达摩折元剑": (: ask_zhe :),
                "折元剑"    : (: ask_zhe :),
        ]));

	create_family("少林派", 1, "开山祖师");
	
        set_temp("apply/attack", 300);
        set_temp("apply/defense", 300);
        set_temp("apply/damage", 150);
        set_temp("apply/unarmed_damage", 150);
        
	set("master_ob",5);
	setup();
        carry_object("/d/shaolin/obj/xuan-cloth")->wear();
}

void attempt_apprentice(object ob)
{
	object me; 
	mapping ob_fam, my_fam;
	string name, new_name;

        if (! permit_recruit(ob))
                return;

	me = this_object();
	my_fam  = me->query("family");

	name = ob->query("name");

	if (! (ob_fam = ob->query("family")) ||
	    ob_fam["family_name"] != "少林派")
	{
		command("say " + RANK_D->query_respect(ob) +
                        "与本派素无来往，不知此话从何谈起？");
		return;
	}

	if ((string)ob->query("class") != "bonze")
	{
		command("say " + RANK_D->query_respect(ob) +
                        "是俗家弟子，不能在寺内学艺。");
		return;
	}

	if ((int)ob_fam["generation"] != 36)
	{
		command("say " + RANK_D->query_respect(ob) +
                        "，贫僧哪里敢当 !");
		return;
	}

        if ((int)ob->query_int() < 35)
        {
                command("say " + RANK_D->query_respect(ob) + "悟性仍有不足，"
                        "需要进一步锻炼才能领悟更高深的佛法。");
        }

        if ((int)ob->query("WPK") > 3)
        {
                command("sigh");
                command("say " + RANK_D->query_respect(ob) + "杀戮心太重，"
                        "不适合继续学习武功。");
		return;
	}

        if ((int)ob->query_skill("buddhism", 1) < 180)
        {
                command("say " + RANK_D->query_respect(ob) + "禅宗心法"
                        "领悟得不够，不妨先多加钻研。");
                return;
        }

        if ((int)ob->query_skill("dodge", 1) < 180)
        {
                command("say " + RANK_D->query_respect(ob) + "基本轻功"
                        "火候尚有欠缺，不妨先苦心修炼。");
                return;
        }

        if ((int)ob->query_skill("force", 1) < 180)
        {
                command("say " + RANK_D->query_respect(ob) + "基本内功"
                        "火候尚有欠缺，不妨先苦心修炼。");
                return;
        }

        if ((int)ob->query_skill("parry", 1) < 180)
        {
                command("say " + RANK_D->query_respect(ob) + "基本招架"
                        "火候尚有欠缺，不妨先苦心修炼。");
                return;
        }

        if (count_lt(ob->query("combat_exp"), 1000000))
	{
                command("say " + RANK_D->query_respect(ob) + "实战经验"
                        "还颇有欠缺，不如先苦心修炼。");
                return;
	}

	command("char 老衲又得一可塑之才，真是大畅老怀 !");
        name = ob->query("name");
	new_name = "渡" + name[2..3];
	NAME_D->remove_name(ob->query("name"), ob->query("id"));
	ob->set("name", new_name);
	NAME_D->map_name(ob->query("name"), ob->query("id"));

	command("say 从今以后你的法名叫做" + new_name + "。");
	command("recruit " + ob->query("id"));
	return;
}

mixed ask_luan()
{
        object me;

        me = this_player();
        if (me->query("can_perform/damo-jian/luan"))
                return "自己好好多练习吧，牢记慈悲为怀，勿得乱用！";

        if (me->query("family/family_name") != query("family/family_name"))
                return RANK_D->query_respect(me) +
		       "并非我们少林弟子，何出此言？";

        if (! me->is_apprentice_of(this_object()))
                return "此招非我弟子不能传，还望这位" +
                       RANK_D->query_respect(me) + "见谅。";

        if (me->query("shen") < 120000)
                return "此招用于降妖除魔，你既然不擅于此道，又何必学它？";

        if (me->query_skill("damo-jian", 1) < 200)
                return "你的达魔剑法还不到家，无法领会其中奥妙！";

        if (me->query("family/gongji") < 600) 
                return "你为我少林派效力还不够，这招我先不忙传你。"; 
                
        message_vision(HIY "$n" HIY "看看$N" HIY
                       "，道：“好吧，你且听仔细了！”说罢在$N"
                       HIY "耳边轻声说了些什么。\n" NOR,
                       me, this_object());
        command("say 记住，除非降妖除魔万不得已，勿要轻易使用。");
        tell_object(me, HIG "你学会了达魔乱气剑。\n" NOR);
        if (me->can_improve_skill("force"))
                me->improve_skill("force", 160000);
        if (me->can_improve_skill("sword")) 
                me->improve_skill("sword", 1600000); 
        if (me->can_improve_skill("damo-jian")) 
                me->improve_skill("damo-jian", 1600000); 
        me->improve_skill("martial-cognize", 1600000);                 
        me->set("can_perform/damo-jian/luan", 1);
        me->add("family/gongji", -600); 
        return 1;
}

mixed ask_qingxin()
{
        object me;

        me = this_player();
        if (me->query("can_perform/damo-jian/qingxin"))
                return "自己好好多练习吧，牢记慈悲为怀，勿得乱用！";

        if (me->query("family/family_name") != query("family/family_name"))
                return RANK_D->query_respect(me) + "并非我们少林弟子，何出此言？";

        if (! me->is_apprentice_of(this_object()))
                return "此招非我弟子不能传，还望这位" +
                       RANK_D->query_respect(me) + "见谅。";

        if (me->query("shen") < 120000)
                return "此招用于降妖除魔，你既然不擅于此道，又何必学它？";

        if (me->query_skill("damo-jian", 1) < 200)
                return "你的达魔剑法还不到家，无法领会其中奥妙！";

        if (me->query("family/gongji") < 600) 
                return "你为我少林派效力还不够，这招我先不忙传你。"; 
                
        message_vision(HIY "$n" HIY "看看$N" HIY
                       "，道：“好吧，你且听仔细了！”说罢在$N"
                       HIY "耳边轻声说了些什么。\n" NOR,
                       me, this_object());
        command("say 记住，除非降妖除魔万不得已，勿要轻易使用。");
        tell_object(me, HIG "你学会了达魔清心剑。\n" NOR);
        if (me->can_improve_skill("sword"))
                me->improve_skill("sword", 160000);
        if (me->can_improve_skill("damo-jian")) 
                me->improve_skill("damo-jian", 1600000); 
        me->improve_skill("martial-cognize", 1600000);                              
        me->set("can_perform/damo-jian/qingxin", 1);
        me->add("family/gongji", -600); 
        return 1;
}

mixed ask_shang()
{
        object me;

        me = this_player();
        if (me->query("can_perform/damo-jian/shang"))
                return "自己好好多练习吧，牢记慈悲为怀，勿得乱用！";

        if (me->query("family/family_name") != query("family/family_name"))
                return RANK_D->query_respect(me) +
		       "并非我们少林弟子，何出此言？";

        if (! me->is_apprentice_of(this_object()))
                return "此招非我弟子不能传，还望这位" +
                       RANK_D->query_respect(me) + "见谅。";

        if (me->query("shen") < 120000)
                return "此招用于降妖除魔，你既然不擅于此道，又何必学它？";

        if (me->query_skill("damo-jian", 1) < 200)
                return "你的达魔剑法还不到家，无法领会其中奥妙！";

        if (me->query("family/gongji") < 600) 
                return "你为我少林派效力还不够，这招我先不忙传你。"; 
                
        message_vision(HIY "$n" HIY "看看$N" HIY
                       "，道：“好吧，你且听仔细了！”说罢在$N"
                       HIY "耳边轻声说了些什么。\n" NOR,
                       me, this_object());
        command("say 记住，除非降妖除魔万不得已，勿要轻易使用。");
        tell_object(me, HIG "你学会了达魔伤神剑。\n" NOR);
        if (me->can_improve_skill("force"))
                me->improve_skill("force", 160000);
        if (me->can_improve_skill("sword"))
                me->improve_skill("sword", 160000);
        if (me->can_improve_skill("damo-jian")) 
                me->improve_skill("damo-jian", 1600000); 
        me->improve_skill("martial-cognize", 1600000);              
        me->set("can_perform/damo-jian/shang", 1);
        me->add("family/gongji", -600); 
        return 1;
}

mixed ask_zhe()
{
        object me;

        me = this_player();
        if (me->query("can_perform/damo-jian/zhe"))
                return "自己好好多练习吧，牢记慈悲为怀，勿得乱用！";

        if (me->query("family/family_name") != query("family/family_name"))
                return RANK_D->query_respect(me) +
		       "并非我们少林弟子，何出此言？";

        if (! me->is_apprentice_of(this_object()))
                return "此招非我弟子不能传，还望这位" +
                       RANK_D->query_respect(me) + "见谅。";

        if (me->query("shen") < 120000)
                return "此招用于降妖除魔，你既然不擅于此道，又何必学它？";

        if (me->query_skill("damo-jian", 1) < 200)
                return "你的达魔剑法还不到家，无法领会其中奥妙！";

        if (me->query("family/gongji") < 600) 
                return "你为我少林派效力还不够，这招我先不忙传你。"; 
                
        message_vision(HIY "$n" HIY "看看$N" HIY
                       "，道：“好吧，你且听仔细了！”说罢在$N"
                       HIY "耳边轻声说了些什么。\n" NOR,
                       me, this_object());
        command("say 记住，除非降妖除魔万不得已，勿要轻易使用。");
        tell_object(me, HIG "你学会了达魔折元剑。\n" NOR);
        if (me->can_improve_skill("force"))
                me->improve_skill("force", 160000);
        if (me->can_improve_skill("sword"))
                me->improve_skill("sword", 160000);
        if (me->can_improve_skill("damo-jian")) 
                me->improve_skill("damo-jian", 1600000); 
        me->improve_skill("martial-cognize", 1600000);         
        me->set("can_perform/damo-jian/zhe", 1);
        me->add("family/gongji", -600); 
        return 1;
}

int accept_ask(object me, string topic)
{
        switch (topic)
        {
        case "历练" :
        case "历炼" :
        case "锻炼" :
                return QUEST_D->accept_ask(this_object(), me, topic);
                break;
        
        case "达摩三绝剑" :
                return MASTER_D->teach_pfm(me, this_object(),
                        ([ "perform" : "can_perform/damo-jian/sanjue",
                           "name"    : "达摩三绝剑",
                           "sk1"     : "damo-jian",
                           "lv1"     : 120,
                           "sk2"     : "force",
                           "lv2"     : 120,  
                           "sk3"     : "buddhism",
                           "lv3"     : 200,                         
                           "gongxian": 600, ]));
                break;

        default:
                return 0;
        }
}
