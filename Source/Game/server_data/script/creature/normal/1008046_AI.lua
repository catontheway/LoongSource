
-- �������ս������ʾ��
function c1008046_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008046, 2, "c1008046_OnEnterCombat")

--����ɱ ����ħ˧ 1008046 ˢ�� �̽�ħ˧ 1008047 ���꣺x 1902 y 21000 z 670
function c1008046_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008047, 1902, 21000, 670)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008046, 4, "c1008046_OnDie")
