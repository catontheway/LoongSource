--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1005460_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20055)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1005460, 2, "c1005460_OnEnterCombat")

--������������
function c1005460_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1005460, 4, "c1005460_OnDie")
