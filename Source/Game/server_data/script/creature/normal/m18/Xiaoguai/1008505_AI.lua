-- �������ս������ʾ��
function c1008505_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008505, 2, "c1008505_OnEnterCombat")


--��ɱ������(typeID��1008505)����(m18,569,33046,329)λ��ˢ��typeIDΪ1008508����ײ�����������ӵ��������ƣ������B03���������޴˵����㣬���ｫ������

function c1008505_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1008508, 569, 33046, 329, 1, "B03")
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008505, 4, "c1008505_OnDie")
