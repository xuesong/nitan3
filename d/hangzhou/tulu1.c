// tulu1.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "土路");
        set("long", @LONG
路旁一条小溪。溪水哗哗地流着，不时有鱼儿(fish)跃出水面。溪旁几各男
孩正低着头专心在钓鱼。
LONG );
        set("exits", ([
            "southwest"     : "/d/hangzhou/shanlu5",
            "northeast"     : "/d/hangzhou/maojiabu",
        ]));
        set("objects", ([
            "/d/hangzhou/npc/boy" : 2,
        ]));
        set("item_desc", ([
            "fish" : "“哗”一条大鱼跃水而出，“噗”的一声又掉回水里。渐得男孩一身水。\n",
        ]));
        set("outdoors", "hangzhou");
//        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}
