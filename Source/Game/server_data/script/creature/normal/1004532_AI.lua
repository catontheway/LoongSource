--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1004532_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 10072)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1004532, 2, "c1004532_OnEnterCombat")

--��������
function c1004532_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1004532, 4, "c1004532_OnDie")
