--���ﺰ���ű�

-- �������ս������
function c1290106_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10094)
end

aux.RegisterCreatureEvent(1290106, 2, "c1290106_OnEnterCombat")


--������������
function c1290106_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10095)

end

aux.RegisterCreatureEvent(1290106, 4, "c1290106_OnDie")

