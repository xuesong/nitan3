inherit ROOM;

void create()
{
        set("short", "���ݲ���");
	set("long", @LONG
�����Ǿ��ݵĲ��ţ�������·�˶�붼������Ъ�š�����
������ˮ��������������ЩЪ�ŵ��ˣ������̸Ц����ͷ��
������Ҫ����������ƹʺ�ҥ�ԣ������Ǹ������ڡ�
LONG);
	set("resource/water", 1);
	set("exits", ([
                "east" : __DIR__"nandajie1",
	]));
	set("objects", ([
                __DIR__"npc/afang" : 1,
	]));
	set("coor/x", -460);
	set("coor/y", -220);
	set("coor/z", 20);
	setup();
	replace_program(ROOM);
}
