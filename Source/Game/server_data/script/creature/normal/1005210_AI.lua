--���ﺰ���ű�

-- �������ս������
function c1005210_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20021)

end

aux.RegisterCreatureEvent(1005210, 2, "c1005210_OnEnterCombat")


--������������
function c1005210_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20022)
	

end

aux.RegisterCreatureEvent(1005210, 4, "c1005210_OnDie")

