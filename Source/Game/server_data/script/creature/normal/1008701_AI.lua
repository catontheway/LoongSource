--���ﺰ���ű�

-- �������ս������
function c1008701_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40075)
	
end

aux.RegisterCreatureEvent(1008701, 2, "c1008701_OnEnterCombat")


--������������
function c1008701_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 40076)	

end

aux.RegisterCreatureEvent(1008701, 4, "c1008701_OnDie")

