
-- �������ս������ʾ��
function c1008503_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008503, 2, "c1008503_OnEnterCombat")


--��ɱ��������(typeID��1008503)����(m18,620,22049,592)λ��ˢ��typeIDΪ1008506����ײ���Ѳ��·��ΪB02��
function c1008503_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1008506, 620, 22049, 592, 1, "B02")
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008503, 4, "c1008503_OnDie")
