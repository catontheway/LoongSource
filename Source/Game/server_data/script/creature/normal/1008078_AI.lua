--����ɱ ̰����� 1008078 ˢ�� ̰�� 1008074 ���꣺x 2819 y 26387 z 684 ������������ ������� 1008073 ��ʧ
function c1008078_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008074, 2819, 26387, 684)
	--map.MapDeleteCreature(MapID, InstanceID, 1008073)

end
aux.RegisterCreatureEvent(1008078, 4, "c1008078_OnDie")

-- �������ս������ʾ��
function c1008078_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008078, 2, "c1008078_OnEnterCombat")
