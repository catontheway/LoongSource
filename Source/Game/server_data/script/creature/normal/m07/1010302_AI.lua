--���ﺰ���ű�

-- �������ս������
function c1010302_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 50058)

end

aux.RegisterCreatureEvent(1010302, 2, "c1010302_OnEnterCombat")


--��������
function c1010302_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

end
aux.RegisterCreatureEvent(1010302, 4, "c1010302_OnDie")
