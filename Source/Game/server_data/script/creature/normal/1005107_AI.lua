--���ﺰ���ű�

--������������
function c1005107_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20007)	

end

aux.RegisterCreatureEvent(1005107, 4, "c1005107_OnDie")

