
-- �������ս������/ʾ���ű�
function c1005420_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20064)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end
aux.RegisterCreatureEvent(1005420, 2, "c1005420_OnEnterCombat")


-- ��ɱ �������� 1005420�� ˢ�� �л걮 3030402�����꣺x 272 y 5270 z 172
function c1005420_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 20066)
	map.MapCreateCreature(MapID, InstanceID, 3030402, 272, 5270, 172)

	--ˢ�¼�ħ̨
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1531101, x, y, z)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
aux.RegisterCreatureEvent(1005420, 4, "c1005420_OnDie")


