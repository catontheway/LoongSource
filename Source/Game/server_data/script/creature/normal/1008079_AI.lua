--����ɱ ̰����� 1008079 ˢ�� ̰�� 1008074 ���꣺x 2840 y 26387 z 316 ������������ ������� 1008073 ��ʧ
function c1008079_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008074, 2840, 26387, 316)
	--map.MapDeleteCreature(MapID, InstanceID, 1008073)
end
aux.RegisterCreatureEvent(1008079, 4, "c1008079_OnDie")

-- �������ս������ʾ��
function c1008079_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008079, 2, "c1008079_OnEnterCombat")
