-- �������ս������/ʾ��
function c1008074_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 40046)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1008074, 2, "c1008074_OnEnterCombat")

--����ɱ ̰�� 1008074 �������Ա�ˢ�� ̰���� 3060026 -----------------------------���޸�
function c1008074_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3060026, 2437, 26387, 731)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 40047)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008074, 4, "c1008074_OnDie")





