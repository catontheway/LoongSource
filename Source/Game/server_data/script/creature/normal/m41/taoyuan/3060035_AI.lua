--������ Ӣ�鱦�� 3060035 ˢ�� Ӣ������ 1008033 / ������Ч txxy03 ���꣺x 1132 y 14867 z 519
function c3060035_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008033, 1132, 14867, 519)
end
aux.RegisterCreatureEvent(3060035, 6, "c3060035_OnInvest")
