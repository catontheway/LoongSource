--���ﺰ���ű�

-- �������ս������
function c1003157_OnEnterCombat(MapID, InstanceID, CreatureID)
 
             cre.MonsterSay(MapID, InstanceID, CreatureID, 58)
	
end

aux.RegisterCreatureEvent(1003157, 2, "c1003157_OnEnterCombat")


--������������
function c1003157_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 60)	

end

aux.RegisterCreatureEvent(1003157, 4, "c1003157_OnDie")

