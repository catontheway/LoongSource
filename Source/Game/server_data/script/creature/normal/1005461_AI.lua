--1005461 ������������ħ�ߣ���������50%���ٻ�
--{1005486 �ٻ���\ʾ���ű�
---------------------------------------------------------��ʱ�Խ���ս����������50%�������޸�
function c1005461_On_Enter_Combat(MapID, InstanceID, CreatureID)
	-- �������ս������
	cre.MonsterSay(MapID, InstanceID, CreatureID, 20062)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1005486, x, y, z+10)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end
aux.RegisterCreatureEvent(1005461,2,"c1005461_On_Enter_Combat")

--������������
function c1005461_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1005461, 4, "c1005461_OnDie")


