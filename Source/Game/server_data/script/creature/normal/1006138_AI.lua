--���ﺰ���ű�

-- �������ս������
function c1006138_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30131)
	
end

aux.RegisterCreatureEvent(1006138, 2, "c1006138_OnEnterCombat")

