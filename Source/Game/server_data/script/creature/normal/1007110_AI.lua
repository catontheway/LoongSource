--���ﺰ���ű�

-- �������ս������
function c1007110_OnEnterCombat(MapID, InstanceID, CreatureID)
      
       cre.MonsterSay(MapID, InstanceID, CreatureID, 40014)	
end

aux.RegisterCreatureEvent(1007110, 2, "c1007110_OnEnterCombat")



--������������
function c1007110_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 40015)	

end

aux.RegisterCreatureEvent(1007110, 4, "c1007110_OnDie")

