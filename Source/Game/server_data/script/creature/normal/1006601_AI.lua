--���ﺰ���ű�

-- �������ս������
function c1006601_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30115)
	
end

aux.RegisterCreatureEvent(1006601, 2, "c1006601_OnEnterCombat")


--������������
function c1006601_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30116)	

end

aux.RegisterCreatureEvent(1006601, 4, "c1006601_OnDie")

