--���ﺰ���ű�

-- �������ս������
function c1007116_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 40018)
	
end

aux.RegisterCreatureEvent(1007116, 2, "c1007116_OnEnterCombat")

