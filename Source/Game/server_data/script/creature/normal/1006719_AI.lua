-- �������ս������ʾ��
function c1006719_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006719, 2, "c1006719_OnEnterCombat")

--����3  ��ɱ 1006719 �������� x=972 y= 2680 z=763 ˢ�� ͨڤ��ʯ 3040534

function c1006719_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3040534, 972, 2680, 763)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע��
aux.RegisterCreatureEvent(1006719,4,"c1006719_OnDie")


