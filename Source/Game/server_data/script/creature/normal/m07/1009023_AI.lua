--���ﺰ���ű�

-- �������ս������
function c1009023_OnEnterCombat(MapID, InstanceID, CreatureID)

       cre.MonsterSay(MapID, InstanceID, CreatureID, 50008)

end

aux.RegisterCreatureEvent(1009023, 2, "c1009023_OnEnterCombat")

--������������
function c1009023_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
        
	     cre.MonsterSay(MapID, InstanceID, TargetID, 50009)

end

aux.RegisterCreatureEvent(1009023, 4, "c1009023_OnDie")

