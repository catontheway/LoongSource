--���ﺰ���ű�

-- �������ս������
function c1009011_OnEnterCombat(MapID, InstanceID, CreatureID)

	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50003)

end

aux.RegisterCreatureEvent(1009011, 2, "c1009011_OnEnterCombat")


