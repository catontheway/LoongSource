--���ﺰ���ű�

-- �������ս������
function c1006603_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30117)
	
end

aux.RegisterCreatureEvent(1006603, 2, "c1006603_OnEnterCombat")



