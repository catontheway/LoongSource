--���ﺰ���ű�

-- �������ս������
function c1003148_OnEnterCombat(MapID, InstanceID, CreatureID)
             
       cre.MonsterSay(MapID, InstanceID, CreatureID, 47)
       	
end

aux.RegisterCreatureEvent(1003148, 2, "c1003148_OnEnterCombat")


--������������
function c1003148_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	           
              cre.MonsterSay(MapID, InstanceID, TargetID, 49)
     
 end

aux.RegisterCreatureEvent(1003148, 4, "c1003148_OnDie")

