// Code of ShenZhou
// Summer 9/12/96
#include "room.h"

inherit ROOM;

void create()
{
        set("short", "前院");
        set("long", @LONG
前院面阔七间，正中供逢着松赞干不像。这里是观看大活佛主持法会和观看
法舞之地。前院四周立有大型明柱一百四十根，上面尽是彩绘佛传故事图。
LONG );
        set("exits", ([
              "south" : "/d/xueshan/shanmen",
                "north" :"/d/xueshan/guangchang",
        ]));

        set("objects", ([
                 "/d/xueshan/npc/fsgelun" : 1,
//                "/d/xueshan/obj/jitan" : 1,
        ]));

        set("cost", 1);
        setup();
        set("outdoors","qilian-shan");
}
/*
int valid_leave(object me, string dir)
{
        object ob;
        ob=(present("fa tan", (environment(me))));
        if (objectp(ob) && me->query("name") == ob->query("pending")
        && ob->query("burning"))
                return notify_fail("你法事没做完就想走？\n");
        return ::valid_leave(me, dir);
}
*/

