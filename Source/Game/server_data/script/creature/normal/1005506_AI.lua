--���ﺰ���ű�

-- �������ս������
function c1005506_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20098)
	
end

aux.RegisterCreatureEvent(1005506, 2, "c1005506_OnEnterCombat")

