--����ɱ ̰����� 1008080 ˢ�� ̰�� 1008074 ���꣺x 2437 y 26387 z 731������������ ������� 1008073 ��ʧ
function c1008080_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008074, 2437, 26387, 731)
	--map.MapDeleteCreature(MapID, InstanceID, 1008073)
end
aux.RegisterCreatureEvent(1008080, 4, "c1008080_OnDie")

-- �������ս������ʾ��
function c1008080_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008080, 2, "c1008080_OnEnterCombat")
