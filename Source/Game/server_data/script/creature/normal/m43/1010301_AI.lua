--���ﺰ���ű�

-- �������ս������
function c1010301_OnEnterCombat(MapID, InstanceID, CreatureID)

    cre.MonsterSay(MapID, InstanceID, CreatureID, 50057)

end

aux.RegisterCreatureEvent(1010301, 2, "c1010301_OnEnterCombat")

