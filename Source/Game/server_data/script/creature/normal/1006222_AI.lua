--���ﺰ���ű�

-- �������ս������
function c1006222_OnEnterCombat(MapID, InstanceID, CreatureID)
      cre.MonsterSay(MapID, InstanceID, CreatureID, 30024)
end

aux.RegisterCreatureEvent(1006222, 2, "c1006222_OnEnterCombat")



--������������
function c1006222_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

      cre.MonsterSay(MapID, InstanceID, TargetID, 30025)
end

aux.RegisterCreatureEvent(1006222, 4, "c1006222_OnDie")

