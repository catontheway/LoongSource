--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1004531_OnEnterCombat(MapID, InstanceID, CreatureID)

   cre.MonsterSay(MapID, InstanceID, CreatureID, 10070)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1004531, 2, "c1004531_OnEnterCombat")


--��������
function c1004531_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1004531, 4, "c1004531_OnDie")

