--���ﺰ���ű�

-- �������ս������
function c1006611_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30124)	
end

aux.RegisterCreatureEvent(1006611, 2, "c1006611_OnEnterCombat")

--������������
function c1006611_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30125)
end

aux.RegisterCreatureEvent(1006611, 4, "c1006611_OnDie")


