--���ﺰ���ű�

-- �������ս������
function c1006227_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30027)
	
end

aux.RegisterCreatureEvent(1006227, 2, "c1006227_OnEnterCombat")



--������������
function c1006227_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30028)	

end

aux.RegisterCreatureEvent(1006227, 4, "c1006227_OnDie")

