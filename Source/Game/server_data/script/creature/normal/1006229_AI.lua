--���ﺰ���ű�

-- �������ս������
function c1006229_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30030)
	
end

aux.RegisterCreatureEvent(1006229, 2, "c1006229_OnEnterCombat")



--������������
function c1006229_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30031)	

end

aux.RegisterCreatureEvent(1006229, 4, "c1006229_OnDie")

