--���ﺰ���ű�

-- �������ս������
function c1006522_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30086)

end

aux.RegisterCreatureEvent(1006522, 2, "c1006522_OnEnterCombat")


--������������
function c1006522_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 30087)

end

aux.RegisterCreatureEvent(1006522, 4, "c1006522_OnDie")

