--���ﺰ���ű�

-- �������ս������
function c1005215_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20025)

end

aux.RegisterCreatureEvent(1005215, 2, "c1005215_OnEnterCombat")



--������������
function c1005215_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20026)

end

aux.RegisterCreatureEvent(1005215, 4, "c1005215_OnDie")

