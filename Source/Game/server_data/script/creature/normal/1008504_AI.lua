
-- �������ս������ʾ��
function c1008504_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008504, 2, "c1008504_OnEnterCombat")


--��ɱǧ��ڤ��(typeID��1008504)����(m18,569,33046,454)λ��ˢ��typeIDΪ1008507����ײ�����������ӵ��������ƣ������B03���������޴˵����㣬���ｫ������
function c1008504_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1008507, 569, 33046, 454, 1, "B03")
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008504, 4, "c1008504_OnDie")
