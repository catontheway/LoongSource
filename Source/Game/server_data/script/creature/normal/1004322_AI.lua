--���ﺰ���ű�

-- �������ս������
function c1004322_OnEnterCombat(MapID, InstanceID, CreatureID)
       
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10042)
	
end

aux.RegisterCreatureEvent(1004322, 2, "c1004322_OnEnterCombat")



--������������
function c1004322_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 10043)	

end

aux.RegisterCreatureEvent(1004322, 4, "c1004322_OnDie")

