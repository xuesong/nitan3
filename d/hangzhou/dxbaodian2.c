// dxbaodian2.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "���۱���");
        set("long", @LONG
��������һ�����������������ж�ʮ������ķ��񣬴���������ʮ����
�����񣬵�������̬����Ĵ�С����һ����ʮ����
LONG );
        set("exits", ([
                "out"     : "/d/hangzhou/dxbaodian1",
        ]));
        set("no_clean_up", 0);
        set("outdoors", "hangzhou");
        setup();
        replace_program(ROOM);
}