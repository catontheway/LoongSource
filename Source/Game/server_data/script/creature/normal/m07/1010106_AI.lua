--���ﺰ���ű�

-- �������ս������
function c1010106_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50051)

end

aux.RegisterCreatureEvent(1010106, 2, "c1010106_OnEnterCombat")



