--���ﺰ���ű�

-- �������ս������
function c1005508_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20100)
	
end

aux.RegisterCreatureEvent(1005508, 2, "c1005508_OnEnterCombat")

