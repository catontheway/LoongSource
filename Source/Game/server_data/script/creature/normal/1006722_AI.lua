
-- �������ս������ʾ��
function c1006722_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006722, 2, "c1006722_OnEnterCombat")

--����4  ��ɱ 1006722 �������� x=780 y=3923 z=757 ˢ�� ͨڤ��ʯ 3040534

function c1006722_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3040534, 780, 3923, 757)

	--ˢ�¼�ħ̨
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1531102, x, y, z)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע��
aux.RegisterCreatureEvent(1006722,4,"c1006722_OnDie")
