--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1004427_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10090)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1004427, 2, "c1004427_OnEnterCombat")



--������������
function c1004427_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10091)
	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1004427, 4, "c1004427_OnDie")


