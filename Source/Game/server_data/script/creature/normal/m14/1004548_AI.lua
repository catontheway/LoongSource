--���ɱ���ý�����ID��1004548��������BOSS�ýǣ�ID��1004532������ x= 682 y= 3226 z= 705

function c1004548_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1004532, 682, 3226, 705)
end
aux.RegisterCreatureEvent(1004548, 4, "c1004548_OnDie")
