--���ﺰ���ű�

-- �������ս������
function c1005407_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20051)

end

aux.RegisterCreatureEvent(1005407, 2, "c1005407_OnEnterCombat")

--������������
function c1005407_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 20053)
end

aux.RegisterCreatureEvent(1005407, 4, "c1005407_OnDie")

