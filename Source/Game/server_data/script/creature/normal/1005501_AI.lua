--���ﺰ���ű�

-- �������ս������
function c1005501_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20094)
	
end

aux.RegisterCreatureEvent(1005501, 2, "c1005501_OnEnterCombat")

--������������
function c1005501_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20095)

end

aux.RegisterCreatureEvent(1005501, 4, "c1005501_OnDie")

