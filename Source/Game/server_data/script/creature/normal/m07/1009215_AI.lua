--���ﺰ���ű�

-- �������ս������
function c1009215_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50026)

end

aux.RegisterCreatureEvent(1009215, 2, "c1009215_OnEnterCombat")



