--���ﺰ���ű�

-- �������ս������
function c1010201_OnEnterCombat(MapID, InstanceID, CreatureID)

    cre.MonsterSay(MapID, InstanceID, CreatureID, 50053)

end

aux.RegisterCreatureEvent(1010201, 2, "c1010201_OnEnterCombat")

