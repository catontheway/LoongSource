--���ﺰ���ű�

-- �������ս������
function c1003109_OnEnterCombat(MapID, InstanceID, CreatureID)
                   
          cre.MonsterSay(MapID, InstanceID, CreatureID, 39)
       	
end

aux.RegisterCreatureEvent(1003109, 2, "c1003109_OnEnterCombat")


--������������
function c1003109_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
             cre.MonsterSay(MapID, InstanceID, TargetID, 41)	

end

aux.RegisterCreatureEvent(1003109, 4, "c1003109_OnDie")

