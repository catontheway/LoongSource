--���ﺰ���ű�

-- �������ս������
function c1006213_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30016)
	
end

aux.RegisterCreatureEvent(1006213, 2, "c1006213_OnEnterCombat")



--������������
function c1006213_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30017)

end

aux.RegisterCreatureEvent(1006213, 4, "c1006213_OnDie")

