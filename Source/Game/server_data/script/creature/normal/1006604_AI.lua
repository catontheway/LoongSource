--���ﺰ���ű�

-- �������ս������
function c1006604_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30118)

end

aux.RegisterCreatureEvent(1006604, 2, "c1006604_OnEnterCombat")

