--���ﺰ���ű�

-- �������ս������
function c1006310_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30034)
	
end

aux.RegisterCreatureEvent(1006310, 2, "c1006310_OnEnterCombat")

