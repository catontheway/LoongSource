--���ﺰ���ű�

-- �������ս������
function c1005308_OnEnterCombat(MapID, InstanceID, CreatureID)
          
         cre.MonsterSay(MapID, InstanceID, CreatureID, 20032)	

end

aux.RegisterCreatureEvent(1005308, 2, "c1005308_OnEnterCombat")


--������������
function c1005308_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20034)

end

aux.RegisterCreatureEvent(1005308, 4, "c1005308_OnDie")

