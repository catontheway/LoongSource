--���ﺰ���ű�

-- �������ս������
function c1009048_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50013)

end

aux.RegisterCreatureEvent(1009048, 2, "c1009048_OnEnterCombat")


