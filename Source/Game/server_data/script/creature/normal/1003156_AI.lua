--���ﺰ���ű�

-- �������ս������
function c1003156_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 55)
	
end

aux.RegisterCreatureEvent(1003156, 2, "c1003156_OnEnterCombat")


--������������
function c1003156_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 57)	

end

aux.RegisterCreatureEvent(1003156, 4, "c1003156_OnDie")

