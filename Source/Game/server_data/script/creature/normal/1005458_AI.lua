--��ɱ ���� 1005458 �� c01 ������ ˢ�� ��ȶ���1005459

-- �������ս������/ʾ���ű�
function c1005458_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 20088)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1005458, 2, "c1005458_OnEnterCombat")

--��������ʱ����
function c1005458_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 20090)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1005459, x+2, y, z+2, 1, "c01")
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע�ắ��
aux.RegisterCreatureEvent(1005458, 4, "c1005458_OnDie")







