--���ﺰ���ű�

-- �������ս������
function c1005207_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20018)
	
end

aux.RegisterCreatureEvent(1005207, 2, "c1005207_OnEnterCombat")


--������������
function c1005207_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20019)	

end

aux.RegisterCreatureEvent(1005207, 4, "c1005207_OnDie")

