--���ﺰ���ű�

-- �������ս������
function c1006614_OnEnterCombat(MapID, InstanceID, CreatureID)
                 
         cre.MonsterSay(MapID, InstanceID, CreatureID, 30113)
       	
end

aux.RegisterCreatureEvent(1006614, 2, "c1006614_OnEnterCombat")



--������������
function c1006614_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	       
      cre.MonsterSay(MapID, InstanceID, TargetID, 30114)	

end

aux.RegisterCreatureEvent(1006614, 4, "c1006614_OnDie")

