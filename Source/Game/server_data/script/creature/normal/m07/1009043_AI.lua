--���ﺰ���ű�

-- �������ս������
function c1009043_OnEnterCombat(MapID, InstanceID, CreatureID)
 
	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50011)

end

aux.RegisterCreatureEvent(1009043, 2, "c1009043_OnEnterCombat")



