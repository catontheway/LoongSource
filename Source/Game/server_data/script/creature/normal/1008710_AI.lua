--���ﺰ���ű�

-- �������ս������
function c1008710_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 40086)

end

aux.RegisterCreatureEvent(1008710, 2, "c1008710_OnEnterCombat")


--������������
function c1008710_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 40087)
	Increase_shili(MapID, InstanceID, RoleID, 1, 15, "jinjieshili_A")

end

aux.RegisterCreatureEvent(1008710, 4, "c1008710_OnDie")

