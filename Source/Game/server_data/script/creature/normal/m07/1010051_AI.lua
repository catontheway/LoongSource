--���ﺰ���ű�

-- �������ս������
function c1010051_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50039)
end

aux.RegisterCreatureEvent(1010051, 2, "c1010051_OnEnterCombat")


--������������
function c1010051_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 50040)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")


end

aux.RegisterCreatureEvent(1010051, 4, "c1010051_OnDie")

