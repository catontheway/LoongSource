--���ﺰ���ű�

-- �������ս������
function c1006606_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30120)
	
end

aux.RegisterCreatureEvent(1006606, 2, "c1006606_OnEnterCombat")

