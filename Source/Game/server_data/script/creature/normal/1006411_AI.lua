
--���ﺰ���ű�

-- �������ս������
function c1006411_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30047)
	
end

aux.RegisterCreatureEvent(1006411, 2, "c1006411_OnEnterCombat")

--������������
function c1006411_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30049)	

end

aux.RegisterCreatureEvent(1006411, 4, "c1006411_OnDie")

