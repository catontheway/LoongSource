--���ﺰ���ű�

-- �������ս������
function c1009143_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50022)

end

aux.RegisterCreatureEvent(1009143, 2, "c1009143_OnEnterCombat")


--������������
function c1009143_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

     cre.MonsterSay(MapID, InstanceID, TargetID, 50023)

end

aux.RegisterCreatureEvent(1009143, 4, "c1009143_OnDie")

