--���ﺰ��/ʾ���ű�

-- �������ս������
function c1003122_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 81)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1003122, 2, "c1003122_OnEnterCombat")

--��������
function c1003122_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1003122, 4, "c1003122_OnDie")
