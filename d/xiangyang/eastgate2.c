// Room: /d/xiangyang/eastgate2.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;
#include <ansi.h>;

void create()
{
	set("short", "青龙外门");
        set("long", @LONG
这里是襄阳城的东城门，只见城门上方刻着『青龙门』三个大字。城门高三
丈，宽约二丈有余，尺许厚的城门上镶满了拳头般大小的柳钉。门洞长约四丈，
大约每隔两个时辰就换官兵把守。近年来蒙古屡次侵犯襄阳城，故这儿把守相当
严密，城内一些官兵们正在盘查要离襄阳城的人。
LONG );
        set("outdoors", "xiangyang");

	set("exits", ([
		"west"  : "/d/xiangyang/eastgate1",
                "east"  : "/d/city2/yidao",
                "northeast"  : "/d/zhongzhou/toyy",
	]));
	set("objects", ([
		"/d/xiangyang/npc/pi"   : 1,
		"/d/xiangyang/npc/bing" : 2,
	]));
	set("coor/x", -240);
	set("coor/y", -90);
	set("coor/z", 0);
	setup();
}


int valid_leave(object me,string dir)
{
       if( dir == "east" && !wizardp(me) 
           && !me->query_temp("warquest") )
       return notify_fail("军营重地，闲杂人等，不得入内！\n");

       return ::valid_leave(me,dir);
}
