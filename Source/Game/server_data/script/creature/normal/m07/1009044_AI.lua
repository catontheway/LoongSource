--���ﺰ���ű�

-- �������ս������
function c1009044_OnEnterCombat(MapID, InstanceID, CreatureID)

	 cre.MonsterSay(MapID, InstanceID, CreatureID, 50012)

end

aux.RegisterCreatureEvent(1009044, 2, "c1009044_OnEnterCombat")


