--���ﺰ���ű�

-- �������ս������
function c1010054_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50042)

end

aux.RegisterCreatureEvent(1010054, 2, "c1010054_OnEnterCombat")


--������������
function c1010054_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")


end

aux.RegisterCreatureEvent(1010054, 4, "c1010054_OnDie")
