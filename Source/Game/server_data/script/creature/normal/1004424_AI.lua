--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1004424_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10082)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1004424, 2, "c1004424_OnEnterCombat")

--��������
function c1004424_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1004424, 4, "c1004424_OnDie")

