--���ﺰ���ű�

-- �������ս������
function c1005418_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20056)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1005418, 2, "c1005418_OnEnterCombat")


--������������
function c1005418_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 20058)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")


end

aux.RegisterCreatureEvent(1005418, 4, "c1005418_OnDie")





