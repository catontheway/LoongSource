--���ﺰ���ű�

-- �������ս������
function c1009218_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50028)

end

aux.RegisterCreatureEvent(1009218, 2, "c1009218_OnEnterCombat")



