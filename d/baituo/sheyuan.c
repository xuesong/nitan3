inherit ROOM;

void create()
{
        set("short", "��԰");
        set("long", @LONG
�����ǰ���ɽׯ����԰������ɽ�Զ��������£�����Ҫ��
���ʾ����Զ��ߡ����������ߣ�ǽ��Ҳ�����ߣ�������Ҳ��
�´����ߣ���֦��Ҳ�������ߣ������㲻�ľ���
LONG);
        set("exits",([
                "north"      : __DIR__"road1",
                "west"       : __DIR__"tuyuan",
        ]));
        set("outdoors", "baituo");
        set("objects", ([
                "/clone/beast/jinshe" : random(3),
                "/clone/beast/dushe"  : 1 + random(3),
                "/clone/beast/fushe"  : random(2),
                "/clone/beast/qingshe": random(2),
                "/clone/beast/wubushe": random(3),
        ]));
        setup();
        replace_program(ROOM);
}