--���ﺰ���ű�

-- �������ս������
function c1290102_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 85)
end

aux.RegisterCreatureEvent(1290102, 2, "c1290102_OnEnterCombat")


--������������
function c1290102_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 86)

end

aux.RegisterCreatureEvent(1290102, 4, "c1290102_OnDie")

