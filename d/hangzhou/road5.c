// road5.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "青石大道");
        set("long", @LONG
大道上游人，或步行，或乘轿，或骑马。个个喜气洋洋的来游西湖。往西是
去玉泉寺的路。大道则延伸向东边和西南边。
LONG );
        set("exits", ([
            "southwest" : "/d/hangzhou/road4",
            "westup"    : "/d/hangzhou/yuquan",
            "south"     : "/d/hangzhou/dadao1",
            "east"      : "/d/hangzhou/road6",
        ]));
        set("objects" , ([
            "/d/hangzhou/npc/honghua1" :1,
            "/d/hangzhou/npc/tiao-fu" :1,
        ]));
        set("outdoors", "hangzhou");
//        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}
