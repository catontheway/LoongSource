--���ﺰ���ű�

-- �������ս������
function c1009065_OnEnterCombat(MapID, InstanceID, CreatureID)

	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50017)

end

aux.RegisterCreatureEvent(1009065, 2, "c1009065_OnEnterCombat")


