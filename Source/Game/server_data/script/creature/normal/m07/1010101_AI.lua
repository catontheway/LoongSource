--���ﺰ���ű�

-- �������ս������
function c1010101_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50047)

end

aux.RegisterCreatureEvent(1010101, 2, "c1010101_OnEnterCombat")



