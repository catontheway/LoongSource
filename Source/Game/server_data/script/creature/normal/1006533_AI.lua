--�������͵��ű�����ID��1006533/����ս��ʾ���ű�
--����ʱ��ͬ��ͼ����<774, 6482, 1474>ˢ�³�������ɷʬ��
--1006533	���͵�������֮�ȣ����� ս�����ٻ�
--{1006546 �޼�Ӫ��ʿ x 1460 y 10168 z 1397
--{1006549 ����Ӫ��ʿ x 1460 y 10168 z 1405
--{1006550 ����Ӫ��ʿ x 1460 y 10168 z 1414
function c1006533_On_Enter_Combat(MapID, InstanceID, CreatureID)
	-- �������ս������
	cre.MonsterSay(MapID, InstanceID, CreatureID, 30106)
       map.MapCreateCreature(MapID, InstanceID, 1006546, 1460, 10168, 1397)
	map.MapCreateCreature(MapID, InstanceID, 1006549, 1460, 10168, 1405)
	map.MapCreateCreature(MapID, InstanceID, 1006550, 1460, 10168, 1414)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
--ע��
aux.RegisterCreatureEvent(1006533,2,"c1006533_On_Enter_Combat")


--����ʱ�ű�
function c1006533_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       --������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 30107)
	map.MapCreateCreature(MapID, InstanceID, 1006534, 774, 6482, 1474)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע��
aux.RegisterCreatureEvent(1006533, 4, "c1006533_OnDie")






