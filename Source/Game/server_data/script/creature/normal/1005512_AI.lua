--���ﺰ���ű�

-- �������ս������
function c1005512_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20106)
	
end

aux.RegisterCreatureEvent(1005512, 2, "c1005512_OnEnterCombat")

