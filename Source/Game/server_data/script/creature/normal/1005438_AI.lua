--���ﺰ���ű�

-- �������ս������
function c1005438_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20072)

end

aux.RegisterCreatureEvent(1005438, 2, "c1005438_OnEnterCombat")


--������������
function c1005438_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20073)


end

aux.RegisterCreatureEvent(1005438, 4, "c1005438_OnDie")

