--���ﺰ���ű�

-- �������ս������
function c1006137_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30130)
	
end

aux.RegisterCreatureEvent(1006137, 2, "c1006137_OnEnterCombat")

