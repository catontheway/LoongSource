--���ﺰ���ű�

-- �������ս������
function c1290103_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 87)
end

aux.RegisterCreatureEvent(1290103, 2, "c1290103_OnEnterCombat")


--������������
function c1290103_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 88)

end

aux.RegisterCreatureEvent(1290103, 4, "c1290103_OnDie")

