--���ﺰ���ű�

--������������
function c1008707_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 40082)

end

aux.RegisterCreatureEvent(1008707, 4, "c1008707_OnDie")

