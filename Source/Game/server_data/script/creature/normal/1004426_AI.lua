--���ﺰ���ű�\ʾ���ű�

-- �������ս������
function c1004426_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10087)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1004426, 2, "c1004426_OnEnterCombat")


--������������
function c1004426_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10089)
 	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1004426, 4, "c1004426_OnDie")


