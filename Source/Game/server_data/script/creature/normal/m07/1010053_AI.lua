--���ﺰ���ű�

-- �������ս������
function c1010053_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50041)

end

aux.RegisterCreatureEvent(1010053, 2, "c1010053_OnEnterCombat")

--��������
function c1010053_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")


end

aux.RegisterCreatureEvent(1010053, 4, "c1010053_OnDie")



