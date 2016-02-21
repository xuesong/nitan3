#include <room.h>

inherit ROOM;

void create()
{ 
        set("short", "��  ��");
        set("long", @LONG
�����������������ܲ��Ĵ��ţ�һ�����ӭ����չ�����顺�����񾡡��ĸ�
���֡�
LONG );
        set("outdoors", "yangzhou");
        set("exits", ([
                "west" :  "/d/xiangyang/jiekou2",
                "north" : "/d/xiangyang/xym_dating",
        ])); 
        set("objects", ([   
             "/d/xiangyang/npc/xym_guard" : 2,   
            ]));
               
        setup();            
}

int valid_leave(object me, string dir) 
{ 
        if (objectp(present("shou wei", environment(me))) && dir == "north") 
        {
           if (! me->query_temp("good_xym1") && me->query("bunch/bunch_name") != "������")
               return notify_fail("����һ����ס�㣬�����ȵ��������������������˵���������\n"); 
           else
           {
              me->delete_temp("good_xym1");
              return ::valid_leave(me, dir);
           }
        }
        return ::valid_leave(me, dir); 
} 
