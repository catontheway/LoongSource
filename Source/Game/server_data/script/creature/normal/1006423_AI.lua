--���ﺰ���ű�

-- �������ս������
function c1006423_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30062)
	
end

aux.RegisterCreatureEvent(1006423, 2, "c1006423_OnEnterCombat")

