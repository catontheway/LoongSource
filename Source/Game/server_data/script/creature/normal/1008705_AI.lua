--���ﺰ���ű�

-- �������ս������
function c1008705_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 40077)
end

aux.RegisterCreatureEvent(1008705, 2, "c1008705_OnEnterCombat")



--������������
function c1008705_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 40078)
	Increase_shili(MapID, InstanceID, RoleID, 1, 15, "jinjieshili_A")

end

aux.RegisterCreatureEvent(1008705, 4, "c1008705_OnDie")

